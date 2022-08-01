#if 1

#include "../../include/cache/RedisPPCache.h"
//#include "RedisPPCache.h"

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
    std::cout << "hello!" << std::endl;
    // *redisInstance.hset(tableName,key,value);

    auto result = _redisInstance.hset(this->_hashTableName,key,value);
    if(result){
        std::cout << "addElement successful:key = " << key << ',' << " value = " << value << std::endl;
    }
    else {
        std::cout << "addElement failed!" << std::endl;

    }

}

// retrieve cache
std::optional<std::string> RedisPPCache::getElement( const std::string& key ){
    auto str = _redisInstance.hget(this->_hashTableName,key);
    if(str){
        return str;
    }
    else 
        return std::nullopt;
}

// // ulter(update) cache 
// void updateElement( const string& key , const string& set_value ){ // 更新value里面的数据

// }

// delete cache
void RedisPPCache::delElement(const std::string& key){
    
    auto res = _redisInstance.hdel(_hashTableName,key);
    if(res){
        std::cout << "key = " << key << "has been removed!" << std::endl;
    }
}

#endif