#include "Configuration.hpp"
Configuration *Configuration::getInstance()
{
    if (nullptr == _SingleConf)
    {
        _SingleConf = new Configuration();
        atexit(destroy);
    }
    return _SingleConf;
}
map<string, string> &Configuration::getConfigMap()
{
    ifstream ifs;
    ifs.open(_configFilePath);
    string line;
    while (getline(ifs, line))
    {
        istringstream istr(line);
        string tmp_str1, tmp_str2;
        istr >> tmp_str1 >> tmp_str2;
        _configs[tmp_str1] = tmp_str2;
    }
    return _configs;
}
void Configuration::destroy()
{
    if (_SingleConf)
    {
        delete _SingleConf;
        _SingleConf = nullptr;
    }
}
Configuration *Configuration::_SingleConf = getInstance();
string Configuration::_configFilePath = "../conf/configure.txt";