#include <iostream>
#include <fstream>
#include "Configuration.hpp"
#include "WordSegmentation.hpp"
#include "WebPage.hpp"
#include "PageLibPreprocessor.hpp"
#include "pagelib.hpp"
using std::ifstream;
int main()
{
    Configuration *conf = Configuration::getInstance();
    DirScanner dir;
    dir.traverse((*conf)["web_yuliao"]);

    PageLib pagelib(dir, *conf);
    pagelib.create();
    pagelib.store();
    // PageLibPreprocessor prepro(*conf);
}