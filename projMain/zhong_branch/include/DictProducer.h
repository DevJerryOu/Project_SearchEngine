#ifndef _DICTPRODUCER_H
#define _DICTPRODUCER_H
#include "tamporary.h"

class DictProducer
{
public:
	DictProducer(const string &filename);

	void buildEnDict(const string &filename);
	void createIndex();
	void storeDict(const char *path);
	void storeIndex(const char *path);

	//void getFile();

	//void showDict();

private:
	string dealWord(string &word);
	map<string, int> _dict;//´Ê ´ÊÆµ->´Êµä
	map<char, set<int> > _dict_index;//26¸ö×ÖÄ¸ËùÔÚ´ÊµäµÄÐÐºÅ
	//vector<string> _file;
};

#endif 
