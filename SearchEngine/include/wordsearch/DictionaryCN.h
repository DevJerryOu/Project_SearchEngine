#ifndef __DICTIONARYCN_H__
#define __DICTIONARYCN_H__

#include "tamporary.h"

class DictionaryCN
{
    class AutoRelease
    {
    public:
        ~AutoRelease()
		{
			if(_pInstance) {
				delete _pInstance;
			}
		}
    };
public:
    static DictionaryCN* createInstance()
    {
        if(_pInstance == nullptr) {
	    _pInstance = new DictionaryCN();//在堆上创建对象
	    }
	    return _pInstance;
    }

    void init(const std::string dictPath, const std::string indexPath);
    vector<pair<std::string, int> >& getDict();
    map<std::string, set<int> >& getIndexTable();
    multimap<int, std::string>& candidateWords(const std::string& queryWord);
private:
	DictionaryCN() {}
    ~DictionaryCN() {}
private:
    vector<pair<std::string, int> > _dict;
    map<std::string, set<int> > _dict_index;
    multimap<int, std::string> _candidateWords;

    static DictionaryCN * _pInstance;
	static AutoRelease _ar;
};
// Dictionary* Dictionary::_pInstance = nullptr;
// Dictionary::AutoRelease Dictionary::_ar;

#endif

