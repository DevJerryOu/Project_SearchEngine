#include <iostream>
#include <fstream>
#include "Configuration.hpp"
#include "WordSegmentation.hpp"
#include "WebPage.hpp"
#include "PageLibPreprocessor.hpp"
#include "pagelib.hpp"
using std::ifstream;
void test1()
{
    // Configuration *conf = Configuration::getInstance();
    // DirScanner dir;
    // dir.traverse((*conf)["web_yuliao"]);

    // PageLib pagelib(dir, *conf);
    // pagelib.create();
    // pagelib.store();
    // PageLibPreprocessor prepro(*conf);
}
void test2()
{
    cout << "enter main()" << endl;
    Configuration *conf = Configuration::getInstance();
    WordSegmentation jieba;

    // WebPage page1(str1, *conf, jieba);
    // WebPage page2(str2, *conf, jieba);
    // if (page1 == page2)
    // {
    //     cout << "page1=page2" << endl;
    // }
    // else
    //     cout << "page1!=page2" << endl;
    // return 0;
}
int main()
{
    test2();
}