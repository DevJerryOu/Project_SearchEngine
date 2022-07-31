#include "splitTool.h"

Cppjieba::Cppjieba()
: _jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
, _files()
{}

Cppjieba::~Cppjieba() {}

void Cppjieba::getFiles() //获取文件路径
{
    struct dirent * pdirent = 0;
    DIR * pdir = opendir("art");

    while((pdirent = readdir(pdir)) != 0)
    {
         if(strcpy(pdirent->d_name,".")!=0 && strcpy(pdirent->d_name,"..")!=0)
         {
             //cout<<"pdirent->d_name"<<pdirent->d_name<<endl;
            _files.push_back(pdirent->d_name);
         }
    }
    closedir(pdir);
}

vector<string> Cppjieba::cut(const string &str) 
{

    for(auto it = str.begin();it != str.end(); ++it)
    {
        cout<<"it = "<<*it<<endl;
        /* ifstream ifs(*it); */
        /* if(!ifs.is_open()) */
        /* { */
        /*     cerr<<"Faild to open file!\n"; */
        /* } */
        /* FILE* fp = fopen(*it,"r+"); */
        /* if(!fp) */
        /* { */
        /*     cout<<"Failed to open file!"<<endl; */
        /*     exit(0); */
        /* } */
    }
 
}

void test()
{
#if 1
    SplitTool wordseg;
    string *str = wordseg.getF();
    vector<string> str2 = wordseg.cut(str);
    
    for(auto it = str2.begin();it != str2.end(); ++it)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
#endif

}

int main()
{
    test();
    return 0;
}


