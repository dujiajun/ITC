#pragma once
#ifndef BYTEIO_H
#define BYTEIO_H

#include<fstream>
#include<vector>
#include<string>
#include"DataType.h"

static size_t getFileSize(FILE* file);
static size_t getFileSize(const char* filePath);

class IOHelper
{
private:
	byte buffer;
	int cnt;
	std::fstream fout;
	std::fstream fin;
	IOHelper()
	{
		buffer = 0;
		cnt = 0;
	}

	IOHelper(const IOHelper &);
	IOHelper& operator=(const IOHelper &);
	~IOHelper();

public:
	static IOHelper* getInstance()
	{
		static IOHelper instance;
		return &instance;
	}
	void write(const std::string &x);
	void write(const std::vector<std::string> &x);
	void writeResidue();
	void setOutputFile(const char* filename);
	std::vector<byte> readFromFile(const char* filePath);
};
#endif // !BYTEIO_H
