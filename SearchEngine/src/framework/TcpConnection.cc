#include "../../include/framework/TcpConnection.h"
#include "../../include/framework/EventLoop.h"
//#include "../../include/framework/Acceptor.h"
#include "MyLog.h"
#include "Configuration.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
using std::cout;
using std::endl;
using std::memcpy;
using std::ostringstream;

// typedef struct
// {
//     int length;
//     std::string content;
// } Train;

TcpConnection::TcpConnection(int fd, EventLoop *loop)
    : _loop(loop), _sock(fd), _sockIO(fd), _localAddr(getLocalAddr()), _peerAddr(getPeerAddr())

{
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::send(const std::string &msg)
{
    int len = msg.size();
    _sockIO.sendlen(&len, 4);
    _sockIO.writen(msg.c_str(), msg.size());
    cout << "send msg:" << msg << endl;
    Configuration *conf = Configuration::getInstance();
    MyLog *myLog = MyLog::getInstance();
    myLog->writeInLog(*conf, msg);
}

void TcpConnection::sendInLoop(const std::string &msg)
{
    //消息msg需要从TcpConnection发送给EventLoop
    /* this->send(msg); */
    if (_loop)
    {
#if 0
        void TcpConnection::send(const std::string &msg)
        {
            _sockIO.writen(msg.c_str(), msg.size());
        }
#endif
        cout << "sendInLoop" << endl;
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg)); // 发送的操作就在上面
        // void runInLoop(function<void()>)
    }
}

std::string TcpConnection::receive()
{
    int len;
    int ret = _sockIO.readlen(&len, 4);
    char buff[65535] = {0};
    ret = _sockIO.readLine(buff, len + 1);
    return std::string(buff);
}

std::string TcpConnection::toString()
{
    ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();

    return oss.str();
}

bool TcpConnection::isClosed() const
{
    char buf[10] = {0};
    int ret = recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);

    return ret == 0;
}

void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb)
{
    _onConectionCb = cb;
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb)
{
    _onMessageCb = cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb)
{
    _onCloseCb = cb;
}

void TcpConnection::handleConnectionCallback()
{
    // this ->TcpConnection > (this - >shared_ptr)
    if (_onConectionCb)
    {
        /* _onConectionCb(shared_ptr<TcpConnection>(this)); */
        _onConectionCb(shared_from_this());
    }
}
void TcpConnection::handleMessageCallback()
{
    if (_onMessageCb)
    {
        _onMessageCb(shared_from_this());
    }
}
void TcpConnection::handleCloseCallback()
{
    if (_onCloseCb)
    {
        _onCloseCb(shared_from_this());
    }
}

//获取本端的网络地址信息
InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getsockname(_sock.fd(), (struct sockaddr *)&addr, &len);
    if (-1 == ret)
    {
        perror("getsockname");
    }

    return InetAddress(addr);
}

//获取对端的网络地址信息
InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
    if (-1 == ret)
    {
        perror("getpeername");
    }

    return InetAddress(addr);
}
