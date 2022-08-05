#include "Configuration.hpp"
Configuration *Configuration::getInstance()
{
    pthread_once(&_once, init);
    return _pCon;
}
void Configuration::init()
{
    _pCon = new Configuration(_configFilePath);
    atexit(destroy);
}
void Configuration::destroy()
{
    if (_pCon)
    {
        delete _pCon;
        _pCon = nullptr;
    }
}
Configuration::Configuration(const std::string &path)
{
    ifstream ifs;
    ifs.open(path);
    std::string line, stop_words_path;
    while (getline(ifs, line))
    {
        istringstream istr(line);
        std::string tmp_str1, tmp_str2;
        istr >> tmp_str1 >> tmp_str2;
        _configs[tmp_str1] = tmp_str2;
        if (tmp_str1 == "stop_words_zh")
        {
            stop_words_path = tmp_str2;
        }
    }
    ifs.close();

    ifs.open(stop_words_path);
    while (getline(ifs, line))
    {
        stopWordList.insert(line);
    }
    ifs.close();
}
map<std::string, std::string> &Configuration::getConfigMap()
{
    return _configs;
}
set<std::string> &Configuration::getStopWordList()
{
    return stopWordList;
}
const std::string &Configuration::operator[](const std::string &str)
{
    return _configs[str];
}
pthread_once_t Configuration::_once = PTHREAD_ONCE_INIT;
std::string Configuration::_configFilePath = "../conf/configure.txt";
Configuration *Configuration::_pCon = Configuration::getInstance();
