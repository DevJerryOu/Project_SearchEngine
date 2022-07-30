#ifndef __PAGELIB_HH__
#define __PAGELIB_HH__
#include"fileprocesser.hh"
#include"DirScanner.hpp"
class PageLib{
public:
	PageLib(DirScanner& dirScanner,FileProcesser &fileprocesser);//构造函数
	void create();//创建网页库和位置偏移库，写到数据结构里
	void store();//存储网页库和位置偏移库，写到文件里
//	void renewConf();//更新配置文件，将pagelib,offsetlib文件名+相对地址写进去
private:
	vector<string> _pages;
	map<int,pair<int,int>> _offsetLib;
	FileProcesser _fileProcesser;
	DirScanner _dirScanner;
//	Configuration _conf;
};


#endif
