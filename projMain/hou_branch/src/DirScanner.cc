#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;


class DirScanner
{
public:
    DirScanner()
    { cout << "DirScanner()" << endl; }

    ~DirScanner()
    { cout << "~DirScanner()" << endl; }

    vector<string> files() 
    {
        vector<string>::iterator it;
        for(it = _files.begin(); it != _files.end(); it++)
        {
            cout << *it << endl;
        }
    }

    void traverse(const string & dirname);

private:
    vector<string> _files;
};

void DirScanner::traverse(const string & dirname)
{
    DIR * pDir;
    struct dirent * entry;
    struct stat statbuf;

    if((pDir = opendir(dirname.c_str())) == NULL)
    {
        printf("There are no dir");
        exit(1);
    }
    while((entry = readdir(pDir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
            _files.push_back(entry->d_name);
        }
    }
    closedir(pDir);
}

int main()
{
    DirScanner ds;
    ds.traverse("/home/scrooge/CPP44/Linux");
    ds.files();

    return 0;
}