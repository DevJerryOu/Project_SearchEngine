#include "DictProducer.h"

int main(void)
{
	cout << "before reading..." << endl;

	time_t beg = time(NULL);
	DictProducer dictionary("The_Holy_Bible.txt");
	DictProducer dictionary2("english.txt");
	time_t end = time(NULL);
	cout << "time: " << (end - beg) << "s" << endl;
	cout << "aftre reading..." << endl;
	dictionary.storeDict("dictMap1.dat");
	dictionary.storeIndex("Index1.dat");
	dictionary2.storeDict("dictMap2.dat");
	dictionary2.storeIndex("Index2.dat");
	return 0;
}