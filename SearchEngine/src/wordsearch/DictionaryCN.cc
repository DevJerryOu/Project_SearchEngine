#include "DictionaryCN.h"
#include "EditDistanceCN.h"

void DictionaryCN::init(const std::string dictPath, const std::string indexPath)
{
    // cout << "init start" <<endl;
    // cout << dictPath << endl;
    // cout << indexPath << endl;

    ifstream ifs1(dictPath);
    if (!ifs1)
    {
        cerr << "ifs1 open" << endl;
        return;
    }

    ifstream ifs2(indexPath);
    if (!ifs2)
    {
        cerr << "ifs2 open" << endl;
        return;
    }
    std::string line;
    std::string word;
    std::string letter;
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

vector<pair<std::string, int>> &DictionaryCN::getDict()
{
    return _dict;
}
map<std::string, set<int>> &DictionaryCN::getIndexTable()
{
    return _dict_index;
}

multimap<int, std::string> &DictionaryCN::candidateWords(const std::string &queryWord)
{
    // cout << "candidateWrods is start" <<endl;
    _candidateWords.clear();
    const char *str = queryWord.c_str();

    std::string word = queryWord;
    vector<std::string> aCHNword;
    size_t charNums = word.size() / nBytesCode(word[0]);
    for (size_t idx = 0, n = 0; n != charNums; ++idx, ++n) //得到字符数
    {
        //按照字符个数切割
        size_t charLen = nBytesCode(word[idx]);
        std::string subWord = word.substr(idx, charLen); //按照编码格式，进行拆分
        aCHNword.emplace_back(subWord);
        idx += (charLen - 1);
    }

    for (vector<std::string>::iterator zi = aCHNword.begin(); zi != aCHNword.end(); zi++)
    {
        for (set<int>::iterator it = _dict_index[*zi].begin(); it != _dict_index[*zi].end(); it++)
        {
            int ret = editDistance(queryWord, _dict[*it].first);
            if (ret <= 4)
            {
                // cout << _dict[*it].first << endl;
                _candidateWords.insert(make_pair(ret, _dict[*it].first));
            }
        }
    }
    // cout << "candidateWrods is ok" <<endl;
    return _candidateWords;
}