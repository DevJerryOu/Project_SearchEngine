#ifndef _WEBPAGE_HPP_
#define _WEBPAGE_HPP_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#define LOGGER_LEVEL LL_WARN
#include "Configuration.hpp"
#include "WordSegmentation.hpp"
#include "../include/simhash-master/include/simhash/Simhasher.hpp"
using std::map;
using std::pair;
using std::sort;
using std::string;
using std::vector;
using namespace simhash;
typedef unsigned long int unit64_t;
class WebPage
{
    const static int TOPK_NUMBER = 10;

    friend bool operator==(const WebPage &lhs, const WebPage &rhs); //判断两篇文章是否相等
    friend bool operator<(const WebPage &lhs, const WebPage &rhs);  //对文章按docid进行排序

public:
    WebPage(string &, Configuration &, WordSegmentation &);
    int getDocId() const;
    string getDoc() const;
    map<string, int> &getWordsMap();

private:
    void processDoc(const string &, Configuration &, WordSegmentation &);
    void calcTopK(vector<string> &, int, set<string> &);
    void clearStop(Configuration &);

private:
    string _doc;
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
    string _docSummary;
    vector<string> _topWords;   //词频最高的10个词
    map<string, int> _wordsMap; //文章所有词语及频率
};

typedef pair<string, int> PAIR;
struct CmpByValue
{
    bool operator()(const PAIR &lhs, const PAIR &rhs)
    {
        return lhs.second > rhs.second;
    }
};
#endif
