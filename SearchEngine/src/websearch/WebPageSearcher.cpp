#include "WebPageSearcher.hpp"
WebPageSearcher::WebPageSearcher(std::string keys, Configuration &conf, int pagenum) : _sought(keys), _conf(conf), _webpagequery(conf, pagenum)
{
}
vector<std::string> WebPageSearcher::doQuery()
{
    cout << "WebPageSearcher" << endl;
    //进行搜索，得到搜索结果
    std::string result = _webpagequery.doQuery(_sought);
    vector<std::string> ret;
    ret.reserve(2);
    if (result.find("Error") != -1)
    {
        return ret;
    }
    else
    {
        int pos1 = result.find("Title:");
        int pos2 = result.find("Abstract:");
        std::string title;
        std::string abstract;
        title = result.substr(pos1 + 6, pos2 - pos1 - 7);
        abstract = result.substr(pos2 + 9, result.size() - pos2 - 9);
        ret.push_back(title);
        ret.push_back(abstract);
    }
    return ret;
}
