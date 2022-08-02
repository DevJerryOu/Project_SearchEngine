#ifndef _WEBPAGESEARCHER_HPP_
#define _WEBPAGESEARCHER_HPP_
#include <iostream>
#include <string>
#include "WebPageQuery.hpp"
#include "Configuration.hpp"
using std::cout;
using std::endl;
using std::string;
class WebPageSearcher
{
private:
    string _sought;
    WebPageQuery _webpagequery;
    // TcpConnection _conn;
    Configuration &_conf;

public:
    WebPageSearcher(string keys, Configuration &, int);
    void doQuery();
};
#endif