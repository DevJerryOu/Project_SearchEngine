#include "getfilename.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void GetFilename::getFile() //获取文件路径
{
    DIR * dir = opendir("./art");
    struct dirent *pdir;
    while((pdir = readdir(dir)) != nullptr)
    {
        string filename(pdir->d_name);
        if(("." == filename) || (".." == filename))
            continue;
        //cout<<"filename = "<<filename<<endl;
        string s = string("./art") + string("/") + filename;
        //cout<<"s = "<<s<<endl;
        _files.push_back(s);
    }
    closedir(dir);
}

void GetFilename::openfile() //根据文件路径打开文件
{
    vector<string> sentence;
    for(auto & it : _files)
    {
        ifstream ifs(it,ifstream::in);
        if(!ifs.is_open())
        {
            cout<<"Failed to open file!"<<endl;  
            exit(0);
        }
            
        string lines;
        string first,second;
        while(getline(ifs,lines))
        {
            sentence = cut(lines);
            for(auto & it : sentence)
            {
                auto exit = _dIndex.find(it);
                if(exit != _dIndex.end())
                {
                    ++(_dict[exit->second].second);
                }
                else
                {
                    _dict.push_back(pair<string,int> (it,1));
                    _dIndex.insert(pair<string,int>(it,_dict.size()));
                }
            }
        }
        ifs.close();
    }
}

#if 1
int main()
{
    GetFilename word;
    //word.openfile();
    return 0;
}
#endif
