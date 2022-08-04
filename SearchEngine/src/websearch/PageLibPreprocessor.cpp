#include "PageLibPreprocessor.hpp"
#include <fstream>
PageLibPreprocessor::PageLibPreprocessor(Configuration &conf) : _jieba()
{
	//调用readinfofromfile读取网页库和偏移库的信息，填充至_pageLib, _offsetLib
	_pageLib.reserve(6000);
	std::string web_path = conf["webpage"];
	std::string offset_path = conf["offset"];
	readInfoFromFile(web_path, offset_path, conf);
	// cout << _pageLib.size() << endl;
}
void PageLibPreprocessor::doProcess()
{ //去除冗余网页，并创建倒排索引，并将其存至磁盘
	//	cout<<"doProcess()"<<endl;
	cutRedundantPages();
	buildInvertIndexTable();
	storeOnDisk();
	cout << "doneprocess()" << endl;
}
void PageLibPreprocessor::readInfoFromFile(std::string webPath, std::string offPath, Configuration &conf)
{
	//	cout<<"readInfoFromFile()"<<endl;
	ifstream ifs_web, ifs_off;
	ifs_web.open(webPath);
	ifs_off.open(offPath);
	std::string off_line;
	while (getline(ifs_off, off_line))
	{
		istringstream istr(off_line);
		std::string tmp1, tmp2, tmp3;
		istr >> tmp1 >> tmp2 >> tmp3;
		cout << tmp1 << " " << tmp2 << " " << tmp3 << endl;

		_offsetLib.insert({atoi(tmp1.c_str()), {atoi(tmp2.c_str()), atoi(tmp3.c_str())}});
		int fsize = atoi(tmp3.c_str());
		// cout<<"fsize_0:"<<fsize<<endl;
		std::string line_content, file_content;
		while (getline(ifs_web, line_content))
		{
			file_content += line_content + "\n";
			//	cout<<"line_content:\n"<<line_content<<endl;
			//	cout<<"file_content:\n"<<file_content;
			//	cout<<"fsize:"<<fsize<<endl;
			//	cout<<"line_content.size():"<<line_content.size()+1<<endl;
			fsize -= (line_content.size() + 1); //
												//	cout<<"fsize-content_size:"<<fsize<<endl<<endl;
			if (fsize == 0)
				break;
		}
		cout << "fsize=" << fsize << endl;
		// cout<<atoi(tmp1.c_str())<<endl<<file_content<<endl;
		_pageLib.push_back(WebPage(file_content, conf, _jieba));
	}
// print
#if 0
	cout << "_pageLib.size():" << _pageLib.size() << endl;
	cout << "_offsetLib.size():" << _offsetLib.size() << endl;

	for (auto &page : _pageLib)
	{
		cout << page.getDoc() << endl;
	}
	for (int i = 1; i <= _offsetLib.size(); i++)
	{
		cout << i << " " << _offsetLib[i].first << " " << _offsetLib[i].second << endl;
	}
#endif
}
#if 0
void PageLibPreprocessor::readInfoFromFile(std::string webPath, std::string offPath, Configuration &conf)
{
	ifstream ifs;
	ifs.open(offPath);
	vector<pair<int, int>> offset;
	std::string line;
	while (getline(ifs, line))
	{
		istringstream istr(line);
		std::string tmp1, tmp2, tmp3;
		istr >> tmp1 >> tmp2 >> tmp3;
		offset.push_back({atoi(tmp2.c_str()), atoi(tmp3.c_str())});
	}
	ifs.close();

	ifs.open(webPath);
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	std::string content(buffer.str());
	ifs.close();
	cout << "content.size" << content.size() << endl;
	for (int i = 0; i < offset.size(); i++)
	{
		cout << i << "substr start" << endl;
		std::string page = content.substr(offset[0].first, offset[0].second);
		cout << "substr finish" << endl;
		_pageLib.push_back(WebPage(page, conf, _jieba));
		cout << "push_back finish" << endl;
	}
}
#endif
void PageLibPreprocessor::getSimhash()
{
	Simhasher sim("../include/websearch/simhash-master/dict/jieba.dict.utf8",
				  "../include/websearch/simhash-master/dict/hmm_model.utf8",
				  "../include/websearch/simhash-master/dict/idf.utf8",
				  "../include/websearch/simhash-master/dict/stop_words.utf8");
	size_t topN = 8;
	std::string str;
	unit64_t u64;
	vector<pair<std::string, double>> res;
	unit12_t a, b, c, d;
	unit64_t x = 65535;
	for (auto &elem : _pageLib)
	{
		str = elem.getDoc();
		sim.extract(str, res, topN);
		sim.make(str, topN, u64);
		cout << elem.getDocId() << ":" << u64 << endl;
		//压入_simhashVal
		_simhashVal.insert({elem.getDocId(), u64});
		a = (u64 & (x << 48)) >> 48;
		b = (u64 & (x << 32)) >> 32;
		c = (u64 & (x << 16)) >> 16;
		d = u64 & x;
		cout << a << " " << b << " " << c << " " << d << endl;
		auto it1 = _partA.find(a);
		auto it2 = _partB.find(b);
		auto it3 = _partC.find(c);
		auto it4 = _partD.find(d);
		//压入_partA
		if (it1 != _partA.end())
		{ //已经存在
			it1->second.push_back({u64, elem.getDocId()});
		}
		else
		{
			vector<pair<unit64_t, int>> tmp;
			tmp.push_back({u64, elem.getDocId()});
			_partA.insert({a, tmp});
		}
		//压入_partB
		if (it2 != _partB.end())
		{ //已经存在
			it2->second.push_back({u64, elem.getDocId()});
		}
		else
		{
			vector<pair<unit64_t, int>> tmp;
			tmp.push_back({u64, elem.getDocId()});
			_partB.insert({b, tmp});
		}
		//压入_partC
		if (it3 != _partC.end())
		{ //已经存在
			it3->second.push_back({u64, elem.getDocId()});
		}
		else
		{
			vector<pair<unit64_t, int>> tmp;
			tmp.push_back({u64, elem.getDocId()});
			_partC.insert({c, tmp});
		}
		//压入_partD
		if (it4 != _partD.end())
		{ //已经存在
			it4->second.push_back({u64, elem.getDocId()});
		}
		else
		{
			vector<pair<unit64_t, int>> tmp;
			tmp.push_back({u64, elem.getDocId()});
			_partD.insert({d, tmp});
		}
	}
}
void PageLibPreprocessor::cutRedundantPages()
{
	int lines = _offsetLib.size();
	getSimhash();
	cout << "cutRedundantPages()" << endl;
	//	cout<<"size:"<<_pageLib.size()<<endl;
	unit12_t a, b, c, d;
	for (int i = 0; i < _pageLib.size(); i++)
	{
		unit64_t simhash = _simhashVal.find(i + 1)->second;
		cout << i + 1 << ":" << simhash << endl;
		unit64_t x = 65535;
		a = (simhash & (x << 48)) >> 48;
		b = (simhash & (x << 32)) >> 32;
		c = (simhash & (x << 16)) >> 16;
		d = simhash & x;
		cout << a << " " << b << " " << c << " " << d << endl;
		auto it1 = _partA.find(a);
		auto it2 = _partB.find(b);
		auto it3 = _partC.find(c);
		auto it4 = _partD.find(d);
		bool isSame = 0;
		if (it1 != _partA.end())
		{
			for (auto &elem : it1->second)
			{
				if (elem.second != i + 1 && Simhasher::isEqual(simhash, elem.first) && _offsetLib.find(elem.second) != _offsetLib.end())
				{
					_offsetLib.erase(i + 1);
					cout << "erase docid:" << i + 1 << endl;
					isSame = 1;
					break;
				}
			}
		}
		if (isSame)
			continue;
		if (it2 != _partB.end())
		{
			for (auto &elem : it2->second)
			{
				if (elem.second != i + 1 && Simhasher::isEqual(simhash, elem.first) && _offsetLib.find(elem.second) != _offsetLib.end())
				{
					_offsetLib.erase(i + 1);
					cout << "erase docid:" << i + 1 << endl;
					isSame = 1;
					break;
				}
			}
		}
		if (isSame)
			continue;
		if (it3 != _partC.end())
		{
			for (auto &elem : it3->second)
			{
				if (elem.second != i + 1 && Simhasher::isEqual(simhash, elem.first) && _offsetLib.find(elem.second) != _offsetLib.end())
				{
					_offsetLib.erase(i + 1);
					cout << "erase docid:" << i + 1 << endl;
					isSame = 1;
					break;
				}
			}
		}
		if (isSame)
			continue;
		if (it4 != _partD.end())
		{
			for (auto &elem : it4->second)
			{
				if (elem.second != i + 1 && Simhasher::isEqual(simhash, elem.first) && _offsetLib.find(elem.second) != _offsetLib.end())
				{
					_offsetLib.erase(i + 1);
					cout << "erase docid:" << i + 1 << endl;
					isSame = 1;
					break;
				}
			}
		}
	}
//打印测试
#if 0
	if (_offsetLib.find(26) == _offsetLib.end())
		cout << "26" << endl;
	if (_offsetLib.find(30) == _offsetLib.end())
		cout << "30" << endl;
	cout << "lines:" << lines << endl;
	for (int i = 1; i <= lines; i++)
	{
		if (_offsetLib.find(i) != _offsetLib.end())
		{
			cout << i << " " << _offsetLib[i].first << " " << _offsetLib[i].second << endl;
		}
		else
			continue;
	}
#endif
}

void PageLibPreprocessor::buildInvertIndexTable() //填倒排索引的数据结构
{
	cout << "buildInvertIndexTable()" << endl;
	int N = 0;
	// 1.统计去重后文档数N;初步填好倒排索引结构,double为出现次数
	for (auto page : _pageLib)
	{
		if (_offsetLib.find(page.getDocId()) == _offsetLib.end())
			continue; //跳过重复的
		N++;
		map<std::string, int> wordmap = page.getWordsMap();
		/*
		for(auto map:wordmap){
			cout<<map.first.c_str()<<" "<<map.second<<endl;
		}
		cout<<endl;
		*/
		for (auto map : wordmap)
		{
			auto iter = _invertIndexTable.find(map.first);
			if (iter == _invertIndexTable.end())
			{
				//				cout<<"不存在"<<endl;
				vector<pair<int, double>> temp;
				// cout<<"id:"<<page.getDocId()<<" "<<"frequency:"<<map.second<<endl;
				temp.push_back({page.getDocId(), map.second}); //先用来存frequency，方便后续计算归一化权重
				// cout<<"temp.size():"<<temp.size()<<endl;
				_invertIndexTable.insert({map.first, temp});
			}
			else
			{
				//				cout<<"存在"<<endl;
				iter->second.push_back({page.getDocId(), map.second});
			}
		}
	}
	// 2.按std::string遍历,_invertIndexTable中double是在每篇文章中的权重
	for (auto &word : _invertIndexTable)
	{									   // word是迭代器
		int DF = word.second.size();	   //包含该词的文章个数
										   //		cout<<"DF:"<<DF<<endl;
		double IDF = log(N) / log(DF + 1); //逆文档频率
										   //		cout<<"IDF:"<<IDF<<endl;
										   //		vector<double> w;//每篇文章中的权重
										   //		vector<double> wUnify;//归一化权重
		for (auto &data : word.second)
		{
			int TF = data.second;	 //这个词在这篇文章中的次数
									 //			cout<<"TF:"<<TF<<endl;
			double wTemp = TF * IDF; //权重
									 //			cout<<"wTemp:"<<wTemp<<endl;
			data.second = wTemp;
		}
	}
	// 3.求每篇文章的归一化权重分母
	vector<double> wDown = {0};
	wDown.reserve(6000);
	for (auto &word : _invertIndexTable)
	{
		for (auto &data : word.second)
		{
			wDown[data.first - 1] += pow(data.second, 2);
		}
	}
	//开方
	for (auto &down : wDown)
	{
		down = sqrt(down);
	}
	// 4.写入归一化权重
	for (auto &word : _invertIndexTable)
	{
		double wUnify;
		for (auto &data : word.second)
		{
			wUnify = data.second / wDown[data.first - 1];
			data.second = wUnify;
		}
	}
	ofstream file1("../conf/configure.txt", ios::app);
	if (!file1)
	{
		cout << "open fortest.txt fail!" << endl;
		exit(0);
	}
	file1 << "pageNum " << N << endl;
	file1.close();
#if 0
	//打印测试
	for (auto &word : _invertIndexTable)
	{
		for (auto &data : word.second)
		{
			cout << word.first << " " << data.first << " " << data.second << endl;
		}
	}
#endif
	/*
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
void PageLibPreprocessor::storeOnDisk() //不用重新存网页库和偏移库，只用标记好，然后生成倒排索引库，再存下来就好
{
	cout << "storeOnDisk()" << endl;
	ofstream ofs("../data/invertIndex.dat");
	if (!ofs)
	{
		cout << "open newripepage.dat fail!" << endl;
		exit(0);
	}
	for (auto &word : _invertIndexTable)
	{
		for (auto &idx : word.second)
		{
			ofs << word.first << " " << idx.first << " " << idx.second << endl;
		}
	}
	ofs.close();
}
