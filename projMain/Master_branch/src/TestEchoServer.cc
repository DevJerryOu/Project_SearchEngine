#include "../include/framework/EchoServer.h"

#include "../include/cache/RedisPPCache.h"

int main(int argc, char **argv)
{
    EchoServer es(4, 10, "127.0.0.1", 8888);
    es.start();
    return 0;
}

