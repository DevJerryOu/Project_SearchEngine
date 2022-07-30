#include "DictProducer.h"

DictProducer::DictProducer(const string &filename)
{
	buildEnDict(filename);
	createIndex();
}

void DictProducer::buildEnDict(const string &filename)
{
	ifstream ifs(filename);
	if (!ifs)
	{
		cerr << "ifs open" << endl;
		return;
	}

	string line;
	while (getline(ifs, line))
	{
		istringstream iss(line);
		string word;
		while (iss >> word)
		{
			string newWord = dealWord(word);
			if (string() != newWord)
			{
				++_dict[newWord];
			}
		}
	}
	ifs.close();
}

void DictProducer::createIndex()
{
	vector<string> wordInDict;
	for (map<string, int>::iterator it = _dict.begin(); it != _dict.end(); ++it) {
		wordInDict.push_back(it->first);
	}//将词典的单词放到vector中

	int countLine = 0;
	for (vector<string>::iterator it = wordInDict.begin(); it != wordInDict.end(); ++it)
	{
		const char *ch = wordInDict[countLine++].c_str();//c_str()的返回值是const char*
		int letterInwrod = 0;
		while (ch[letterInwrod])
		{
			_dict_index[ch[letterInwrod]].insert(countLine);
			letterInwrod++;
			//将该单词的行号给单词中每一个字母
		}
	}

}

string DictProducer::dealWord(string &word)
{
	for (int c = 0; c < word.size(); ++c)
	{
		if (word[c] >= 'A' && word[c] <= 'Z')
		{
			word[c] = tolower(word[c]);
		}

		if (!isalpha(word[c]))
		{
			word.erase(c);
		}
	}

	//转换为合理的单词
	return word;
}

void DictProducer::storeDict(const char *path)
{
	ofstream ofs(path);
	if (!ofs)
	{
		cerr << "dict ofs open " << path << " error!" << endl;
		return;
	}

	map<string, int>::iterator it;
	for (it = _dict.begin(); it != _dict.end(); ++it)
	{
		ofs << it->first << "  " << it->second << endl;
	}

	ofs.close();
}

void DictProducer::storeIndex(const char *path)
{
	ofstream ofs(path);
	if (!ofs)
	{
		cerr << "ofs open " << path << " error!" << endl;
		return;
	}

	map<char, set<int> >::iterator it;
	for (it = _dict_index.begin(); it != _dict_index.end(); ++it)
	{
		ofs << it->first << " ";
		set<int> tmp = it->second;
		for (set<int>::iterator i = tmp.begin(); i != tmp.end(); i++)
		{
			ofs << *i << " ";
		}
		ofs << endl;
	}

	ofs.close();
}
