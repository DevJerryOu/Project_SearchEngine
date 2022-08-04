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
using std::map;
using std::set;
using std::istringstream;
class Configuration
{
public:
    static Configuration *getInstance();
    static void init();
    static void destroy();

    map<std::string, std::string> &getConfigMap();
    set<std::string> &getStopWordList();
    const std::string &operator[](const std::string &);

private:
    Configuration(const std::string &);
    ~Configuration() {}
    static Configuration *_pCon;
    static std::string _configFilePath;
    static pthread_once_t _once;
    map<std::string, std::string> _configs;
    set<std::string> stopWordList;
};
#endif
