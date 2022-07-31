#include "WordSegmentation.hpp"
WordSegmentation::WordSegmentation()
{
}
vector<string> WordSegmentation::cut(const string &sentence)
{
    const char *const DICT_PATH = "../include/cppjieba/dict/jieba.dict.utf8";
    const char *const HMM_PATH = "../include/cppjieba/dict/hmm_model.utf8";
    const char *const USER_DICT_PATH = "../include/cppjieba/dict/user.dict.utf8";
    const char *const IDF_PATH = "../include/cppjieba/dict/idf.utf8";
    const char *const STOP_WORD_PATH = "../include/cppjieba/dict/stop_words.utf8";

    cppjieba::Jieba jieba(DICT_PATH,
                          HMM_PATH,
                          USER_DICT_PATH,
                          IDF_PATH,
                          STOP_WORD_PATH);
    vector<string> words;

    jieba.CutAll(sentence, words);
    return words;
}
