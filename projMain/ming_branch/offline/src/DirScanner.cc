#include "../include/DirScanner.h"
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
    DIR * dir = opendir(_dir);
    struct dirent *pdir;
    while((pdir = readdir(dir)) != nullptr)
    {
        string filename(pdir->d_name);
        if(("." == filename) || (".." == filename))
            continue;
        //cout<<"filename = "<<filename<<endl;
        string s = _dir + string("/") + filename;
        //cout<<"s = "<<s<<endl;
        _files.push_back(s);
    }
    closedir(dir);
}
