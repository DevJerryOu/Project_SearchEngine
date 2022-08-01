#if 1
#ifndef __REDISPP_CACHE_H__
#define __REDISPP_CACHE_H__

#define _GLIBCXX_USE_CXX11_ABI 0

//#include "VirtualCache.h"
#include <sw/redis++/redis++.h>
#include <iostream>
#include <string>
#include <set>


using namespace std;
using namespace sw::redis;

// 这里采用Redis++来做缓存，就不用自己设计数据成员了，因为所有的数据都在Redis服务器那里存着
// 自己写一些CRUD的函数就搞定了
// 应该采用哈希表结构来存储。数据结构为key
// 模块一的在线部分，如果启用了缓存服务，那查询时会传进来一个string 比如 hellw，(这是key)，然后我就返回一个value（关键词候选列表）
// 里面包含{ "hello kitty",
//          "hello world",
//          "hello 树先生",
//          "hello venus",
//          "hello baby"  } 
class RedisPPCache
//:public VirtualCache
{
public:

    RedisPPCache(Redis&&,std::string);

    // create cache
    void addElement(const std::string& , const std::string& ) ;
    // retrieve cache
    std::string getElement(const std::string& );
    // // ulter(update) cache 
    // void updateElement( const std::string& , const std::string );
    // delete cache
    void delElement(const Redis& redisInstance,const std::string&) ;

private:
    Redis&& _redisInstance;
    std::string _hashTableName;

};






#endif
#endif