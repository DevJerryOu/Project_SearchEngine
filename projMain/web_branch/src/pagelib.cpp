#include "pagelib.hpp"
PageLib::PageLib(DirScanner &dirscanner, Configuration &conf)
	: _dirScanner(dirscanner), _conf(conf)
{
	_pages.reserve(6000); //预开空间
}
void PageLib::create()
{ //创建网页库和偏移库
	vector<string> names = _dirScanner.files();
	cout << "names.size():" << names.size() << endl;
	int article = 0;		 //统计所有页面的文章总数
	unsigned int length = 0; //统计总长度
	string path = _conf["web_yuliao"] + "/";
	for (auto name : names)
	{
		string filename = path + name;
		XMLDocument doc;
		doc.LoadFile(filename.c_str());
		if (doc.ErrorID())
		{
			cout << doc.ErrorIDToName << endl;
			cout << "loadfile fail!" << endl;
			exit(0);
		}
		XMLElement *itemNode = doc.RootElement()->FirstChildElement("channel")->FirstChildElement("item");
		while (itemNode)
		{
			string title = itemNode->FirstChildElement("title")->GetText();
			string link = itemNode->FirstChildElement("link")->GetText();
			// content都没有，若有description，优先用它替代，若没有description,用title替代
			string content;
			if (itemNode->FirstChildElement("description"))
				content = itemNode->FirstChildElement("description")->GetText();
			else
				content = title;
			regex reg("<[^>]+>");
			content = regex_replace(content, reg, "");
			//存入_pages
			string str;
			str += "<doc>\n";
			str += "<docid>" + std::to_string(article + 1) + "</docid>\n";
			str += "<title>" + title + "</title>\n";
			str += "<link>" + link + "</link>\n";
			str += "<content>" + content + "</content>\n";
			str += "</doc>\n";
			_pages.push_back(str);
			int len = str.length();
			//存入_offsetLib
			_offsetLib.insert({article + 1, {length, len}});
			article++;
			length += len;
			itemNode = itemNode->NextSiblingElement("item");
		}
	}
	/*
	for(auto page:_pages){
		cout<<page.c_str()<<endl;
	}
	for(auto elem:_offsetLib){
		cout<<elem.first<<" "<<elem.second.first<<" "<<elem.second.second<<endl;
	}
	cout<<"article:"<<article<<endl;
	cout<<"length:"<<length<<endl;
	*/
}

void PageLib::store()
{
	ofstream ofs(_conf["webpage"]);
	if (!ofs)
	{
		cout << "open newripepage.dat fail!" << endl;
		exit(0);
	}
	for (auto page : _pages)
	{
		ofs << page;
	}
	ofs.close();
	ofstream ofs1(_conf["offset"]);
	if (!ofs1)
	{
		cout << "open newoffset.dat fail!" << endl;
		exit(0);
	}
	for (int i = 1; i <= _offsetLib.size(); i++)
	{
		ofs1 << i << " " << _offsetLib[i].first << " " << _offsetLib[i].second << endl;
	}
	ofs1.close();
}
