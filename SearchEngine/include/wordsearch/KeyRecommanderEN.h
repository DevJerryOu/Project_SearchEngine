#ifndef __KEYRECOMMANDEREN_H__
#define __KEYRECOMMANDEREN_H__

#include "DictionaryEN.h"
// DictionaryEN *DictionaryEN::_pInstance = nullptr;
// DictionaryEN::AutoRelease DictionaryEN::_ar;

class KeyRecommanderEN
{
public:
    KeyRecommanderEN(const std::string sought);
    DictionaryEN *dicEN = DictionaryEN::createInstance();
    set<std::string> doQuery(const std::string &dictPath, const std::string &indexPath);

private:
    std::string _sought;

    // TcpConnectionPtr _con;
};

#endif