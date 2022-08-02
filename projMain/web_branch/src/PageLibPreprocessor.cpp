#include "PageLibPreprocessor.hpp"
#include <fstream>
PageLibPreprocessor::PageLibPreprocessor(Configuration &conf) : _jieba()
{
	//调用readinfofromfile读取网页库和偏移库的信息，填充至_pageLib, _offsetLib
    _pageLib.reserve(6000);
    string web_path = conf["webpage"];
    string offset_path = conf["offset"];
    readInfoFromFile(web_path, offset_path, conf);
    // cout << _pageLib.size() << endl;
}
void PageLibPreprocessor::doProcess()
{ //去除冗余网页，并创建倒排索引，并将其存至磁盘
//	cout<<"doProcess()"<<endl;
	cutRedundantPages();
    buildInvertIndexTable();
	storeOnDisk();
//	cout<<"doneprocess()"<<endl;
}
void PageLibPreprocessor::readInfoFromFile(string webPath, string offPath, Configuration &conf)
{
//	cout<<"readInfoFromFile()"<<endl;
    ifstream ifs_web, ifs_off;
    ifs_web.open(webPath);
    ifs_off.open(offPath);
    string off_line;
    while (getline(ifs_off, off_line))
    {
		istringstream istr(off_line);
        string tmp1, tmp2, tmp3;
        istr >> tmp1 >> tmp2 >> tmp3;
		cout<<tmp1<<" "<<tmp2<<" "<<tmp3<<endl;
        
		_offsetLib.insert({atoi(tmp1.c_str()), {atoi(tmp2.c_str()), atoi(tmp3.c_str())}});
        int fsize = atoi(tmp3.c_str());
		//cout<<"fsize_0:"<<fsize<<endl;
        string line_content, file_content;	
        while (getline(ifs_web, line_content))
        {
			file_content += line_content+"\n";
		//	cout<<"line_content:\n"<<line_content<<endl;
		//	cout<<"file_content:\n"<<file_content;
		//	cout<<"fsize:"<<fsize<<endl;
		//	cout<<"line_content.size():"<<line_content.size()+1<<endl;
            fsize -= (line_content.size()+1);//
		//	cout<<"fsize-content_size:"<<fsize<<endl<<endl;
            if (fsize == 0) break;
        }
		//cout<<atoi(tmp1.c_str())<<endl<<file_content<<endl;
		_pageLib.push_back(WebPage(file_content, conf, _jieba));
    }
	//print
	cout<<"_pageLib.size():"<<_pageLib.size()<<endl;
	cout<<"_offsetLib.size():"<<_offsetLib.size()<<endl;
	
	for(auto& page:_pageLib){
		cout<<page.getDoc()<<endl;
	}
	for(int i=1;i<=_offsetLib.size();i++){
		cout<<i<<" "<<_offsetLib[i].first<<" "<<_offsetLib[i].second<<endl;
	}
	
}
void PageLibPreprocessor::cutRedundantPages()
{
    int lines=_offsetLib.size();
	//网页去重，找到重复的网页，在文档表和索引表去除相应的表项
	cout<<"cutRedundantPages()"<<endl;
	cout<<"size:"<<_pageLib.size()<<endl;
	for(int i=0;i<_pageLib.size()-1;i++){
		for(int j=i+1;j<_pageLib.size();j++){
			cout<<"i:"<<i<<" "<<"j:"<<j<<endl;
			if(_pageLib[i]==_pageLib[j]){
				_offsetLib.erase(i+1);//map里已经没有重复的
				cout<<"erase docid:"<<i+1<<endl;
				break;
			}
		}
	}
	//打印测试
	for(int i=1;i<=lines;i++){
		if(_offsetLib.find(i)!=_offsetLib.end()){
			cout<<i<<" "<<_offsetLib[i].first<<" "<<_offsetLib[i].second<<endl;
		}
		else continue;
	}
}

void PageLibPreprocessor::buildInvertIndexTable()//填倒排索引的数据结构
{	
	cout<<"buildInvertIndexTable()"<<endl;
	int N=0;
	//统计去重后文档数N;初步填好倒排索引结构,double为出现次数
	for(auto page:_pageLib){
		if(_offsetLib.find(page.getDocId())==_offsetLib.end()) continue;//跳过重复的
		N++;
		map<string,int> wordmap=page.getWordsMap();
		/*
		for(auto map:wordmap){
			cout<<map.first.c_str()<<" "<<map.second<<endl;
		}
		cout<<endl;
		*/
		for(auto map:wordmap){//map是迭代器
			auto iter=_invertIndexTable.find(map.first);
			if(iter==_invertIndexTable.end()){
//				cout<<"不存在"<<endl;
				vector<pair<int,double>> temp;
				//cout<<"id:"<<page.getDocId()<<" "<<"frequency:"<<map.second<<endl;
				temp.push_back({page.getDocId(),map.second});//先用来存frequency，方便后续计算归一化权重
				//cout<<"temp.size():"<<temp.size()<<endl;
				_invertIndexTable.insert({map.first,temp});
			}
			else{
//				cout<<"存在"<<endl;
				iter->second.push_back({page.getDocId(),map.second});
			}
		}
	}
	/*
	//打印检查倒排索引表1.0
	for(auto elem: _invertIndexTable){//迭代器
		cout<<elem.first<<" ";
		int n=0;
		for(auto data:elem.second){
			cout<<data.first<<" "<<data.second;
			if(n<elem.second.size()) cout<<" ";
		}
		cout<<endl;
	}
	*/
	
	for(auto & word:_invertIndexTable){//word是迭代器
		int DF=word.second.size();//包含该词的文章个数
//		cout<<"DF:"<<DF<<endl;
		double IDF=log(N)/log(DF+1);//逆文档频率
//		cout<<"IDF:"<<IDF<<endl;
		vector<double> w;//每篇文章中的权重
		vector<double> wUnify;//归一化权重
		//记录这个词在每篇文章中的权重，一行
		for(auto &data:word.second){
			int TF=data.second;//这个词在这篇文章中的次数
//			cout<<"TF:"<<TF<<endl;
			double wTemp=TF*IDF;//权重
//			cout<<"wTemp:"<<wTemp<<endl;
			w.push_back(wTemp);
		}
		/*
		//test
		for(auto& data:w){
			cout<<data<<" ";
		}
		cout<<endl;
		*/
		//求归一化权重分母
		double wDown;
		for(auto& wPer:w){
			wDown+=pow(wPer,2);
		}
		wDown=sqrt(wDown);
//		cout<<"wDown:"<<wDown<<endl;
		//记录归一化权重
		for(auto& wPer:w){
//			cout<<wPer/wDown<<" ";
			wUnify.push_back(wPer/wDown);
		}
		//将得到的归一化权重写入对应vector的第二个参数
		int i=0;
		for(auto& data:word.second){
			data.second=wUnify[i];
			i++;
		}
		//每行打印测试
		cout<<word.first<<" ";
		for(auto &data:word.second){
			cout<<data.first<<" "<<data.second<<" ";
		}
		cout<<endl;
	}
	/*
	//打印测试
	for(auto& elem:_invertIndexTable){
		cout<<elem.first<<" ";
		int n=1;
		for(auto& data:elem.second){
			cout<<data.first<<" "<<data.second;
			if(n<elem.second.size()) cout<<" ";
			n++;
		}
		cout<<endl;
	}
	*/
}
void PageLibPreprocessor::storeOnDisk()//不用重新存网页库和偏移库，只用标记好，然后生成倒排索引库，再存下来就好
{
	cout<<"storeOnDisk()"<<endl;
	ofstream ofs("../data/invertIndex.dat");
	if(!ofs){
		cout<<"open newripepage.dat fail!"<<endl;
		exit(0);
	}
	for(auto& word:_invertIndexTable){
		ofs<<word.first<<" ";
		int n=0;
		for(auto& idx:word.second){
			ofs<<idx.first<<" "<<idx.second;
			if(n<word.second.size()) ofs<<" ";
			n++;
		}
		ofs<<endl;
	}
	ofs.close();
}
