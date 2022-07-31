#include"pagelib.hh"
int main(){
	DirScanner dir;
	dir.traverse("../resource/yuliao_web");
	PageLib pagelib(dir);
	pagelib.create();

	pagelib.store();

	return 0;
}
