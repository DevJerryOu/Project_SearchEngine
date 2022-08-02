#include"PageLibPreprocessor.hpp"

int main(){
	Configuration *conf=Configuration::getInstance();
	PageLibPreprocessor pageprocess(*conf);//先read！填完_pageLib _offsetLib
	pageprocess.doProcess();








/*	Configuration *conf=Configuration::getInstance();
	WordSegmentation jieba;
	
	string str1="<docid>21</docid> <title> 构造函数里调用文档的处理函数。雨后江汉天阔小，老周新科制多少，远山险竹林芳草，春风拂绿了芭蕉。寒梅落尽把冬了";
	string str2="<docid>33</docid> <title>hello,world.王道在线";
	cout<<str1<<endl;
	cout<<str2<<endl;

	WebPage page1(str1,*conf,jieba);
	WebPage page2(str2,*conf,jieba);
	if(page1==page2){
		cout<<"page1=page2"<<endl;
	}
	else cout<<"page1!=page2"<<endl;
	*/


	return 0;
}
