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
#if 0
    Configuration *conf = Configuration::getInstance();
    //生成离线文档
    DirScanner dir;
    dir.traverse((*conf)["web_yuliao"]);
    PageLib pagelib(dir, *conf);
    pagelib.create();
    pagelib.store();                        //生成网页库
    PageLibPreprocessor pageprocess(*conf); //先read！填完_pageLib _offsetLib
    pageprocess.doProcess();
#endif
    return 0;
}
