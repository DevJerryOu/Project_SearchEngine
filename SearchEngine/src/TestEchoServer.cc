#include "../include/framework/EchoServer.h"
#include "../include/websearch/PageLibPreprocessor.hpp"
#include "../include/websearch/pagelib.hpp"
#if 0
#include "../include/cache/RedisPPCache.h"
#endif
int main(int argc, char **argv)
{
    EchoServer es(4, 10, "127.0.0.1", 8888);
    es.start();

    // Configuration *conf = Configuration::getInstance();
//生成离线文档
#if 0
    DirScanner dir;
    dir.traverse((*conf)["web_yuliao"]);
    PageLib pagelib(dir, *conf);
    pagelib.create();
    pagelib.store();
    PageLibPreprocessor pageprocess(*conf); //先read！填完_pageLib _offsetLib
    pageprocess.doProcess();
    int pageNum = atoi((*conf)["pageNum"].c_str());
    cout << "pageNum=" << pageNum << endl;

    string str1 = "时代新人";
    WebPageSearcher seach(str1, *conf, pageNum);
    seach.doQuery();
#endif
    return 0;
}
