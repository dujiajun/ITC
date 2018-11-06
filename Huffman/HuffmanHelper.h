#ifndef HUFFMANHELPER_H
#define HUFFMANHELPER_H


#include <fstream>
#include "HuffmanTree.h"
#include "DataType.h"

bool cmpByByte(const ByteInfo &x, const ByteInfo &y);
bool cmpByWord(const ByteInfo &x, const ByteInfo &y);

class HuffmanHelper
{
	HuffmanTree tree;
	std::fstream fout;
	std::fstream fin;
	std::vector<ByteInfo> table;
	std::vector<byte> rawBytes;
	HuffmanHelper() {}
	HuffmanHelper(const HuffmanHelper &);
	size_t getFileSize(FILE* file);
	size_t getFileSize(const char* filePath);
	void readFromFile(const char* filePath);
public:
	static HuffmanHelper* getInstance()
	{
		static HuffmanHelper instance;
		return &instance;
	}
	void encode(const std::string &input, const std::string &output);
	void decode(const std::string &input, const std::string &output);
};

#endif // !HUFFMANHELPER_H
