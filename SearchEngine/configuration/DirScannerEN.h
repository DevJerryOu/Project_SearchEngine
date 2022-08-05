#ifndef __DirScannerEN_H__
#define __DirScannerEN_H__


#include "tamporary.h"


//class DictProdeucer;
class DirScanner
{
//friend class DictProducer;
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