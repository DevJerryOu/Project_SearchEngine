#ifndef _WEBPAGE_HPP_
#define _WEBPAGE_HPP_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include "Configuration.hpp"
#include "WordSegmentation.hpp"
using std::map;
using std::pair;
using std::sort;
using std::string;
using std::vector;
class WebPage
{
    const static int TOPK_NUMBER = 5;

public:
    WebPage(string &, Configuration &, WordSegmentation &);
    int getDocId() const;
	string getTitle() const;
	string getContent() const;
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
