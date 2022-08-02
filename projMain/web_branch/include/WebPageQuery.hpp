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
using std::string;
using std::unordered_map;
using std::vector;
class WebPageQuery
{
private:
    WordSegmentation _jieba;
    unordered_map<int, pair<int, int>> _offsetLib;
    unordered_map<string, map<int, double>> _invertIndexTable;
    int _pagenum;
    string web_path;

private:
    double Calcos(vector<double> &v1, vector<double> &v2);
    string readWebPage(int, int);

public:
    WebPageQuery(Configuration &, int);
    string doQuery(const string &str);
    void LoadLibrary(string, string);
    vector<double> getQueryWordsWeightVector(vector<string> &);
    bool executeQuery(const vector<string> &queryWords);
    string returnNoAnswer();
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