#include"MyLog.hpp"
#include<unistd.h>
int main(){
	Configuration *conf=Configuration::getInstance();
	MyLog* plog=MyLog::getInstance();
	plog->writeInLog(*conf,"hello:1");
	sleep(3);
	plog->writeInLog(*conf,"after 3 s");
	sleep(10);
	plog->writeInLog(*conf,"after 10 s");


	return 0;
}
