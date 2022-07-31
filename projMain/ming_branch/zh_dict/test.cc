#include "splitTool.h"
#include "title.h"
#include "libgen.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

void test()
{
    /* Cppjieba wordseg; */
    /* vector<string> str = wordseg.cut(); */
    
    /* int idx = 0; */
    /* for(auto it = str[idx].begin();it != str[idx].end(); ++it) */
    /* { */
    /*     cout<<*it<<" "; */
    /*     idx++; */
    /* } */
    /* cout<<endl; */

    /* char *path = "art"; */
    /* char *str1 = dirname(path); //art所在目录 */
    /* cout<<str1<<endl; */ 
    /* char *str2 = basename(path); //获取art文件名 */
    /* cout<<str2<<endl; */
    

    //char *str = basename(path);
    /* for(int idx = 0;; ++idx) */
    /* { */
    /*     cout<<str[idx]<<endl; */
    /* } */


    struct dirent * pdirent = 0;
    DIR * pdir = opendir("art");

    while((pdirent = readdir(pdir)) != 0)
    {
        if(strcpy(pdirent->d_name,".")!=0 && strcpy(pdirent->d_name,"..")!=0)
        {
            
        }

        if((pdirent->d_name[0] == '.') && (pdirent->d_name[1] == '..'))
        {     continue; }
        else{
            //问题1 得到文件名 -> 获取文件内容
            //问题2 文件内容 -> 传进cutall分词 -> 返回分词结果q
            
            //打开文件
            FILE *fp = fopen(pdirent->d_name[2],"rw+");
            if(fp == nullptr)
            {
                cout<<"fail to open file"<<endl;
                exit(0);
            }
            //获取文件内容
            char *line = nullptr;
            size_t len = 0;
            ssize_t read;
            while((read = getline(&line,&len,fp)) != -1)
            {
                cout<<"read = "<<read<<endl;
                cout<<"line = "<<line<<endl;
            }
        }
    }
}

int main()
{
    test();
    return 0;
}
