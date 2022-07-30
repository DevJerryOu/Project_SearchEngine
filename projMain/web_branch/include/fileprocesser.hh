#ifndef __FILEPROCESSER_HH__
#define __FILEPROCESSER_HH__
#include"tinyxml2.h"
#include<iostream>
#include<string>
#include<regex>
#include<vector>
#include<map>
#include<utility>
#include<fstream>
using namespace std;
using namespace tinyxml2;
struct RSSItem{
	string _title;
	string _link;
	string _description;
	string _content;
};

class FileProcesser{//给文件名，用process()输出字符串
public:
	FileProcesser(size_t capacity);
	void read();//读出文件每个RSSItem，压入_rss中
	string process(const string& filename);
	map<int,pair<int,int>> offset();
private:
	string _titleFeature;//这个就是文件名
	vector<RSSItem> _rss;//RSS结构文件
	string _fileStr;//返回的string	
	map<int,pair<int,int>> _offset;//每个文件的offset
};

#endif
