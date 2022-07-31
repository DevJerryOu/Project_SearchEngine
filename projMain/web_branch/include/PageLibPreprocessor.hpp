#ifndef _PAGELIBPREPROCESSOR_HPP_
#define _PAGELIBPREPROCESSOR_HPP_
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include "WordSegmentation.hpp"
#include "WebPage.hpp"
using std::istringstream;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;
class PageLibPreprocessor
{
public:
    PageLibPreprocessor(Configuration &);
    void doProcess();

private:
    void readInfoFromFile(string, string, Configuration &);
    void cutRedundantPages();
    void buildInvertIndexTable();
    void storeOnDisk();

private:
    WordSegmentation _jieba;
    vector<WebPage> _pageLib;                                           //文档表
    unordered_map<int, pair<int, int>> _offsetLib;                      //偏移表
    unordered_map<string, vector<pair<int, double>>> _invertIndexTable; //倒排索引表
};
#endif