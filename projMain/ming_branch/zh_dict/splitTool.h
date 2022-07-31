#ifndef __SplitTool_H__
#define __SplitTool_H__

//分词工具:获取文件，把文件传入分词工具容器
#include "cppjieba/Jieba.hpp"
#include "title.h"

using std::cout;
using std::endl ;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::cerr;
using std::unique_ptr;


const char * const DICT_PATH = "./cppjieba/dict/jieba.dict.utf8"; //最大概率法(MPSegment: Max Probability) 分词所使用的词典路径
const char * const HMM_PATH = "./cppjieba/dict/hmm_model.utf8"; //隐式马尔科夫模型(HMMSegment: Hi dden Markov Model ) 分词所使用的词典路径
const char * const USER_DICT_PATH = "./cppjieba/dict/user.dict.utf8";  //用户自定义词典路径
const char* const IDF_PATH = "./cppjieba/dict/idf.utf8"; //IDF路径. 
const char* const STOP_WORD_PATH = "./cppjieba/dict/stop_words.utf8"; //停用词路径

class SplitTool
{
public:
    SplitTool() {}
    virtual ~SplitTool() {}
    virtual vector<string> cut() = 0;
};

class Cppjieba
: public SplitTool
{
public:
    Cppjieba();
    ~Cppjieba();
    vector<string> cut(const string &str);
    void getFiles();
    string *getF(){
        return &_files;
    }
private:
    cppjieba::Jieba _jieba;
    vector<string> _files;
};

#endif
