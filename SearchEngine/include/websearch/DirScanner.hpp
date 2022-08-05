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
    vector<std::string> files();
    void traverse(const std::string &dirname);

private:
    vector<std::string> _files;
};
