#ifndef _PAGELIBPREPROCESSOR_HPP_
#define _PAGELIBPREPROCESSOR_HPP_
#include <math.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <utility>
#include "WordSegmentation.hpp"
#include "WebPage.hpp"
#include "../include/simhash-master/include/simhash/Simhasher.hpp"
using std::istringstream;
using std::map; // add
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;
using namespace simhash;            // add
typedef unsigned long int unit64_t; // add
typedef unsigned int unit12_t;      // add
class PageLibPreprocessor
{
public:
    PageLibPreprocessor(Configuration &);
    void doProcess();
    int getPageNum();

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
    int _pageNum;

    void getSimhash();                                           // add
    map<int, unit64_t> _simhashVal;                              //所有文章的simhash值//add
    unordered_map<unit12_t, vector<pair<unit64_t, int>>> _partA; // add
    unordered_map<unit12_t, vector<pair<unit64_t, int>>> _partB; // add
    unordered_map<unit12_t, vector<pair<unit64_t, int>>> _partC; // add
    unordered_map<unit12_t, vector<pair<unit64_t, int>>> _partD; // add
};
#endif
