#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "TcpServer.h"
#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>
#include <boost/json/src.hpp>

using std::cout;
using std::endl;


class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con)
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
    string _msg;
    TcpConnectionPtr _con;
};

class EchoServer
{
public:
    EchoServer(size_t threadNum, size_t queSize
               , const string &ip, unsigned short port)
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
        string msg =  con->receive();
        cout << "recv from client msg : " << msg << endl;
        //msg是应该做处理的,就是业务逻辑的处理
        //将msg的处理交给线程池
        MyTask task(msg, con);
        _pool.addTask(std::bind(&MyTask::process, task));
    }

    std::string getCandidateWordsList(const string& keyWords)
    {
        // 填写逻辑
    }

    void moduleOne_KeyWordsRecommend(const TcpConnectionPtr &con)
    {
        // 模块一 关键词推荐
        string msg =  con->receive();//从client接收关键词
        cout << "recv from client msg : " << msg << endl;
        // 从json格式的值中获取key_word
        boost::json::value val1;
        boost::json::object val1_object;
        val1 = boost::json::parse(msg);
        val1_object = val1.get_object();
        std::cout << val1_object["key_words"] << std::endl;
        
        // std::string keyWords = val1.get_string();
        // ！！！如何通过字段名拿到字段值string？
        auto keyWords = val1_object["key_words"];
        // 拿到关键词 开始执行查询
        auto candidateWordsList = getCandidateWordsList(keyWords);
        

        
        //msg是应该做处理的,就是业务逻辑的处理
        //将msg的处理交给线程池
        MyTask task(msg, con);
        _pool.addTask(std::bind(&MyTask::process, task));
    }
    
    void onMessage(const TcpConnectionPtr &con)
    {
        #if 0
        //实现了回显服务
        string msg =  con->receive();
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
