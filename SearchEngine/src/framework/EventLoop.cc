// #include "EventLoop.h"
// #include "Acceptor.h"
#include "../../include/framework/EventLoop.h"
#include "../../include/framework/Acceptor.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <iostream>

using std::cout;
using std::endl;

EventLoop::EventLoop(Acceptor &acceptor)
    : _epfd(createEpollFd()), _evfd(createEventFd()), _acceptor(acceptor), _isLooping(false), _evtList(1024)
{
    addEpollReadFd(_acceptor.fd()); //监听listenfd
    addEpollReadFd(_evfd);          //将eventfd放在红黑树上进行监听
}

EventLoop::~EventLoop()
{
    if (_epfd)
    {
        close(_epfd);
    }

    if (_evfd)
    {
        close(_evfd);
    }
}

void EventLoop::loop()
{
    _isLooping = true;
    while (_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

void EventLoop::runInLoop(Functor &&cb)
{
    {
        MutexLockGuard autoLock(_mutex);
        _pendingsCb.push_back(std::move(cb));
    }
    wakeup();
}

void EventLoop::setConnectionCallback(TcpConnectionCallback &&cb)
{
    _onConectionCb = std::move(cb);
}
void EventLoop::setMessageCallback(TcpConnectionCallback &&cb)
{
    _onMessageCb = std::move(cb);
}
void EventLoop::setCloseCallback(TcpConnectionCallback &&cb)
{
    _onCloseCb = std::move(cb);
}
void EventLoop::waitEpollFd()
{
    int nready = -1;
    do
    {
        nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 3000);
    } while (-1 == nready && errno == EINTR);

    if (-1 == nready)
    {
        perror("epoll_wait nready == -1");
        return;
    }
    else if (0 == nready)
    {
        cout << ">>epoll_wait timeout" << endl;
    }
    else
    {
        //可以解决扩容的问题，连接请求超过1024
        if (nready == (int)_evtList.size())
        {
            _evtList.resize(2 * nready);
        }

        for (int idx = 0; idx < nready; ++idx)
        {
            int fd = _evtList[idx].data.fd;
            if (fd == _acceptor.fd()) //有新的连接
            {
                if (_evtList[idx].events & EPOLLIN) //并且是读事件
                {
                    handleNewConnection(); //处理新的连接请求
                }
            }
            else if (fd == _evfd) //向tcp发送数据
            {
                if (_evtList[idx].events & EPOLLIN)
                {
                    handleRead();
                    //执行所有的任务
                    doPendingFunctors();
                }
            }
            else //老的连接上有数据发过来了
            {
                if (_evtList[idx].events & EPOLLIN) //并且是读事件
                {
                    handleMessage(fd); //进行正常业务逻辑,进行数据的收发
                }
            }
        }
    }
}

void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    if (peerfd < 0)
    {
        perror("peerfd");
    }
    addEpollReadFd(peerfd);

    //创建Tcp连接
    TcpConnectionPtr con(new TcpConnection(peerfd, this));

    //注册三个事件
    con->setConnectionCallback(_onConectionCb);
    con->setMessageCallback(_onMessageCb);
    con->setCloseCallback(_onCloseCb);

    _conns.insert(std::make_pair(peerfd, con));
    //
    con->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    auto it = _conns.find(fd);
    if (it != _conns.end())
    {
        bool flag = it->second->isClosed();
        if (flag)
        {
            //处理连接断开
            it->second->handleCloseCallback();
            delEpollReadFd(fd);
            _conns.erase(it);
        }
        else
        {
            //处理消息到达的事件
            it->second->handleMessageCallback();
        }
    }
    else
    {
        cout << "该连接不存在" << endl;
    }
}

//执行所有的任务
void EventLoop::doPendingFunctors()
{
    vector<Functor> tmp;
    {
        //粒度小一些
        MutexLockGuard autoLock(_mutex);
        tmp.swap(_pendingsCb);
    }

    /* for(auto &cb : _pendingsCb) */
    for (auto &cb : tmp)
    {
        cb();
    }
}
void EventLoop::handleRead()
{
    uint64_t one = 1;
    int ret = read(_evfd, &one, sizeof(one));
    if (ret != sizeof(one))
    {
        perror("read");
        return;
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = write(_evfd, &one, sizeof(one));
    if (ret != sizeof(one))
    {
        perror("read");
        return;
    }
}
int EventLoop::createEventFd()
{
    int fd = eventfd(0, 0);
    if (fd < 0)
    {
        perror("eventfd");
        return -1;
    }

    return fd;
}

int EventLoop::createEpollFd()
{
    int fd = epoll_create1(0);
    if (fd < 0)
    {
        perror("epoll_create1");
    }
    return fd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if (ret < 0)
    {
        perror("epoll_ctl add");
        return;
    }
}

void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if (ret < 0)
    {
        perror("epoll_ctl del");
        return;
    }
}
