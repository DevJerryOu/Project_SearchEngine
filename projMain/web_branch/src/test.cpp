
#include"pagelib.hh"
#include<iostream>
using namespace std;
int main(){
	DirScanner dir;
	dir.traverse("../resource/yuliao_web");
	FileProcesser fp(5000);
	
	PageLib pagelib(dir,fp);
	pagelib.create();

//	pagelib.store();

	return 0;
}
