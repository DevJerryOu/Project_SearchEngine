#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__

#include "KeyRecommanderEN.h"
#include "KeyRecommanderCN.h"

// DictionaryEN *DictionaryEN::_pInstance = nullptr;
// DictionaryEN::AutoRelease DictionaryEN::_ar;

// DictionaryCN *DictionaryCN::_pInstance = nullptr;
// DictionaryCN::AutoRelease DictionaryCN::_ar;

size_t getByteNum_UTF8(const char byte)
{
    int byteNum = 0;
    for (size_t i = 0; i < 6; ++i)
    {
        if (byte & (1 << (7 - i)))
            ++byteNum;
        else
            break;
    }

    return byteNum == 0 ? 1 : byteNum;
}

set<string> commander(const std::string &chineseQuery, const std::string &englishQuery)
{
    cout << "chineseQuery" << chineseQuery << " englishQuery" << englishQuery << endl;
    DictionaryCN *dicCN = DictionaryCN::createInstance();
    KeyRecommanderCN keyCN(chineseQuery);
    set<string> st1 = keyCN.doQuery("../configuration/dictCHN.dat", "../configuration/IndexCHN.dat");

    DictionaryEN *dicEN = DictionaryEN::createInstance();
    KeyRecommanderEN keyEN(englishQuery);
    set<string> st2 = keyEN.doQuery("../configuration/dictEN.dat", "../configuration/IndexEN.dat");
    set<string> ret_st;
    for (auto &item : st1)
    {
        ret_st.insert(item);
    }
    for (auto &item : st2)
    {
        ret_st.insert(item);
    }
    return ret_st;
}

set<string> keyWords(const std::string &word)
{
    char *Chinese = nullptr;
    char *Endlish = nullptr;
    int i = 0;
    const char *ch = word.c_str();
    // cout << word.size() <<endl;
    std::string subWord1 = "";
    std::string subWord2 = "";

    while (i < word.size())
    {
        {
            if (getByteNum_UTF8(ch[i]) == 3)
            {
                std::string subWord = word.substr(i, 3);
                subWord1 = subWord1 + subWord;

                i += 3;
                // cout<<"CHN"<<endl;
            }
            else
            {
                std::string subWord = word.substr(i, 1);
                subWord2 = subWord2 + subWord;

                ++i;
                // cout<<"EN"<<endl;
            }
        }
    }
    // cout << subWord1 << endl;
    // cout << subWord2 << endl;

    return commander(subWord1, subWord2);
}

#endif
