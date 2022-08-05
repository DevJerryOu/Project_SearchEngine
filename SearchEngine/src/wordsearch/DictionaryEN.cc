#include "DictionaryEN.h"
#include "EditDistanceEN.h"

void DictionaryEN::init(const std::string dictPath, const std::string indexPath)
{
    // cout << "init start" <<endl;
    // cout << dictPath << endl;
    // cout << indexPath << endl;

    ifstream ifs1(dictPath);
    if (!ifs1)
    {
        cerr << "ifs1 open failed" << endl;
        return;
    }

    ifstream ifs2(indexPath);
    if (!ifs2)
    {
        cerr << "ifs2 open failed" << endl;
        return;
    }
    std::string line;
    std::string word;
    char letter;
    int num;
    while (getline(ifs1, line), !ifs1.eof())
    {
        istringstream iss(line);
        iss >> word;
        iss >> num;
        _dict.push_back(make_pair(word, num));
    }
    // cout << "dict is ok" <<endl;

    while (getline(ifs2, line), !ifs2.eof())
    {
        istringstream iss(line);
        iss >> letter;
        while (iss >> num)
        {
            _dict_index[letter].insert(num);
        }
    }
    // cout << "index is ok" <<endl;
    ifs1.close();
    ifs2.close();

    // cout << "init is ok" <<endl;
}

vector<pair<std::string, int>> &DictionaryEN::getDict()
{
    return _dict;
}
map<char, set<int>> &DictionaryEN::getIndexTable()
{
    return _dict_index;
}

multimap<int, std::string> &DictionaryEN::candidateWords(const std::string &queryWord)
{
    _candidateWords.clear();
    const char *str = queryWord.c_str();
    for (int i = 0; i < strlen(str); i++)
    {
        char tmpChar = str[i];
        for (set<int>::iterator it = _dict_index[tmpChar].begin(); it != _dict_index[tmpChar].end(); it++)
        {
            int ret = editDistanceEN(queryWord, _dict[*it].first);
            if (ret <= 2)
            {
                // cout << _dict[*it].first <<endl;
                _candidateWords.insert(make_pair(ret, _dict[*it].first));
                //_candidateWords.insert(_dict[*it].first);
            }
        }
    }
    // cout << "candidateWrods is ok" <<endl;
    return _candidateWords;
}