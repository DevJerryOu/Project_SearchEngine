#include "../../include/framework/Thread.h"
#include <stdio.h>

//回调函数_cb在此处注册
Thread::Thread(ThreadCallback &&cb)
: _thid(0)
, _isRunning(false)
, _cb(std::move(cb))//注册
{

}

Thread::~Thread()
{
    if(_isRunning)
    {
        //让子线程与主线程分离，子线程自己回收自己的资源
        pthread_detach(_thid);
    }
}

//this
void Thread::start()
{
    //threadFunc要设置为静态函数，原因是pthread_create函数第三个参数
    //是一个函数指针，该指针的类型已经被严格固定了，而如果将threadFunc
    //设计为成员函数的时候，会多一个this，所以需要设置为静态的
    //this  --- > pth  ---- > new MyThread
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret)
    {
        perror("pthread_create");
        return;
    }

    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
    {
        //shift + k可以进行系统函数的跳转
        int ret = pthread_join(_thid, nullptr);
        if(ret)
        {
            perror("pthread_join");
            return;
        }

        _isRunning = false;
    }
}

//threadFunc是一个静态的 
void *Thread::threadFunc(void *arg)
{
    //this如何传进来
    Thread *pth = static_cast<Thread *>(arg);
    if(pth)
    {
        //执行操作  run
        pth->_cb();//执行对应的任务,执行回调函数
    }


    /* return nullptr; */
    pthread_exit(nullptr);
}

