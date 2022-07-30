#ifndef _CONFIGURATION_HPP_
#define _CONFIGURATION_HPP_
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <stdlib.h>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::map;
using std::string;
class Configuration
{
public:
    static Configuration *getInstance();
    map<string, string> &getConfigMap();
    static void destroy();

private:
    Configuration() {}
    ~Configuration() {}
    static string _configFilePath;
    map<string, string> _configs;
    static Configuration *_SingleConf;
};
#endif