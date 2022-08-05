#ifndef _WEBPAGESEARCHER_HPP_
#define _WEBPAGESEARCHER_HPP_
#include <iostream>
#include <vector>
#include <string>
#include "WebPageQuery.hpp"
#include "Configuration.hpp"
using std::cout;
using std::endl;
using std::string;
using std::vector;
class WebPageSearcher
{
private:
    std::string _sought;
    WebPageQuery _webpagequery;
    // TcpConnection _conn;
    Configuration &_conf;

public:
    WebPageSearcher(std::string keys, Configuration &, int);
    vector<std::string> doQuery();
};
#endif
