#ifndef __DICTIONARYEN_H__
#define __DICTIONARYEN_H__

#include "tamporary.h"

class DictionaryEN
{
    class AutoRelease
    {
    public:
        ~AutoRelease()
		{
			//cout << "~AutoRelease()" << endl;
			if(_pInstance) {
				delete _pInstance;
			}
		}
    };
public:
    static DictionaryEN* createInstance()
    {
        if(_pInstance == nullptr) {
	    _pInstance = new DictionaryEN();//在堆上创建对象
	    }
	    return _pInstance;
    }

    void init(const std::string dictPath, const std::string indexPath);
    vector<pair<std::string, int> >& getDict();
    map<char, set<int> >& getIndexTable();
    multimap<int, std::string>& candidateWords(const std::string& queryWord);
private:
	DictionaryEN() {	}
    ~DictionaryEN() {		}
private:
    vector<pair<std::string, int> > _dict;
    map<char, set<int> > _dict_index;
    multimap<int, std::string> _candidateWords;

    static DictionaryEN * _pInstance;
	static AutoRelease _ar;
};


#endif

