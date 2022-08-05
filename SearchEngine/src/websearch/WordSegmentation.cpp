#include "WordSegmentation.hpp"
WordSegmentation::WordSegmentation()
{
}
vector<std::string> WordSegmentation::cut(const std::string &sentence)
{
    const char *const DICT_PATH = "../include/websearch/simhash-master/dict/jieba.dict.utf8";
    const char *const HMM_PATH = "../include/websearch/simhash-master/dict/hmm_model.utf8";
    const char *const IDF_PATH = "../include/websearch/simhash-master/dict/idf.utf8";

    cppjieba::Jieba jieba(DICT_PATH,
                          HMM_PATH,
                          IDF_PATH);
    vector<std::string> words;

    jieba.CutAll(sentence, words);
    return words;
}
