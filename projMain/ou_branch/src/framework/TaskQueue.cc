//#include "TaskQueue.h"
#include "../../include/framework/TaskQueue.h"


TaskQueue::TaskQueue(size_t sz)
: _queSize(sz)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _flag(true)
{

}

TaskQueue::~TaskQueue()
{

}

bool TaskQueue::empty() const
{
    return 0 == _que.size();
}

bool TaskQueue::full() const
{
    return _que.size() == _queSize;
}

void TaskQueue::push(ElemType &&value)
{
    MutexLockGuard autoLock(_mutex);//栈对象autoLock

    while(full())
    {
        //等待在满的线程，需要进行wait
        _notFull.wait();
    }

    //只有queue不为满的时候，才可以去进行push数据
    _que.push(std::move(value));

    //此时可以唤醒等待在条件变量为空的线程
    _notEmpty.notify();
}

ElemType TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);//栈对象autoLock

    //_flag标志位
    while(_flag && empty())
    {
        _notEmpty.wait();//卡主的
    }

    if(_flag)
    {
        ElemType tmp = _que.front();
        _que.pop();

        _notFull.notify();
        return tmp;

    }
    else
    {
        return nullptr;
    }


}
void TaskQueue::wakeup()
{
    _flag = false;
    _notEmpty.notifyAll();
}

