#include "../include/Configuration.h"

#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;

Configuration::Configuration(const string &filepath)
: _filepath(filepath)
{
    readconf();
}

Configuration::~Configuration() {}

map<string,string> &Configuration::getConfigMap()
{
    return _filepath;
}

void Configuration::readconf()
{
    ifstream ifs(_filepath,ifstram::in);
    if(!ifs.good())
    {
        cout<<"Failed to open file!\n";
        exit(0);
    }
    string lines;
    string first, second;
    while (getline(ifs, lines))
    {
        stringstream ss(lines);
        ss >> first >> second;
        _confmap[first] = second;
    }
    ifs.close();
}
