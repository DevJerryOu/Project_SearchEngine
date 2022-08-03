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

typedef enum _taskType_s{
    _KeyWordsRecommend,
    _ArticleSearch
}TaskType;

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

    std::set<std::string> getCandidateWordsList(const std::string& keyWord)
    {
        // 填写逻辑
    }

    std::vector<std::string> getArticle(const std::string& confirmWord){
        // 填写逻辑
    }

    std::string moduleOne_KeyWordsRecommend(const std::string& msg)
    {
        // // 模块一 关键词推荐
        // std::string msg =  con->receive();//从client接收关键词 JSON格式
        // cout << "recv from client msg : " << msg << endl;
        // 从json格式的字符串中获取key_word
        boost::json::value val1;
        boost::json::object val1_object;
        val1 = boost::json::parse(msg);
        val1_object = val1.get_object();

        std::string fieldValue;
        std::string fieldName2Get = "key_words";
        ModuleOne::extract(val1_object,fieldValue,fieldName2Get);

        //通过关键词获取到关键词列表 
        std::set<std::string> candidateWordsList = getCandidateWordsList(fieldValue);

        boost::json::object candidateWordsListJsonObj;
        std::set<std::string>::iterator cdwlIterator = candidateWordsList.begin();
        
        // 给json对象赋值
        candidateWordsListJsonObj["candidate_word1"] = (*cdwlIterator);
        cdwlIterator++;
        candidateWordsListJsonObj["candidate_word2"] = (*cdwlIterator);
        cdwlIterator++;
        candidateWordsListJsonObj["candidate_word3"] = (*cdwlIterator);

        // JSON对象序列化，将候选词列表发送给client
        std::string cwl2Send = serialize(candidateWordsListJsonObj);

        return cwl2Send;
        
    }

    std::string moduleTwo_ArticleSearch(const std::string& msg)
    {
        // // 模块二 文章搜索
        boost::json::value val1;
        boost::json::object val1_object;
        val1 = boost::json::parse(msg);
        val1_object = val1.get_object();
        
        std::string fieldValue;
        std::string fieldName2Get = "confirm_word";
        ModuleOne::extract(val1_object,fieldValue,fieldName2Get);

        //通过关键词获取到文章
        std::vector<std::string> article = getArticle(fieldValue);

        std::string articleName = article[0];
        std::string articleAbstract = article[1];

        boost::json::object articleJsonObj;

        // 给json对象赋值
        articleJsonObj["article_title"] = articleName;
        articleJsonObj["article_abstract"] = articleAbstract;

        // JSON对象序列化
        std::string cwl2Send = serialize(articleJsonObj);

        return cwl2Send;
    }

    TaskType parseTask(const std::string& task) // 输入json格式的字符串，提取出任务类型并返回
    {
          // 从json格式的字符串中获取key_word
        boost::json::value val1;
        boost::json::object val1_object;
        val1 = boost::json::parse(task);
        val1_object = val1.get_object();
       
        std::string fieldValue;
        std::string fieldName2Get = "task_type";
        ModuleOne::extract(val1_object,fieldValue,fieldName2Get); // fieldValue是传入传出参数，此时任务类型就存在这里

        if(fieldValue == "key_word_recommend")
        {
            return _KeyWordsRecommend;
        }
        else if(fieldValue == "article_search")
        {
            return _ArticleSearch;
        }
        // else 
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
       // echoService(con);

        // 解析任务 到底是走模块一还是走模块二
        std::string recvMsg =  con->receive(); //从client接收关键词 JSON格式
        int taskType = parseTask(recvMsg);

        std::string msg2Send;
        if(_KeyWordsRecommend == taskType){
            // 走模块一的逻辑
            msg2Send = moduleOne_KeyWordsRecommend(recvMsg);
        }

        else if(_ArticleSearch == taskType){
            // 走模块二的逻辑
            msg2Send = moduleTwo_ArticleSearch(recvMsg);
        }

        // 打包成task,交给线程池去发送
        MyTask task(msg2Send, con);
        _pool.addTask(std::bind(&MyTask::process, task));

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
