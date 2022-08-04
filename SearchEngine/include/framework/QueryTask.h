#ifndef __QueryTask_H__
#define __QueryTask_H__

#include <string>
#include <boost/json/src.hpp>
#include <vector>
#include "TcpConnection.h"
#include "KeyRecommander.h"
using std::vector;
class QueryTask
{
public:
    QueryTask(const std::string &msg, const TcpConnectionPtr &con)
        : _msg(msg), _con(con)
    {
    }
    void process()
    {
        cout << "_msg=" << _msg << endl;
        set<std::string> st = keyWords(_msg);
        vector<std::string> jason_string;
        cout << "st.size()=" << st.size() << endl;
        for (auto &item : st)
        {
            cout << item << endl;
            jason_string.push_back(item);
        }
        boost::json::object inputKeyWordsObj;
        if (jason_string.size() == 3)
        {
            inputKeyWordsObj["ret_type"] = "choren";
            inputKeyWordsObj["candidate_word1"] = jason_string[0];
            inputKeyWordsObj["candidate_word2"] = jason_string[1];
            inputKeyWordsObj["candidate_word3"] = jason_string[2];
        }
        else if (jason_string.size() == 6)
        {
            inputKeyWordsObj["ret_type"] = "chanden";
            inputKeyWordsObj["candidate_word1"] = jason_string[0];
            inputKeyWordsObj["candidate_word2"] = jason_string[1];
            inputKeyWordsObj["candidate_word3"] = jason_string[2];
            inputKeyWordsObj["candidate_word4"] = jason_string[3];
            inputKeyWordsObj["candidate_word5"] = jason_string[4];
            inputKeyWordsObj["candidate_word6"] = jason_string[5];
        }

        std::string cwl2Send = serialize(inputKeyWordsObj);

        _con->sendInLoop(cwl2Send);
    }

private:
    std::string _msg;
    TcpConnectionPtr _con;
};

#endif
