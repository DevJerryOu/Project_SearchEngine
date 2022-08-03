#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "TcpServer.h"
#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>
#include <boost/json/src.hpp>
#include <set>
#include <vector>

using std::cout;
using std::endl;

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


class MyTask
{
public:
    MyTask(const std::string &msg, const TcpConnectionPtr &con)
    : _msg(msg)
    , _con(con)
    {

    }
    void process()
    {
        //_msg
        //处理_msg,进行相应的编解码、计算
        //处理完成之后，消息是需要发出去的,需要一个TCP连接
        //消息的接收与发送此时都是Reactor/EventLoop
        //
        //涉及到计算线程（线程池）与IO线程（EventLoop/Reactor）进行通信
        _con->sendInLoop(_msg);
        //消息要从TcpConnection对象发送给EventLoop进行发送
        //此时TcpConnection必须要知道EventLoop的存在(在TcpConnection中添加
        //EventLoop的引用或者指针)
        //
    }

private:
    std::string _msg;
    TcpConnectionPtr _con;
};

class EchoServer
{
public:
    EchoServer(size_t threadNum, size_t queSize
               , const std::string& ip, unsigned short port)
    : _pool(threadNum, queSize)
    , _server(ip, port)
    {

    }

    void start()
    {
        _pool.start();
        //void(const TcpConnectionPtr &)
        //TcpConnectionPtr con;
        //void setAllCallback(TcpConnectionCallback &&onConnection
        using namespace std::placeholders;

        //bind可以改变函数的形态，函数的返回类型是改不了的
        //只能改变函数参数的个数的，也不能改变函数参数的类型
        //只能减少绑定的函数的参数个数，但是不能增加绑定函数的参数个数
        //bind绑定函数之后，bind的返回类型肯定是一个右值
        //
        //bind默认情况是进行值传递,如果想用引用传递,std::cref/std::ref,
        //想进行地址传递, 要对变量进行取地址
        _server.setAllCallback(std::bind(&EchoServer::onConnection, this, _1),
                               std::bind(&EchoServer::onMessage, this, _1)
                               , std::bind(&EchoServer::onClose, this, _1));
        _server.start();
    }

    void stop()
    {
        _pool.stop();
        _server.stop();
    }

    void onConnection(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has connected!" << endl;
    }

    void echoService(const TcpConnectionPtr &con)
    {
        //实现了回显服务
        std::string msg =  con->receive();
        cout << "recv from client msg : " << msg << endl;
        //msg是应该做处理的,就是业务逻辑的处理
        //将msg的处理交给线程池
        MyTask task(msg, con);
        _pool.addTask(std::bind(&MyTask::process, task));
    }

    std::vector<std::string> getCandidateWordsList(const std::string& keyWords)
    {
        // 填写逻辑
    }

    void moduleOne_KeyWordsRecommend(const TcpConnectionPtr &con)
    {
        // 模块一 关键词推荐
        std::string msg =  con->receive();//从client接收关键词 JSON格式
        cout << "recv from client msg : " << msg << endl;
        // 从json格式的值中获取key_word
        boost::json::value val1;
        boost::json::object val1_object;
        val1 = boost::json::parse(msg);
        val1_object = val1.get_object();

        // 可以直接用 << 运算符将字段值输出到终端，但是不可以直接通过字段名获取字段值
        std::cout << val1_object["key_words"] << std::endl;
        
        // std::string keyWords = val1.get_string();
        
        // // ！！！如何通过字段名拿到字段值string？
        // auto keyWords = val1_object["key_words"];
        // // 拿到关键词 开始执行查询
        // auto candidateWordsList = getCandidateWordsList(keyWords);
        
        std::string fieldValue;
        std::string fieldName2Get = "key_words";
        ModuleOne::extract(val1_object,fieldValue,fieldName2Get);

        //通过关键词获取到关键词列表 
        std::vector<std::string> candidateWordsList = getCandidateWordsList(fieldValue);

    // { "candidate_word1":"hallo",
    //   "candidate_word2":"hollo",
    //   "candidate_word1":"hello" }
        boost::json::object candidateWordsListJsonObj;
        candidateWordsListJsonObj["candidate_word1"] = candidateWordsList[0];
        candidateWordsListJsonObj["candidate_word2"] = candidateWordsList[1];
        candidateWordsListJsonObj["candidate_word3"] = candidateWordsList[2];

        // json对象序列化成string,准备发送给client
        std::string candidateWordsListStr = serialize(candidateWordsListJsonObj);

        // 发送关键词列表给client
        


        
        //candidateWordsListJsonValue["candidate_word1"]
        
        //msg是应该做处理的,就是业务逻辑的处理
        //将msg的处理交给线程池
        MyTask task(msg, con);
        _pool.addTask(std::bind(&MyTask::process, task));
    }
    
    void onMessage(const TcpConnectionPtr &con)
    {
        #if 0
        //实现了回显服务
        std::string msg =  con->receive();
        cout << "recv from client msg : " << msg << endl;
        //msg是应该做处理的,就是业务逻辑的处理
        //将msg的处理交给线程池
        MyTask task(msg, con);
        _pool.addTask(std::bind(&MyTask::process, task));
        #endif

        // echo服务
        echoService(con);


    }

    void onClose(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has closed!" << endl;
    }

public:
    ThreadPool _pool;
    TcpServer _server;
};

#endif
