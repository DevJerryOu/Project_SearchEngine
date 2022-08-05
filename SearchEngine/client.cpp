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
// using std::string;

template <class T>
void extract(boost::json::object const &obj, T &t, boost::core::string_view key)
{
    t = boost::json::value_to<T>(obj.at(key));
}

void test()
{
    int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd < 0)
    {
        perror("socket");
        return;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t length = sizeof(serverAddr);

    if (::connect(clientfd, (struct sockaddr *)&serverAddr, length) < 0)
    {
        perror("connect");
        close(clientfd);
        return;
    }
    printf("conn has connected!...\n");

    while (1)
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
        //  发送关键词到服务器
        cout << "cwl2Send.size() = " << cwl2Send.length() << endl;
        cout << "strlen(cwl2Send.c_str()) = " << strlen(cwl2Send.c_str()) << endl;

        // 发送json字符串的长度
        int lengthbuf = cwl2Send.length();
        // lengthbuf[0] = (char)cwl2Send.length();
        // cout << "lengthbuf = " << lengthbuf << endl;
        send(clientfd, &lengthbuf, 4, 0);

        // 发送关键词并接收回显
        send(clientfd, cwl2Send.c_str(), lengthbuf, MSG_WAITALL);

        // char recvBuf[1024];
        // recv(clientfd,recvBuf,sizeof(recvBuf),0);
        // cout << "recvBuf = " << recvBuf << endl;

        cout << "send word success" << endl;
        // 接收关键词列表json字符串的长度
        int lenOfKeyWordsList;
        recv(clientfd, &lenOfKeyWordsList, 4, 0);
        cout << "recv wordlist" << lenOfKeyWordsList << endl;

        // 接收关键词列表json字符串
        char keyWordListBuf[1024] = {0};
        recv(clientfd, keyWordListBuf, lenOfKeyWordsList, MSG_WAITALL);

        cout << "msg=" << keyWordListBuf << endl;
        //解析关键词列表json字符串

        std::string keyWordList = (std::string)keyWordListBuf;
        boost::json::value keyWordListVal;
        keyWordListVal = boost::json::parse(keyWordList);
        boost::json::object keyWordListObj = keyWordListVal.get_object();

        std::string ret_type;              // 待获取的字段值
        std::string type_key = "ret_type"; // 字段名
        extract(keyWordListObj, ret_type, type_key);

        std::string confirmWord;
        if (ret_type == "choren")
        {
            std::string candidateWord1;                             // 待获取的字段值
            std::string candidateWordName2Get1 = "candidate_word1"; // 字段名
            extract(keyWordListObj, candidateWord1, candidateWordName2Get1);

            std::string candidateWord2;                             // 待获取的字段值
            std::string candidateWordName2Get2 = "candidate_word2"; // 字段名
            extract(keyWordListObj, candidateWord2, candidateWordName2Get2);

            std::string candidateWord3;                             // 待获取的字段值
            std::string candidateWordName2Get3 = "candidate_word3"; // 字段名
            extract(keyWordListObj, candidateWord3, candidateWordName2Get3);

            // 将候选词打印到终端，让用户选择
            cout << "Pls choose the key word:" << endl;
            cout << "1." << candidateWord1 << ' '
                 << "2." << candidateWord2 << ' '
                 << "3." << candidateWord3 << endl;

            int keyWordNum;
            cin >> keyWordNum;
            switch (keyWordNum)
            {
            case 1:
                confirmWord = candidateWord1;
                break;
            case 2:
                confirmWord = candidateWord2;
                break;
            case 3:
                confirmWord = candidateWord3;
                break;
            default:
                cout << "Error" << endl;
            }
            if (keyWordNum <= 0 || keyWordNum > 3)
            {
                break;
            }
        }
        else if (ret_type == "chanden")
        {
            std::string candidateWord1;                             // 待获取的字段值
            std::string candidateWordName2Get1 = "candidate_word1"; // 字段名
            extract(keyWordListObj, candidateWord1, candidateWordName2Get1);

            std::string candidateWord2;                             // 待获取的字段值
            std::string candidateWordName2Get2 = "candidate_word2"; // 字段名
            extract(keyWordListObj, candidateWord2, candidateWordName2Get2);

            std::string candidateWord3;                             // 待获取的字段值
            std::string candidateWordName2Get3 = "candidate_word3"; // 字段名
            extract(keyWordListObj, candidateWord3, candidateWordName2Get3);

            std::string candidateWord4;                             // 待获取的字段值
            std::string candidateWordName2Get4 = "candidate_word4"; // 字段名
            extract(keyWordListObj, candidateWord4, candidateWordName2Get4);

            std::string candidateWord5;                             // 待获取的字段值
            std::string candidateWordName2Get5 = "candidate_word5"; // 字段名
            extract(keyWordListObj, candidateWord5, candidateWordName2Get5);

            std::string candidateWord6;                             // 待获取的字段值
            std::string candidateWordName2Get6 = "candidate_word6"; // 字段名
            extract(keyWordListObj, candidateWord6, candidateWordName2Get6);
            // 将候选词打印到终端，让用户选择
            cout << "Pls choose the key word:" << endl;
            cout << "1." << candidateWord1 << ' '
                 << "2." << candidateWord2 << ' '
                 << "3." << candidateWord3 << ' '
                 << "4." << candidateWord4 << ' '
                 << "5." << candidateWord5 << ' '
                 << "6." << candidateWord6 << endl;

            int keyWordNum;
            cin >> keyWordNum;
            switch (keyWordNum)
            {
            case 1:
                confirmWord = candidateWord1;
                break;
            case 2:
                confirmWord = candidateWord2;
                break;
            case 3:
                confirmWord = candidateWord3;
                break;
            case 4:
                confirmWord = candidateWord4;
                break;
            case 5:
                confirmWord = candidateWord5;
                break;
            case 6:
                confirmWord = candidateWord6;
                break;
            default:
                cout << "Error" << endl;
            }
            if (keyWordNum <= 0 || keyWordNum > 6)
            {
                continue;
            }
        }
        else if (ret_type == "error")
        {
            cout << "Sorry,this word not exist" << endl;
            continue;
        }

        // 得到confirmWord
        // 打包到json

        boost::json::object confirmWordObj;

        // 给json对象赋值
        confirmWordObj["task_type"] = "article_search";
        confirmWordObj["confirm_word"] = confirmWord;
        // JSON对象序列化
        std::string confirmWord2Send = boost::json::serialize(confirmWordObj);

        // 先发送json字符串的长度
        int confirmWordLen = confirmWord2Send.length();
        // lengthbuf[0] = (char)cwl2Send.length();
        // cout << "confirmWordLen = " << confirmWordLen << endl;
        int ret = send(clientfd, &confirmWordLen, 4, 0);

        // 发送确定词
        ret = send(clientfd, confirmWord2Send.c_str(), confirmWordLen, MSG_WAITALL);

        // 接收文章
        // 接收文章json字符串的长度
        int lenOfArticleJson;
        recv(clientfd, &lenOfArticleJson, 4, 0);
        // 接收文章的json字符串
        // cout << "article len=" << lenOfArticleJson << endl;
        char ArticleJsonBuf[1024] = {0};
        recv(clientfd, ArticleJsonBuf, lenOfArticleJson, MSG_WAITALL);

        // cout << "Article=" << ArticleJsonBuf << endl;
        // cout << "size=" << strlen(ArticleJsonBuf) << endl;
        //  接到文章的json，进行解析
        //  // 模块二 文章搜索
        boost::json::value articleJsonVal;
        boost::json::object articleJsonObj;
        articleJsonVal = boost::json::parse(ArticleJsonBuf);
        articleJsonObj = articleJsonVal.get_object();

        std::string fieldValue1;                      // 用来接收字段值的
        std::string fieldName2Get1 = "article_title"; // 字段名

        std::string fieldValue2;                         // 用来接收字段值的
        std::string fieldName2Get2 = "article_abstract"; // 字段名

        // 从json对象里面提取article_
        extract(articleJsonObj, fieldValue1, fieldName2Get1);
        extract(articleJsonObj, fieldValue2, fieldName2Get2);

        // 获取到了文章信息，打印到终端显示给用户
        cout << "article_title: " << fieldValue1 << endl;
        cout << "article_abstract: " << fieldValue2 << endl;
    }

    close(clientfd);
}

int main(int argc, char *argv[])
{
    test();
    return 0;
}
