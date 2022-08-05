#include "KeyRecommanderEN.h"

DictionaryEN* DictionaryEN::_pInstance = nullptr;
DictionaryEN::AutoRelease DictionaryEN::_ar;
KeyRecommanderEN::KeyRecommanderEN(const std::string sought)
    : _sought(sought)
{
}

set<std::string> KeyRecommanderEN::doQuery(const std::string &dictPath, const std::string &indexPath)
{
    set<std::string> result;
    dicEN->init(dictPath, indexPath);

    multimap<int, std::string> tmp;
    tmp = dicEN->candidateWords(_sought);

    // multimap<int,string>::iterator it = tmp.begin();
    //  for (;it != tmp.end();it++)
    //  {
    //      cout << it->first<<"  " <<it->second <<endl;
    //  }

    for (int i = 0; i < 3; i++) // i =  shortDistance
    {
        multimap<int, std::string>::iterator it = tmp.find(i); // multimap to find key to value
        // cout << "instance : "<< i << endl;
        for (int j = 0, len = tmp.count(i); j < len; ++j, ++it)
        {

            auto insertPtr = result.end();
            result.emplace_hint(insertPtr, it->second);
            insertPtr = result.end();

            // cout << it->second << endl;
            // cout << "emplaced" <<endl;
            // cout<<endl;
            if (result.size() > 2)
            {
                // set<string>::iterator delFirst = result.begin();
                // result.erase(delFirst);

                for (auto it : result)
                {
                    cout << it << endl;
                }
                break;
            }
        }
        if (result.size() > 2)
        {
            break;
        }
    }
    return result;
}