#include "pagelib.hpp"
#include "PageLibPreprocessor.hpp"
#include "WebPageSearcher.hpp"
int main()
{
	Configuration *conf = Configuration::getInstance();

	DirScanner dir;
	dir.traverse((*conf)["web_yuliao"]);
	PageLib pagelib(dir, *conf);
	pagelib.create();
	pagelib.store();

	PageLibPreprocessor pageprocess(*conf); //先read！填完_pageLib _offsetLib
	pageprocess.doProcess();
	int pagenum = pageprocess.getPageNum();
	cout << "pagenum=" << pagenum << endl;

#if 0
	string str = "时代新人";
	WebPageSearcher seach(str, *conf, 56);
	seach.doQuery();
#endif
#if 0
	string str = "消费体验";
	WebPageSearcher seach(str, *conf, 56);
	seach.doQuery();
#endif
#if 0
	string str = "科技创新";
	WebPageSearcher seach(str, *conf, 56);
	seach.doQuery();
#endif
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
