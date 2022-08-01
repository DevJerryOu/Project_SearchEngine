#if 0
#ifndef __VIRTUAL_CACHE_H__
#define __VIRTUAL_CACHE_H__

#include <string>
//#include <string.h>
#include <set>

using namespace std; // 可能是个坑

class VirtualCache{
public:
    // create cache
    virtual void addElement( const std::string& , const set<std::string> ) = 0;
    // retrieve cache
    virtual set<std::string> getElement( const std::string& ) = 0;
    // // ulter(update) cache 
    // void updateElement( const string& , const string );
    // delete cache
    virtual void delElement(const std::string&) = 0;

};

#endif
#endif