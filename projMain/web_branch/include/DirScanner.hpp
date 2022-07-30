#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <string.h>
using std::string;
using std::vector;
class DirScanner
{
public:
    DirScanner();
    vector<string> files();
    void traverse(const string &dirname);

private:
    vector<string> _files;
};