#include "DirScanner.hpp"
using std::cout;
using std::endl;
DirScanner::DirScanner()
{
}
vector<std::string> DirScanner::files()
{
    return _files;
}
void DirScanner::traverse(const std::string &dirname)
{
    DIR *pdir_info;
    struct dirent *pdir;
    pdir_info = opendir(dirname.c_str());
    if (pdir_info)
    {
        while ((pdir = readdir(pdir_info)) != NULL)
        {
            if (strcmp(pdir->d_name, "..") == 0 || strcmp(pdir->d_name, ".") == 0)
            {
                continue;
            }
            else
            {
                _files.push_back(std::string(pdir->d_name));
            }
        }
    }
    closedir(pdir_info);
}
