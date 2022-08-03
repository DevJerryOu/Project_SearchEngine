#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__
#include <string>

class SocketIO
{
public:
    explicit SocketIO(int fd);
    ~SocketIO();
    int readn(char *buf, int len);
    int readLine(char *buf, int len);
    std::string echoRead();
    std::string readKeyWord();
    int writen(const char *buf, int len);

private:
    int _fd;

};

#endif
