#ifndef _SPLITTOOL_HPP_
#define _SPLITTOOL_HPP_
#include <iostream>
#include <vector>
#include <string>
using std::string;
using std::vector;
class SplitTool
{
public:
    SplitTool() {}
    virtual ~SplitTool() {}
    virtual vector<string> cut(const string &sentence) = 0;
};
#endif