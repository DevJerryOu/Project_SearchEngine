// #include "SocketIO.h"
#include "../../include/framework/SocketIO.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
using std::cout;
using std::endl;

SocketIO::SocketIO(int fd)
    : _fd(fd)
{
}

SocketIO::~SocketIO()
{
}
int SocketIO::readn(char *buf, int len)
{
    int left = len;
    char *pstr = buf;
    int ret = 0;

    while (left > 0)
    {
        ret = read(_fd, pstr, left);
        if (-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if (-1 == ret)
        {
            perror("read error -1");
            return len - ret;
        }
        else if (0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }

    return len - left;
}

int SocketIO::readLine(char *buf, int len)
{
    int left = len - 1;
    char *pstr = buf;
    int ret = 0, total = 0;

    while (left > 0)
    {
        // MSG_PEEK不会将缓冲区中的数据进行清空,只会进行拷贝操作
        ret = recv(_fd, pstr, left, MSG_PEEK);
        // cout << "socketIO ret=" << ret << endl;
        if (-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if (-1 == ret)
        {
            perror("readLine error -1");
            return len - ret;
        }
        else if (0 == ret)
        {
            break;
        }
        else
        {
            for (int idx = 0; idx < ret; ++idx)
            {
                if (pstr[idx] == '\n')
                {
                    int sz = idx + 1;
                    readn(pstr, sz);
                    pstr += sz;
                    *pstr = '\0';

                    return total + sz;
                }
            }

            readn(pstr, ret); //从内核态拷贝到用户态
            total += ret;
            pstr += ret;
            left -= ret;
        }
    }
    *pstr = '\0';

    return total - left;
}

int SocketIO::readlen(void *buf, int len)
{
    int ret = recv(_fd, buf, len, 0);
    return ret;
}

int SocketIO::sendlen(void *buf, int len)
{
    int ret = send(_fd, buf, len, 0);
    return ret;
}

int SocketIO::writen(const char *buf, int len)
{
    cout << "SocketIO writen" << endl;
    int left = len;
    const char *pstr = buf;
    int ret = 0;

    while (left > 0)
    {
        ret = write(_fd, pstr, left);
        if (-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if (-1 == ret)
        {
            perror("writen error -1");
            return len - ret;
        }
        else if (0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;
}
