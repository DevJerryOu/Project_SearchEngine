#include "../include/DictProducer.h"

DictProducer::DictProducer() //传入分词，配置文件

{
    buildCnDict();
    createIndex();
}

DictProducer::~DictProducer() {}

void DictProducer::buildCnDict()
{
    map<string,string> &_conf = Configuration::getConfigMap();

}

void DictProducer::createIndex()
{

}

void DictProducer::storeDict() 
{

}

void DictProducer::storeIndex()
{

}
