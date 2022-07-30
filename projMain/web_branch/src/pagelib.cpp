#include"pagelib.hh"
#include<iostream>
#include<fstream>
using namespace std;

PageLib::PageLib(DirScanner& dirscanner,FileProcesser& fileprocesser)
:_dirScanner(dirscanner)
,_fileProcesser(fileprocesser)
{
	_pages.reserve(500);
}
void PageLib::create(){
	vector<string> names=_dirScanner.files();
	int mapLine=0;
	for(auto filename:names){
		_pages.push_back(_fileProcesser.process(filename));
		map<int,pair<int,int>> offset=_fileProcesser.offset();
		
		for(int i=1;i<=offset.size();i++){
			_offsetLib.insert({i+mapLine,{offset[i].first,offset[i].second}});
		}
		mapLine+=offset.size();
	}
	for(auto str:_pages){
		cout<<str;
	}
	for(int i=1;i<=_offsetLib.size();i++){
		cout<<i<<" "<<_offsetLib[i].first<<" "<<_offsetLib[i].second<<endl;
	}
}

void PageLib::store(){
	ofstream ofs("../data/newripepage.dat");
	if(!ofs){
		cout<<"open newripepage.dat fail!"<<endl;
		exit(0);
	}
	for(auto page:_pages){
		ofs<<page;
	}
	ofs.close();
	ofstream ofs1("../data/newoffset.dat");
	if(!ofs1){
		cout<<"open newoffset.dat fail!"<<endl;
		exit(0);
	}
	for(int i=1;i<=_offsetLib.size();i++){
		ofs1<<i<<" "<<_offsetLib[i].first<<" "<<_offsetLib[i].second<<endl; 
	}
	ofs1.close();
}
