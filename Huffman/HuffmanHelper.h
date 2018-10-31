#ifndef HUFFMANHELPER_H
#define HUFFMANHELPER_H

#define COUT_IN_CONSOLE
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
	//std::string input_filename, output_filename;
	//void writeEncode();
	//void writeDecode();
	HuffmanHelper() {}
	HuffmanHelper(const HuffmanHelper &);
	size_t getFileSize(FILE* file);
	size_t getFileSize(const char* filePath);
	void readFromFile(const char* filePath);
public:
	//static enum ACTION { ENCODE, DECODE };
	static HuffmanHelper* getInstance()
	{
		static HuffmanHelper instance;
		return &instance;
	}
	void encode(const std::string &input, const std::string &output);
	void decode(const std::string &input, const std::string &output);
	//void writeTofile(const std::string &filename, ACTION action);
};

#endif // !HUFFMANHELPER_H
