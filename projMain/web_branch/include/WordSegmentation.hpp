#ifndef _WORDSEGMENTATION_HPP_
#define _WORDSEGMENTATION_HPP_
#include <iostream>
#include "Configuration.hpp"
#include "SplitTool.hpp"
#include "../include/simhash/cppjieba/Jieba.hpp"//change
#define LOGGER_LEVEL LL_WARN
class WordSegmentation : public SplitTool
{
public:
    WordSegmentation();
    ~WordSegmentation() {}
    vector<string> cut(const string &sentence);

private:
};
#endif
