#ifndef _WORDSEGMENTATION_HPP_
#define _WORDSEGMENTATION_HPP_
#include <iostream>
#include "Configuration.hpp"
#include "SplitTool.hpp"
#include "simhash-master/deps/cppjieba/Jieba.hpp"
class WordSegmentation : public SplitTool
{
public:
    WordSegmentation();
    ~WordSegmentation() {}
    vector<std::string> cut(const std::string &sentence);

private:
};
#endif
