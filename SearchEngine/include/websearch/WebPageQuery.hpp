#ifndef _WEBPAGEQUERY_HPP_
#define _WEBPAGEQUERY_HPP_
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <iterator>
#include <utility>
#include <fstream>
#include <sstream>
#include <math.h>
#include "WordSegmentation.hpp"
#include "Configuration.hpp"
#include "WebPage.hpp"
using std::ifstream;
using std::insert_iterator;
using std::istringstream;
using std::map;
using std::pair;
using std::unordered_map;
using std::vector;
class WebPageQuery
{
private:
    WordSegmentation _jieba;
    unordered_map<int, pair<int, int>> _offsetLib;
    unordered_map<std::string, map<int, double>> _invertIndexTable;
    int _pagenum;
    std::string web_path;

private:
    double Calcos(vector<double> &v1, vector<double> &v2);
    std::string readWebPage(int, int);
    size_t nBytesCode(const char);

public:
    WebPageQuery(Configuration &, int);
    std::string doQuery(const std::string &str);
    void LoadLibrary(std::string, std::string);
    vector<double> getQueryWordsWeightVector(vector<std::string> &);
    bool executeQuery(const vector<std::string> &queryWords);
    std::string returnNoAnswer();
    std::string getContent(vector<std::string>, string);
};
typedef pair<int, double> PAIR2;
struct CmpByValue2
{
    bool operator()(const PAIR2 &lhs, const PAIR2 &rhs)
    {
        return lhs.second > rhs.second;
    }
};
#endif
