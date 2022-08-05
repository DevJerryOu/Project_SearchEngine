#ifndef __WebTask_H__
#define __WebTask_H__
#include "../websearch/WebPageSearcher.hpp"
#include "../websearch/Configuration.hpp"
#include <boost/json/src.hpp>
class WebTask
{
public:
    WebTask(const std::string &msg, const TcpConnectionPtr &con)
        : _msg(msg), _con(con)
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
        Configuration *conf = Configuration::getInstance();
        WebPageSearcher seach(_msg, *conf, atoi((*conf)["pageNum"].c_str()));
        vector<std::string> jason_string = seach.doQuery();
        boost::json::object inputKeyWordsObj;
        if (jason_string.size() == 2)
        {
            inputKeyWordsObj["article_title"] = jason_string[0];
            inputKeyWordsObj["article_abstract"] = jason_string[1];
        }
        else
        {
            inputKeyWordsObj["article_title"] = "ERROR";
            inputKeyWordsObj["article_abstract"] = "title not exist";
        }

        // JSON对象序列化
        std::string cwl2Send = serialize(inputKeyWordsObj);

        _con->sendInLoop(cwl2Send);

        //消息要从TcpConnection对象发送给EventLoop进行发送
        //此时TcpConnection必须要知道EventLoop的存在(在TcpConnection中添加
        // EventLoop的引用或者指针)
        //
    }

private:
    std::string _msg;
    TcpConnectionPtr _con;
};

#endif
