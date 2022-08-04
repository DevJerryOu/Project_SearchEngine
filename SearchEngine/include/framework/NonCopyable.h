#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__


class NonCopyable
{
public:
protected:
/* private: */
    NonCopyable()
    {

    }
    ~NonCopyable() 
    {

    }

    NonCopyable(const NonCopyable &rhs) = delete;
    NonCopyable &operator=(const NonCopyable &rhs) = delete;

private:

};

#endif
