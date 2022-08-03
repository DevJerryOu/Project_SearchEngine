//#define _GLIBCXX_USE_CXX11_ABI 0
#include "RedisPPCache.h"
#include <initializer_list>
#include <set>
#include <string>
#include <boost/json/src.hpp>
#include <string_view>

using namespace boost::json;
using namespace std::literals; // string_view

namespace ModuleOne{
    struct KeyWordsObj{
        std::string keyWords;
    };

    #if 0
    void tag_invoke( value_from_tag, value& jv, KeyWordsObj const& kwobj )
    {
        jv = {
            { "key_words" , kwobj.keyWords }
        };
    }
    #endif

    // This helper function deduces the type and assigns the value with the matching key
    template<class T>
    void extract( object const& obj, T& t, boost::core::string_view key )
    {
        t = value_to<T>( obj.at( key ) );
    }

    KeyWordsObj tag_invoke( value_to_tag< KeyWordsObj >, value const& jv )
    {
        KeyWordsObj c;
        object const& obj = jv.as_object();
        extract( obj, c.keyWords, "key_words" );
        // extract( obj, c.name, "name" );
        // extract( obj, c.current, "current" );
        return c;
    }

}

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

    // 反序列化
    boost::json::value val1;
    boost::json::object val1_object;
    val1 = parse(str);
    val1_object = val1.get_object();
    // 可以直接用 << 运算符将字段值输出到终端，但是不可以直接通过字段名获取字段值
    cout << "va11_object[a_object] = " << val1_object["a_object"] << std::endl;
    cout << "va11_object[a_number] = " << val1_object["a_number"] << std::endl;
    cout << "val1_object[a_string] = " << val1_object["a_string"] << endl;

    // 将string反序列化成json对象后，因为value不可以直接用[]通过字段名取字段值取，
    // 所以将value转换成自定义的对象，再取keywords
   // ModuleOne::KeyWordsObj kwObj;
    //kwObj = ModuleOne::tag_invoke(kwObj , val1);
    std::string resStr;
    std::string testStr = "a_string";
    boost::core::string_view testStrSv(testStr);
    ModuleOne::extract(val1_object,resStr,testStr);

    cout << "ModuleOne::extract() = " << resStr << endl;
    
    
    
    // 测试向redis++插入数据
    std::string testKey = "hello redispp";  
    RedisPPCache rpc(std::move(redis),"hash");
    rpc.addElement(testKey,str);
    std::optional<std::string> resultTmp1 = rpc.getElement(testKey);
    cout << "resultTmp1=" << *resultTmp1 << std::endl;
    rpc.delElement("field1");
    
    // auto res1 = redis.command<OptionalString>("hgetall","hash");
    // cout << "res1:" << res1 << endl;

    // 增加多态特性 ，如果要自己写一个LRUCache，可以拓展
    VirtualCache *vc;
    RedisPPCache rpc1(std::move(redis),"hash1");
    vc = &rpc1;
    vc->addElement(testKey,str);


    return 0;
}