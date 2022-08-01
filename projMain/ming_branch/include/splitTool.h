#ifndef __splitTool_H__
#define __splitTool_H__

#include "cppjieba/Jieba.hpp"
#include <vector>
#include <string>

using std::vector;
using std::string;

const char * const DICT_PATH = "./cppjieba/dict/jieba.dict.utf8"; //最大概率法(MPSegment: Max Probability) 分词所使用的词典路径
const char * const HMM_PATH = "./cppjieba/dict/hmm_model.utf8"; //隐式马尔科夫模型(HMMSegment: Hi dden Markov Model ) 分词所使用的词典路径
const char * const USER_DICT_PATH = "./cppjieba/dict/user.dict.utf8";  //用户自定义词典路径
const char* const IDF_PATH = "./cppjieba/dict/idf.utf8"; //IDF路径. 
const char* const STOP_WORD_PATH = "./cppjieba/dict/stop_words.utf8"; //停用词路径

class splitTool
{
public:
    splitTool()
    : _jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH) {  }

    ~splitTool() {}

    vector<string> cut(string str)  //分词
    {
        vector<string> words;
        _jieba.CutForSearch(str,words);
        return words;
    }
private:
    cppjieba::Jieba _jieba; 
};



#endif
