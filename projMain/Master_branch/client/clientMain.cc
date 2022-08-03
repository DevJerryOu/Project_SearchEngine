#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>
#include <boost/json/src.hpp>
#include <cstdlib>
#include <cstdio>

using std::cin;
using std::cout;
using std::endl;
//using std::string;
 
void test() 
{
	int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd < 0) 
    {
		perror("socket");
		return ;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8888);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t length = sizeof(serverAddr);

	if(::connect(clientfd,(struct sockaddr*)&serverAddr, length) < 0) 
    {
		perror("connect");
		close(clientfd);
		return;
	}
	printf("conn has connected!...\n");

	while(1)
    {
        #if 0
		string line;
		getline(cin, line);
		cout << ">> pls input some message:";
		//1. 客户端先发数据
		send(clientfd, line.data(), line.size(), 0);

		char buff[128] = {0};
		recv(clientfd, buff, sizeof(buff), 0);
		printf("recv msg from server: %s\n", buff);
        #endif
        // 从终端输入
        cout << "Please input keywords:" << endl;
        std::string inputKeyWords;
        cin >> inputKeyWords;

        // 打包成json

        boost::json::object inputKeyWordsObj;

        // 给json对象赋值
        inputKeyWordsObj["task_type"] = "key_word_recommend";
        inputKeyWordsObj["key_words"] = inputKeyWords;

        // JSON对象序列化
        std::string cwl2Send = serialize(inputKeyWordsObj);

        cout << "cwl2Send = " << cwl2Send << endl;
        // 发送关键词到服务器
        cout << "cwl2Send.size() = " << cwl2Send.length() << endl;
        cout << "strlen(cwl2Send.c_str()) = " <<  strlen(cwl2Send.c_str()) << endl;

        // 发送json字符串的长度
        int lengthbuf = cwl2Send.length();
        // lengthbuf[0] = (char)cwl2Send.length();
        cout << "lengthbuf = " << lengthbuf << endl;
        send(clientfd,&lengthbuf,4,0);

        send(clientfd,cwl2Send.c_str(),lengthbuf,MSG_WAITALL);

        char recvBuf[1024];
        recv(clientfd,recvBuf,sizeof(recvBuf),0);
        cout << "recvBuf = " << recvBuf << endl;
        
	}

	close(clientfd);
} 
 
int main(int argc, char *argv[])
{
	test();
	return 0;
}
