//#include "Condition.h"
//#include "MutexLock.h"
#include "../../include/framework/Condition.h"
#include "../../include/framework/MutexLock.h"

#include <stdio.h>

Condition::Condition(MutexLock &mutex)
: _mutex(mutex) 
{
    int ret = pthread_cond_init(&_cond, nullptr);
    if(ret)
    {
        perror("pthread_cond_init");
    }
}

Condition::~Condition()
{
    int ret = pthread_cond_destroy(&_cond);
    if(ret)
    {
        perror("pthread_cond_destroy");
    }
}
void Condition::wait()
{
    //上半部
    //下半部
    int ret = pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());
    if(ret)
    {
        perror("pthread_cond_wait");
    }
}

void Condition::notify()
{
    //signal会至少唤醒一个等待的线程
    int ret = pthread_cond_signal(&_cond);
    if(ret)
    {
        perror("pthread_cond_signal");
    }
}

void Condition::notifyAll()
{
    int ret = pthread_cond_broadcast(&_cond);
    if(ret)
    {
        perror("pthread_cond_broadcast");
    }
}
