#ifndef _SPLITTOOL_HPP_
#define _SPLITTOOL_HPP_
#include <iostream>
#include <vector>
#include <string>
using std::vector;
class SplitTool
{
public:
    SplitTool() {}
    virtual ~SplitTool() {}
    virtual vector<std::string> cut(const std::string &sentence) = 0;
};
#endif