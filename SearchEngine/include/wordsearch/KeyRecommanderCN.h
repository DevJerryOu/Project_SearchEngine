#ifndef __KEYRECOMMANDERCN_H__
#define __KEYRECOMMANDERCN_H__

#include "DictionaryCN.h"
// DictionaryCN *DictionaryCN::_pInstance = nullptr;
// DictionaryCN::AutoRelease DictionaryCN::_ar;

class KeyRecommanderCN
{
public:
    KeyRecommanderCN(const std::string sought);
    DictionaryCN *dicCN = DictionaryCN::createInstance();
    set<std::string> doQuery(const std::string dictPath, const std::string indexPath);

private:
    std::string _sought;

    // TcpConnectionPtr _con;
};

#endif