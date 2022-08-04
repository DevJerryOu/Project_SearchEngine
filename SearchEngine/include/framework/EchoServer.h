#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "TcpServer.h"
#include "ThreadPool.h"
#include "MyLog.h"
#include "WebTask.h"
#include "QueryTask.h"
#include "Configuration.hpp"
#include <iostream>
#include <unistd.h>
#include <boost/json/src.hpp>
#include <set>
#include <vector>

using std::cout;
using std::endl;
using namespace boost::json;
using namespace std::literals; // string_view

typedef enum _taskType_s
{
    _KeyWordsRecommend,
    _ArticleSearch
} TaskType;

namespace ModuleOne
{
    struct KeyWordsObj
    {
        std::string keyWords;
    };

    // This helper function deduces the type and assigns the value with the matching key
    template <class T>
    void extract(object const &obj, T &t, boost::core::string_view key)
    {
        t = value_to<T>(obj.at(key));
    }

    KeyWordsObj tag_invoke(value_to_tag<KeyWordsObj>, value const &jv)
    {
        KeyWordsObj c;
        object const &obj = jv.as_object();
        extract(obj, c.keyWords, "key_words");
        // extract( obj, c.name, "name" );
        // extract( obj, c.current, "current" );
        return c;
    }
}

class EchoServer
{
public:
    EchoServer(size_t threadNum, size_t queSize, const std::string &ip, unsigned short port)
        : _pool(threadNum, queSize), _server(ip, port)
    {
    }

    void start()
    {
        _pool.start();
        // void(const TcpConnectionPtr &)
        // TcpConnectionPtr con;
        // void setAllCallback(TcpConnectionCallback &&onConnection
        using namespace std::placeholders;

        // bind可以改变函数的形态，函数的返回类型是改不了的
        //只能改变函数参数的个数的，也不能改变函数参数的类型
        //只能减少绑定的函数的参数个数，但是不能增加绑定函数的参数个数
        // bind绑定函数之后，bind的返回类型肯定是一个右值
        //
        // bind默认情况是进行值传递,如果想用引用传递,std::cref/std::ref,
        //想进行地址传递, 要对变量进行取地址
        _server.setAllCallback(std::bind(&EchoServer::onConnection, this, _1),
                               std::bind(&EchoServer::onMessage, this, _1), std::bind(&EchoServer::onClose, this, _1));
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

    void onMessage(const TcpConnectionPtr &con)
    {
        //实现了回显服务
        std::string msg = con->receive();
        cout << "recv from client msg : " << msg << endl;
        // msg是应该做处理的,就是业务逻辑的处理
        //将msg的处理交给线程池

        boost::json::value val1;
        boost::json::object val1_object;
        val1 = boost::json::parse(msg);
        val1_object = val1.get_object();

        std::string fieldValue;
        std::string fieldName2Get = "task_type";
        ModuleOne::extract(val1_object, fieldValue, fieldName2Get); // fieldValue是传入传出参数，此时任务类型就存在这里

        if (fieldValue == "key_word_recommend")
        {
            //创建一个WordTask
            std::string value_word = "key_words";
            std::string key_word;
            ModuleOne::extract(val1_object, key_word, value_word);

            QueryTask queryTask(key_word, con);
            _pool.addTask(std::bind(&QueryTask::process, queryTask));
        }
        else if (fieldValue == "article_search")
        {
            std::string value_word = "confirm_word";
            std::string key_word;
            ModuleOne::extract(val1_object, key_word, value_word);

            WebTask webTask(key_word, con);
            _pool.addTask(std::bind(&WebTask::process, webTask));
        }
        Configuration *conf = Configuration::getInstance();
        MyLog *myLog = MyLog::getInstance();
        myLog->writeInLog(*conf, msg);
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
