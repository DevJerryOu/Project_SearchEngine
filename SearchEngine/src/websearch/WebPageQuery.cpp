#include "WebPageQuery.hpp"
WebPageQuery::WebPageQuery(Configuration &conf, int pageNum) : _jieba(), _pagenum(pageNum)
{
    LoadLibrary(conf["offset"], conf["invertIndex"]);
    web_path = conf["webpage"];
}
std::string WebPageQuery::doQuery(const std::string &str)
{
    //去倒排索引表中查找，调用execute query函数。若存在查询词不在索引表中，就认为没找到网页，调用noAnswer函数

    vector<std::string> segWord = _jieba.cut(str);
    for (int i = 0; i < segWord.size(); i++)
    {
        cout << "segWord=" << segWord[i] << endl;
    }
    if (executeQuery(segWord) == false)
    {
        return returnNoAnswer();
    }
    cout << "executeQuery success" << endl;
    //将查询关键字视为文档，计算权重，生成基准向量。调用getquerywordweight函数
    vector<double> SearchWordWeight = getQueryWordsWeightVector(segWord); //获得了这篇文章的基向量
                                                                          //取每个查询关键字的网页id+权重，取交集，得到网页向量
    cout << "searchWordWeight" << endl;
    vector<vector<int>> page_inter(segWord.size());
    for (int i = 0; i < segWord.size(); i++)
    {
        for (auto item : _invertIndexTable[segWord[i]])
        {
            page_inter[i].push_back(item.first);
        }
    }

#if 0
    for (int i = 0; i < page_inter.size(); i++)
    {
        for (int j = 0; j < page_inter[i].size(); j++)
        {
            cout << page_inter[i][j] << " ";
        }
        cout << endl;
    }
#endif

    //取网页的交集
    sort(page_inter[0].begin(), page_inter[0].end());
    vector<int> res1(page_inter[0].begin(), page_inter[0].end()), res2;
    for (int i = 1; i < page_inter.size(); i++)
    {
        sort(page_inter[i].begin(), page_inter[i].end());
        std::set_intersection(res1.begin(), res1.end(), page_inter[i].begin(), page_inter[i].end(), insert_iterator<vector<int>>(res2, res2.begin()));
        res1.assign(res2.begin(), res2.end());
        res2.clear();
    }

    if (res1.size() == 0)
    {
        return returnNoAnswer();
    }

    vector<vector<double>> YBase(res1.size());
    map<int, double> XYcos;
    for (int i = 0; i < res1.size(); i++)
    {
        for (int j = 0; j < segWord.size(); j++)
        {
            YBase[i].push_back(_invertIndexTable[segWord[j]][res1[i]]);
        }
        XYcos.insert({res1[i], Calcos(SearchWordWeight, YBase[i])});
    }
#if 0
    for (auto item : XYcos)
    {
        cout << item.first << " " << item.second << endl;
    }
#endif
    vector<PAIR2> vec_tmp(XYcos.begin(), XYcos.end());
    sort(vec_tmp.begin(), vec_tmp.end(), CmpByValue2());

    int offsize = _offsetLib[vec_tmp[0].first].first;
    int filesize = _offsetLib[vec_tmp[0].first].second;
    WordSegmentation ws;
    std::string str_tmp = readWebPage(offsize, filesize);
    // cout << str_tmp << endl;
    WebPage page(str_tmp, *(Configuration::getInstance()), ws);
    // cout << page.getDoc() << endl;
    return "Title:" + page.getTitle() + "\nAbstract:" + page.getSummary();
}
void WebPageQuery::LoadLibrary(std::string offPath, std::string invertIndex)
{
    ifstream ifs;
    ifs.open(offPath);
    std::string str;
    while (getline(ifs, str))
    {
        istringstream istr(str);
        std::string tmp1, tmp2, tmp3;
        istr >> tmp1 >> tmp2 >> tmp3;
        _offsetLib.insert({atoi(tmp1.c_str()), {atoi(tmp2.c_str()), atoi(tmp3.c_str())}});
    }
    cout << "_offsetLib=" << _offsetLib.size() << endl;
    ifs.close();

    ifs.open(invertIndex);
    while (getline(ifs, str))
    {
        istringstream istr(str);
        std::string keys, tmp1, tmp2;
        while (istr >> keys >> tmp1 >> tmp2)
        {
            _invertIndexTable[keys].insert({atoi(tmp1.c_str()), atof(tmp2.c_str())});
        }
    }
    cout << "_invertIndex=" << _invertIndexTable.size() << endl;
    ifs.close();

    // for (auto item : _invertIndexTable)
    // {
    //     cout << item << " ";
    //     for (auto item2 : item.second)
    //     {
    //         cout << item2.first << " " << item.second;
    //     }
    //     cout << endl;
    // }
}
vector<double> WebPageQuery::getQueryWordsWeightVector(vector<std::string> &searchWord)
{
    //计算查询词的权重值
    vector<double> RetWeight;
    RetWeight.reserve(searchWord.size());
    map<std::string, pair<double, double>> word;
    //将关键字看成一篇文章，计算该关键字在文章的次数
    for (int i = 0; i < searchWord.size(); i++)
    {
        if (word.find(searchWord[i]) != word.end())
        {
            word[searchWord[i]].first++;
        }
        else
        {
            word[searchWord[i]].first = 1;
            word[searchWord[i]].second = 0;
        }
    }
    double sum = 0;
    for (auto iter = word.begin(); iter != word.end(); iter++)
    {
        double DF = _invertIndexTable[iter->first].size() + 1;
        double tmp = log2((_pagenum + 1) / (1 + DF)) * iter->second.first;
        iter->second.second = tmp;
        sum += tmp * tmp;
    }
    for (auto iter = word.begin(); iter != word.end(); iter++)
    {
        iter->second.second = iter->second.second / sqrt(sum);
    }
    for (int i = 0; i < searchWord.size(); i++)
    {
        RetWeight.push_back(word[searchWord[i]].second);
    }
    return RetWeight;
}
bool WebPageQuery::executeQuery(const vector<std::string> &queryWords)
{
    //执行查询
    for (int i = 0; i < queryWords.size(); i++)
    {
        if (_invertIndexTable.find(queryWords[i]) == _invertIndexTable.end())
        {
            return false;
        }
        // else
        // {
        //     cout << queryWords[i] << "find success" << endl;
        //     for (auto item : _invertIndexTable[queryWords[i]])
        //     {
        //         cout << item.first << " " << item.second;
        //     }
        //     cout << endl;
        // }
    }
    return true;
}
std::string WebPageQuery::returnNoAnswer()
{
    return "Error:not found";
}
double WebPageQuery::Calcos(vector<double> &v1, vector<double> &v2)
{
    double tmp1 = 0, tmp2 = 0, tmp3 = 0;
    for (int i = 0; i < v1.size(); i++)
    {
        tmp1 += v1[i] * v2[i];
        tmp2 += v1[i] * v1[i];
        tmp3 += v2[i] * v2[i];
    }
    tmp2 = sqrt(tmp2);
    tmp3 = sqrt(tmp3);
    return tmp1 / (tmp2 * tmp3);
}
std::string WebPageQuery::readWebPage(int offset, int fsize)
{
    ifstream ifs;
    ifs.open(web_path);
    std::string line;
    int all = offset;
    std::string web = "";
    while (getline(ifs, line))
    {
        if (all == 0)
        {
            web += line;
            fsize -= (line.size() + 1);
            if (fsize == 0)
            {
                break;
            }
        }
        else
        {
            all -= (line.size() + 1);
        }
    }
    ifs.close();
    return web;
}
