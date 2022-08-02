#include "WebPage.hpp"
WebPage::WebPage(string &str, Configuration &conf, WordSegmentation &wordsegmentation)
{ //构造函数里调用文档的处理函数
    processDoc(str, conf, wordsegmentation);
    //	cout<<"process finish"<<endl;
    //生成摘要,频率top5词组成
    for (int i = 0; i < TOPK_NUMBER; i++)
    {
        _docSummary += _topWords[i];
        if (i != TOPK_NUMBER - 1)
        {
            _docSummary += " ";
        }
    }
    // cout << _docSummary << endl;
}
int WebPage::getDocId() const
{
    return _docId;
}
string WebPage::getTitle() const
{
    return _docTitle;
}
string WebPage::getContent() const
{
    return _docContent;
}
string WebPage::getDoc() const
{
    return _doc;
}
string WebPage::getSummary() const
{
    return _docSummary;
}
map<string, int> &WebPage::getWordsMap()
{
    return _wordsMap;
}
void WebPage::processDoc(const string &str, Configuration &conf, WordSegmentation &wordsegmentation)
{
    //先提取网页库的string 提取出每个string的id，title，url，content，summary
    //调用jieba对文档进行分割，并解析出每个词语的使用频率
    // cout << "process start" << endl;
    _doc = str;
    int pos1 = str.find("<docid>");
    int pos2 = str.find("<title>");
    int pos3 = str.find("<link>");
    int pos4 = str.find("<content>");
    int pos5 = str.find("</doc>");
    _docId = stoi(str.substr(pos1 + 7, pos2 - 15 - pos1));
    _docTitle = str.substr(pos2 + 7, pos3 - 15 - pos2);
    _docUrl = str.substr(pos3 + 6, pos4 - pos3 - 13);
    _docContent = str.substr(pos4 + 9, pos5 - pos4 - 19);
    // cout << "cut start1" << endl;
    string cutWord = _docTitle + _docContent;
    vector<string> vec = wordsegmentation.cut(cutWord);
    // cout << "cut finish1" << endl;
    for (int i = 0; i < vec.size(); i++)
    {
        _wordsMap[vec[i]]++;
    }
    calcTopK(_topWords, TOPK_NUMBER, conf.getStopWordList());
}
void WebPage::calcTopK(vector<string> &wordsVec, int k, set<string> &stopWordList)
{
    set<string> st2;
    for (auto iter = _wordsMap.begin(); iter != _wordsMap.end(); iter++)
    {
        if (stopWordList.find(iter->first) != stopWordList.end())
        {
            st2.insert(iter->first);
        }
    }
    for (auto iter = st2.begin(); iter != st2.end(); iter++)
    {
        _wordsMap.erase(*iter);
    }

    //按词频排序，提取前5个词频
    vector<PAIR> vec_tmp(_wordsMap.begin(), _wordsMap.end());
    //	cout<<"vec_tmp.size():"<<vec_tmp.size()<<endl;
    sort(vec_tmp.begin(), vec_tmp.end(), CmpByValue());
    //	cout<<"sort finish"<<endl;
    for (int i = 0; i < k; i++)
    {
        // cout << vec_tmp[i].first << endl;
        //		cout<<"for():"<<i<<endl;
        wordsVec.push_back(vec_tmp[i].first);
        //		cout<<"push_back finish"<<endl;
    }
}
