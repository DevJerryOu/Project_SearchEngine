#include "../../include/framework/ThreadPool.h"
#include <unistd.h>

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(_queSize)
, _isExit(false)
{
    //预留空间
    _threads.reserve(_threadNum);
}

ThreadPool::~ThreadPool()
{
    if(!_isExit)
    {
        stop();
        _isExit = true;
    }
}

void ThreadPool::start()
{
    //创建工作线程并存储到vector
    for(size_t idx = 0; idx < _threadNum; ++idx)
    {
        //unique_ptr不允许复制或者赋值,所以传递右值
        unique_ptr<Thread> up(new Thread(bind(&ThreadPool::threadFunc, this)));
        _threads.push_back(std::move(up));
    }

    //运行工作线程
    for(auto &th : _threads)
    {
        th->start();//运行工作线程
    }
}

void ThreadPool::stop()
{
    //任务队列中只要有数据，就一定要执行完毕，不能让子线程退出
    while(!_taskQue.empty())
    {
        sleep(1);
    }

    _isExit = true;//置标志位
    _taskQue.wakeup();//所有的工作线程都唤醒

    for(auto &th : _threads)
    {
        th->join();//工作线程的退出
    }
}

void ThreadPool::addTask(Task &&task)
{
    if(task)
    {
        _taskQue.push(std::move(task));
    }
}

Task ThreadPool::getTask()
{
    return _taskQue.pop();
}

//交给工作线程执行的具体任务
void ThreadPool::threadFunc()
{
    while(!_isExit)
    {
        //如果getTask执行的非常慢，那么在进入while判断时，
        //_isExit已经为true，就不会再去取任务，但是如果
        //getTask执行非常快，那么_isExit来不及设置为true
        //就会卡死
        Task taskcb = getTask();
        if(taskcb)
        {
            taskcb();//执行的具体任务
        }
    }
}
