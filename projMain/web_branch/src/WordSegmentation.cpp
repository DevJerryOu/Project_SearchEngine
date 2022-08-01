#include "WordSegmentation.hpp"
WordSegmentation::WordSegmentation()
{
}
vector<string> WordSegmentation::cut(const string &sentence)
{
    const char *const DICT_PATH = "../include/simhash-master/dict/jieba.dict.utf8";
    const char *const HMM_PATH = "../include/simhash-master/dict/hmm_model.utf8";
    const char *const IDF_PATH = "../include/simhash-master/dict/idf.utf8";

    cppjieba::Jieba jieba(DICT_PATH,
                          HMM_PATH,
                          IDF_PATH);
    vector<string> words;

    jieba.CutAll(sentence, words);
    return words;
}
