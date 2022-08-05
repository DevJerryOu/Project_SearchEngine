#ifndef __PAGELIB_HH__
#define __PAGELIB_HH__
#include "tinyxml2.h"
#include "DirScanner.hpp"
#include "Configuration.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
using namespace std;
using namespace tinyxml2;
/*struct RSSItem{
	string _title;
	string _link;
	string _description;
	string _content;
};*/

class PageLib
{
public:
	PageLib(DirScanner &dirScanner, Configuration &); //构造函数
	void create();									  //创建网页库和位置偏移库，写到数据结构里
	void store();									  //存储网页库和位置偏移库，写到文件里
													  //	void renewConf();//更新配置文件，将pagelib,offsetlib文件名+相对地址写进去
private:
	vector<std::string> _pages;			 //存所有网页的文章RSS结构
	map<int, pair<int, int>> _offsetLib; //存所有的偏移量
	DirScanner _dirScanner;
	Configuration &_conf;
};

#endif
