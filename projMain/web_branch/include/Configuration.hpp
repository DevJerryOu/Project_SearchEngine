#ifndef _CONFIGURATION_HPP_
#define _CONFIGURATION_HPP_
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <set>
#include <stdlib.h>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::map;
using std::set;
using std::string;
class Configuration
{
public:
    static Configuration *getInstance();
    static void init();
    static void destroy();

    map<string, string> &getConfigMap();
    set<string> &getStopWordList();
    const string &operator[](const string &);

private:
    Configuration(const string &);
    ~Configuration() {}
    static Configuration *_pCon;
    static string _configFilePath;
    static pthread_once_t _once;
    map<string, string> _configs;
    set<string> stopWordList;
};
#endif