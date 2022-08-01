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
    virtual void addElement( const string& , const set<string> ) = 0;
    // retrieve cache
    virtual set<string> getElement( const string& ) = 0;
    // // ulter(update) cache 
    // void updateElement( const string& , const string );
    // delete cache
    virtual void delElement(const string&) = 0;

};

#endif
#endif