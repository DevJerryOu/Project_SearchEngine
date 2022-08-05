#ifndef __splitTool_H__
#define __splitTool_H__

#include "cppjieba/Jieba.hpp"

#include "tamporary.h"

#if 0
const char * const DICT_PATH = "cppjieba/dict/jieba.dict.utf8"; //最大概率法(MPSegment: Max Probability) 分词所使用的词典路径
const char * const HMM_PATH = "cppjieba/dict/hmm_model.utf8"; //隐式马尔科夫模型(HMMSegment: Hi dden Markov Model ) 分词所使用的词典路径
const char * const USER_DICT_PATH = "cppjieba/dict/user.dict.utf8";  //用户自定义词典路径
const char* const IDF_PATH = "cppjieba/dict/idf.utf8"; //IDF路径. 
const char* const STOP_WORD_PATH = "cppjieba/dict/stop_words.utf8"; //停用词路径
#endif
const char *const DICT_PATH = "../include/websearch/simhash-master/dict/jieba.dict.utf8";
const char *const HMM_PATH = "../include/websearch/simhash-master/dict/hmm_model.utf8";
const char *const IDF_PATH = "../include/websearch/simhash-master/dict/idf.utf8";

// class DictProdeucer;
class splitTool
{
    // friend class DictProdeucer;
public:
    splitTool()
        : _jieba(DICT_PATH, HMM_PATH, IDF_PATH)
    {
        cout << "jieba is statr" << endl;
    }

    ~splitTool() {}

    vector<std::string> operator()(const std::string str) //分词
    {
        vector<std::string> words;
        _jieba.CutAll(str, words);
        return words;
    }

private:
    cppjieba::Jieba _jieba;
};

#endif