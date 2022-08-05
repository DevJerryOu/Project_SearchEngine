#include "../include/DirScannerCN.h"
#include <sys/types.h>
#include <dirent.h>

DirScanner::DirScanner(const string &dir)
: _dir(dir)
{
    getFiles();
}

DirScanner::~DirScanner() {}

vector<string> &DirScanner::getFilePath() //返回所有文件的路径
{
    return _files;
}
    
void DirScanner::getFiles() //获取目录下的所有文件
{
    const char* str = _dir.c_str(); //_dir is string, need to trans !
    DIR * dir = opendir(str);
    struct dirent *pdir;
    while((pdir = readdir(dir)) != nullptr)
    {
        string filename(pdir->d_name);
        if(("." == filename) || (".." == filename))
            continue;
        string s = _dir + string("/") + filename;
        _files.push_back(s);
    }
    closedir(dir);
}