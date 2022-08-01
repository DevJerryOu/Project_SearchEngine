#if 1
#include "RedisPPCache.h"

RedisPPCache::RedisPPCache(Redis&& redisInstance,std::string str)
:_redisInstance(std::move(redisInstance)),
 _hashTableName(str)
{

}
// create cache
void RedisPPCache::addElement(
const std::string& key, 
const std::string& value )
{
    cout << "hello!" << endl;
    // *redisInstance.hset(tableName,key,value);

    auto result = _redisInstance.hset(this->_hashTableName,key,value);
    if(result){
        cout << "addElement successful:key = " << key << ',' << " value = " << value << endl;
    }
    else {
        cout << "addElement failed!" << endl;

    }

}

// retrieve cache
std::string RedisPPCache::getElement( const string& key ){
    auto str = _redisInstance.hget(this->_hashTableName,key);
    return str;
}

// // ulter(update) cache 
// void updateElement( const string& key , const string& set_value ){ // 更新value里面的数据

// }

// delete cache
void RedisPPCache::delElement(const Redis& redisInstance,const string& key){
    cout << "hello!" << endl;
}

#endif