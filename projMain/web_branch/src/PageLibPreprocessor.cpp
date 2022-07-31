#include "PageLibPreprocessor.hpp"
PageLibPreprocessor::PageLibPreprocessor(Configuration &conf) : _jieba()
{
    //调用readinfofromfile读取网页库和偏移库的信息，填充至_pageLib, _offsetLib
    string web_path = conf["webpage"];
    string offset_path = conf["offset"];
    readInfoFromFile(web_path, offset_path, conf);
    // cout << _pageLib.size() << endl;
}
void PageLibPreprocessor::doProcess()
{ //去除冗余网页，并创建倒排索引，并将其存至磁盘
  // cutRedundantPages();
  // buildInvertIndexTable();
  // storeOnDisk();
}
void PageLibPreprocessor::readInfoFromFile(string webPath, string offPath, Configuration &conf)
{
    ifstream ifs_web, ifs_off;
    ifs_web.open(webPath);
    ifs_off.open(offPath);
    string off_line;
    while (getline(ifs_off, off_line))
    {
        istringstream istr(off_line);
        string tmp1, tmp2, tmp3;
        istr >> tmp1 >> tmp2 >> tmp3;
        _offsetLib.insert({atoi(tmp1.c_str()), {atoi(tmp2.c_str()), atoi(tmp3.c_str())}});
        int fsize = atoi(tmp3.c_str());
        string line_content, file_content;
        while (getline(ifs_web, line_content))
        {
            file_content += line_content;
            fsize -= line_content.size();
            if (fsize == 0)
            {
                break;
            }
        }
        _pageLib.push_back(WebPage(file_content, conf, _jieba));
    }
}
void PageLibPreprocessor::cutRedundantPages()
{
    //网页去重，找到重复的网页，在文档表和索引表去除相应的表项
}
void PageLibPreprocessor::buildInvertIndexTable()
{
    int N = _pageLib.size();
    map<int, int> idToIndex;
    unordered_map<string, vector<int>> strToId;
    for (int i = 0; i < _pageLib.size(); i++)
    {
        idToIndex.insert({_pageLib[i].getDocId(), i});
    }
}
void PageLibPreprocessor::storeOnDisk()
{
}