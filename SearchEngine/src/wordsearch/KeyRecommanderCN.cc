#include "KeyRecommanderCN.h"

DictionaryCN *DictionaryCN::_pInstance = nullptr;
DictionaryCN::AutoRelease DictionaryCN::_ar;
KeyRecommanderCN::KeyRecommanderCN(const std::string sought)
    : _sought(sought)
{
}

set<std::string> KeyRecommanderCN::doQuery(const std::string dictPath, const std::string indexPath)
{
    set<std::string> result;
    dicCN->init(dictPath, indexPath);

    multimap<int, std::string> tmp;
    // cout << "_sought=" << _sought << endl;
    tmp = dicCN->candidateWords(_sought);

    multimap<int, string>::iterator it = tmp.begin();
    for (; it != tmp.end(); it++)
    {
        cout << it->first << "  " << it->second << endl;
    }

    for (int i = 0; i < 3; i++)
    {
        multimap<int, std::string>::iterator it = tmp.find(i); // multimap to find key to value
        // cout << "instance : "<< i << endl;
        for (int j = 0, len = tmp.count(i); i < len; ++j, ++it)
        {

            auto insertPtr = result.end();
            result.emplace_hint(insertPtr, it->second);
            insertPtr = result.end();

            // result.emplace_back(it->second);
            // cout << "emplaced" <<endl;
            // cout<<endl;
            if (result.size() > 3)
            {
                set<std::string>::iterator delFirst = result.begin();
                result.erase(delFirst);

                for (auto it : result)
                {
                    cout << it << endl;
                }
                break;
            }
        }
        if (result.size() > 3)
        {
            break;
        }
    }
    // for (auto item : result)
    // {
    //     cout << "doQuery=" << item << endl;
    // }
    return result;
}