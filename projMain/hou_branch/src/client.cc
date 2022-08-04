#include <func.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <boost/json/src.hpp>
#include <boost/json/object.hpp>

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using namespace boost::json;

int main(int argc, char* argv[])
{
    // 创建套接字
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(clientfd, -1, "socket");

    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(8888);
    serAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    // 向服务器发起建立连接的请求
    int ret = connect(clientfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "connect");
    printf("connect has completed.\n");

    // 客户端向服务器发送json格式的关键字
    cout << "请输入关键字: " <<endl;
    boost::json::object json_data;
    char buf[65535] = {0};
    cin >> buf;
    json_data["key_words"] = buf;
    ret = send(clientfd, serialize(json_data).c_str(), strlen(buf), 0);
    ERROR_CHECK(ret, -1, "send");
    cout << "send to server: " << serialize(json_data).c_str() << endl;
    memset(buf, 0, sizeof(buf));

    // 客户端接收服务器返回string数据
    boost::json::value confirm_list;
    boost::json::object json_obj;
    ret = recv(clientfd, buf, sizeof(buf), 0);
    ERROR_CHECK(ret, -1, "recv");
    confirm_list = parse(buf);
    json_obj = confirm_list.get_object();
    cout << "recv from server: " << serialize(confirm_list) << endl;
    memset(buf, 0, sizeof(buf));

    // 客户端选择一个序号
    cout << "输入候选词的序号: " << endl;
    int id;
    cin >> id;
    cout << "发送候选词: " << endl;
    // 根据序号找到候选词
    switch(id)
    {
        case 1: 
            strcpy(buf, serialize(json_obj["candidate_word1"]).c_str());
        case 2:
            strcpy(buf, serialize(json_obj["candidate_word2"]).c_str());
        case 3:
            strcpy(buf, serialize(json_obj["candidate_word3"]).c_str());
        default:
            break;
    }
    // 将候选词用json格式发送给服务器
    ret = send(clientfd, buf, sizeof(buf), 0);
    ERROR_CHECK(ret, -1, "send");
    cout << buf << endl;
    memset(buf, 0, sizeof(buf));

    // 服务器返回客户端选择的序号的相关内容
    boost::json::value confirm_content;
    boost::json::object content_obj;
    ret = recv(clientfd, buf, sizeof(buf), 0);
    ERROR_CHECK(ret, -1, "recv");
    confirm_content = parse(buf);
    content_obj = confirm_content.get_object();
    cout << "recv from server: " << serialize(confirm_content) << endl;
    memset(buf, 0, sizeof(buf));

    close(clientfd);

    return 0;
}