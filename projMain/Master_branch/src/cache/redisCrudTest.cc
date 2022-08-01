#if 0
//#define _GLIBCXX_USE_CXX11_ABI 0

#include "../../include/cache/RedisPPCache.h"
//#include "RedisPPCache.h"

#include <initializer_list>
#include <set>
#include <string>
#include <boost/json/src.hpp>

using namespace boost::json;

// 8.1上午 任务是把<string,set<string>>写到redis中
// set<string>得用initializer_list<string>代替
using std::cout;

// ostream& operator<<(ostream&,set<string> st){
//     cout << "set:{";
//     for(auto &it :st){
//         cout << it << ' ';
//     }
//     cout << '}';
// }

// template<typename T>
// ostream& operator<<(ostream&,initializer_list<T> il){
//     cout << "initializer_list:{";
//     for(auto &it : il){
//         cout << *it << ' ';
//     }
//     cout << '}';
// }

// struct CandidateWordsList{
//     string key;
//     set<string> value;
// };


int main(){
    auto redis = Redis("tcp://127.0.0.1:6379");
    #if 0
    std::unordered_map<std::string, std::string> m = { // 模板参数只能是string string类型
            {"field3", "val4"},
            {"field4", "val4"}
    };
    redis.hmset("hash", m.begin(), m.end());
    m.clear();
    redis.hgetall("hash", std::inserter(m, m.begin()));
    #endif

    // if(res){
    //     cout << *res << endl;
    // }

    // for(auto& it : m){
    //     cout << "Key:" << it.first << ',' 
    //          << "Value:" << it.second << endl;
    // }

    //redis.hmset()
    
    
    //set<string> candidateWordsList1 = {"Hello 树先生","Hello Kitty","Hello World"};
    //set<string> candidateWordsList2 = {"王道考研","王道征途","王道霸道"};
    
    #if 0
    std::unordered_map< std::string , std::set<string> > m = {
            {"key1",candidateWordsList1},
            {"key2", candidateWordsList2}
    };
    
    redis.hmset("hash", m.begin(), m.end());
    m.clear();
    redis.hgetall("hash", std::inserter(m, m.begin()));

    for(auto& it : m){
        cout << "Key:" << it.first << ',' 
             << "Value:" << it.second << endl;
    }

     
    #endif

//    CandidateWordsList cwl1;
//    cwl1.key = "testcwl1";
//     set<string> set1 = {
//                 "hello motor",
//                 "hello 书线索",
//                 "hello teacher!"};
    std::string candidateWordList = {
        "hello motor,hello 书线索,hello teacher!"
    };

    
    // cwl1.value.insert("hello 树先生","hello motor");

    // auto val = redis.hset("hash","hello",candidateWordList);
    // //auto val = redis.hset(cwl1.key,cwl1.value);
    // if(val){
    //     cout << "set successful!" << endl;
    // }
    // auto result = redis.hget("hash","hello");
    // for(auto& it: result){
    //     cout << *it << endl;
    // }
    
    // 序列化
    boost::json::object val;
    val["a_string"] = "test_string";
    val["a_number"] = 123;
    val["a_null"] = nullptr;
    val["a_array"] = {
        1, "2", boost::json::object({{"123", "123"}})
    };
    val["a_object"].emplace_object()["a_name"] = "a_data";
    val["a_bool"] = true;

    std::string str = serialize(val);

    cout << "str = " << str << std::endl;
    auto tmp1 = redis.hset("hash","hello",str);
    if(tmp1){
        cout << "hset successful!" << std::endl;
    }

    //cout << val["a_bool"] << endl;

    boost::json::value val1;
    boost::json::object val1_object;
    val1 = parse(str);
    val1_object = val1.get_object();
    cout << val1_object["a_number"] << std::endl;
    std::string testKey = "hello redispp";  
    RedisPPCache rpc(std::move(redis),"hash");
    rpc.addElement(testKey,str);

    std::optional<std::string> resultTmp1 = rpc.getElement(testKey);
    cout << "resultTmp1=" << *resultTmp1 << std::endl;

    rpc.delElement("field1");
    
    // auto res1 = redis.command<OptionalString>("hgetall","hash");
    // cout << "res1:" << res1 << endl;

    return 0;
}
#endif