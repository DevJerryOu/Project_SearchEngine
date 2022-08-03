#ifndef __MYLOG_HPP__
#define __MYLOG_HPP__
#include"Configuration.hpp"
#include<time.h>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
class MyLog{
public:
	static MyLog* getInstance();
	static void destroy();
	void writeInLog(Configuration &conf,const string &str);//调用getDataTime()
private:
	MyLog();
	~MyLog(){};
	void getDataTime();
private:
	static MyLog* _pLog;
	string _time;
};
#endif
