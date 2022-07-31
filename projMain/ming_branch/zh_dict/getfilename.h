#ifndef __getfilename_H__
#define __getfilename_H__

#include "cppjieba/Jieba.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

const char * const DICT_PATH = "./GetFilename/dict/jieba.dict.utf8"; //最大概率法(MPSegment: Max Probability) 分词所使用的词典路径
const char * const HMM_PATH = "./GetFilename/dict/hmm_model.utf8"; //隐式马尔科夫模型(HMMSegment: Hi dden Markov Model ) 分词所使用的词典路径
const char * const USER_DICT_PATH = "./GetFilename/dict/user.dict.utf8";  //用户自定义词典路径
const char* const IDF_PATH = "./GetFilename/dict/idf.utf8"; //IDF路径. 
const char* const STOP_WORD_PATH = "./GetFilename/dict/stop_words.utf8"; //停用词路径

class GetFilename
{
public:
    GetFilename() 
    : _jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH) 
    { 
        getFile(); 
         openfile();
    }

    ~GetFilename() {}
    
private:
    void openfile();
    void getFile(); //获取art文件夹目录下的所有文件
    vector<string> cut(const string str)
    {
        vector<string> words;
        _jieba.CutForSearch(str,words);
        return words;
    }

private:
    vector<string> _files;
    map<string,string> _smap;

    cppjieba::Jieba _jieba;  

    vector<pair<string,int>> _dict;
    unordered_map<string,int> _dIndex;
};


#endif
