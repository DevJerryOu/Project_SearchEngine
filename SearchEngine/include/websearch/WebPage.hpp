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
#include "simhash-master/include/simhash/Simhasher.hpp"
using std::map;
using std::pair;
using std::sort;
using std::vector;
using namespace simhash;
typedef unsigned long int unit64_t;
class WebPage
{
    const static int TOPK_NUMBER = 5;

public:
    WebPage(std::string &, Configuration &, WordSegmentation &);
    int getDocId() const;
    std::string getTitle() const;
    std::string getContent() const;
    std::string getDoc() const;
    std::string getSummary() const;
    map<std::string, int> &getWordsMap();

private:
    void processDoc(const std::string &, Configuration &, WordSegmentation &);
    void calcTopK(vector<std::string> &, int, set<string> &);
    void clearStop(Configuration &);

private:
    std::string _doc;
    int _docId;
    std::string _docTitle;
    std::string _docUrl;
    std::string _docContent;
    std::string _docSummary;
    vector<std::string> _topWords;   //词频最高的5个词
    map<std::string, int> _wordsMap; //文章所有词语及频率
};

typedef pair<std::string, int> PAIR;
struct CmpByValue
{
    bool operator()(const PAIR &lhs, const PAIR &rhs)
    {
        return lhs.second > rhs.second;
    }
};
#endif
