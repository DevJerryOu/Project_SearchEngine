#include"fileprocesser.hh"
#include"tinyxml2.h"
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<utility>
#include<regex>
using namespace std;
using namespace tinyxml2;

FileProcesser::FileProcesser(size_t capacity=1000)
{
	_rss.reserve(capacity);
}
void FileProcesser::read(){
	XMLDocument doc;
	doc.LoadFile(_titleFeature.c_str());
	if(doc.ErrorID()){
		cout<<"loadfile fail!"<<endl;
		exit(0);
	}
	XMLElement *itemNode=doc.RootElement()->FirstChildElement("channel")->FirstChildElement("item");
	while(itemNode){
		string title=itemNode->FirstChildElement("title")->GetText();
		string link=itemNode->FirstChildElement("link")->GetText();
		string description;
		if(itemNode->FirstChildElement("description"))
			description=itemNode->FirstChildElement("description")->GetText();
		else
			description=title;
		string content=title;//内容都没有
		
		regex reg("<[^>]+>");//一个看不懂的正则表达式，为了去掉中间的/p，只有description和content里有
		description=regex_replace(description,reg,"");
		content=regex_replace(content,reg,"");
		RSSItem item;
		item._title=title;
		item._link=link;
		item._description=description;
		item._content=content;
		_rss.push_back(item);
		itemNode=itemNode->NextSiblingElement("item");
	}
}
map<int,pair<int,int>> FileProcesser::offset(){
	/*for(int i=1;i<=_offset.size();i++){
		cout<<i<<" "<<_offset[i].first<<" "<<_offset[i].second<<endl;
	}*/
	return _offset;
}
string FileProcesser::process(const string& filename){
	_titleFeature="../resource/yuliao_web/"+filename;
	cout<<_titleFeature<<endl;
	FileProcesser::read();
	for(int i=0;i<_rss.size();i++){
		int size0=_fileStr.size();
		_fileStr+="<doc>\n";
		_fileStr+="<docid>"+std::to_string(i+1)+"</docid>\n";
		_fileStr+="<title>"+_rss[i]._title+"</title>\n";
		_fileStr+="<link>"+_rss[i]._link+"</link>\n";
		_fileStr+="<description>"+_rss[i]._description+"</description>\n";
		_fileStr+="<content>"+_rss[i]._content+"</content>\n";
		_fileStr+="</doc>\n";
		int size1=_fileStr.size();
		_offset.insert({i+1,{size0,size1-size0}});
	}
	FileProcesser::offset();
	return _fileStr;
}

