#include "WebPageSearcher.hpp"
WebPageSearcher::WebPageSearcher(string keys, Configuration &conf, int pagenum) : _sought(keys), _conf(conf), _webpagequery(conf, pagenum)
{
}
void WebPageSearcher::doQuery()
{
    cout << "WebPageSearcher" << endl;
    //进行搜索，得到搜索结果
    string result = _webpagequery.doQuery(_sought);
    cout << "result" << result << endl;
}