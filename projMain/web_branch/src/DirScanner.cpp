#include "DirScanner.hpp"
DirScanner::DirScanner()
{
}
vector<string> DirScanner::files()
{
    return _files;
}
void DirScanner::traverse(const string &dirname)
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
                _files.push_back(string(pdir->d_name));
            }
        }
    }
    closedir(pdir_info);
}