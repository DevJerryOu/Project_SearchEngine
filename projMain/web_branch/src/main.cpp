#include <iostream>
#include "Configuration.hpp"
#include "DirScanner.hpp"
int main()
{
    Configuration *conf = Configuration::getInstance();
    map<string, string> mp = conf->getConfigMap();
    DirScanner dscan;
    for (auto iter = mp.begin(); iter != mp.end(); iter++)
    {
        if (iter->first == "web_yuliao")
        {
            dscan.traverse(iter->second);
            break;
        }
    }
    int count = 0;
    vector<string> tmp = dscan.files();
    for (auto iter = tmp.begin(); iter != tmp.end(); iter++)
    {
        cout << *iter << endl;
        count++;
    }
    cout << count << endl;
}