#ifndef __DirScanner_H__
#define __DirScanner_H__


#include <vector>
#include <string>

using std::vector;
using std::string;

class DirScanner
{
public:
    DirScanner(const string &dir);
    ~DirScanner();

    vector<string>& getFilePath(); //返回所有文件的路径
    
private:
    void getFiles(); //获取目录下的所有文件

private:
    string _dir;
    vector<string> _files;
};


#endif
