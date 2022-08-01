#ifndef __DictProducer_H__
#define __DictProducer_H__

#include "splitTool.h"
#include "Configuration.h"

#include <unordered_map>
#include <vector>
#include <string>

using std::vector;
using std::string;

class DictProducer
{
public:
    DictProducer();
    ~DictProducer();
    
    void buildCnDict();
    void createIndex();
    void storeDict();
    void storeIndex();

private:
   splitTool &_sTool; //引用分词工具类
   //Configuration &_conf; //配置文件类 ：获取文件路径后 以文件流的形式打开存放在map中

   vector<pair<string,int>> _dict;
   
};




#endif
