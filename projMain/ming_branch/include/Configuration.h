#ifndef __Configuration_H__
#define __Configuration_H__

#include <string>
#include <map>

using std::map;
using std::string;

class Configuration
{
public:
    Configuration(const string &filepath);
    ~Configuration();
    map<string,string> &getConfigMap();

private:
    void readconf();

private:
    string _filepath;
    map<string,string> _confmap;
};





#endif
