#include "pagelib.hpp"
#include "PageLibPreprocessor.hpp"
#include "WebPageSearcher.hpp"
int main()
{
	Configuration *conf = Configuration::getInstance();
#if 0
	DirScanner dir;
	dir.traverse((*conf)["web_yuliao"]);
	PageLib pagelib(dir, *conf);
	pagelib.create();
	pagelib.store();

	PageLibPreprocessor pageprocess(*conf); //先read！填完_pageLib _offsetLib
	pageprocess.doProcess();
	int pagenum = pageprocess.getPageNum();
	cout << "pagenum=" << pagenum << endl;
#endif
#if 0
	string str = "时代新人";
	WebPageSearcher seach(str, *conf, 56);//第三个参数需要知道去重后的总文章数
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

	return 0;
}
