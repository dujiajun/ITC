#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include "HuffmanHelper.h"
#include "DataType.h"
using namespace std;

void HuffmanHelper::encode(const string &input, const string &output)
{
	map<byte, int> probs;
	set<byte> byteSet;

	rawBytes.clear();
	table.clear();
	tree.clear();

#ifdef COUT_IN_CONSOLE
	double progress = 0;
	double proc = 0;
	cout << "开始读入文件..." << endl;
#endif // COUT_IN_CONSOLE

	readFromFile(input.c_str());

	for(auto &i:rawBytes)
	{
		if (byteSet.find(i) == byteSet.end())
		{
			byteSet.insert(i);
			probs[i] = 1;
		}
		else
		{
			probs[i] += 1;
		}
	}
	for (auto &i : byteSet)
	{
		ByteInfo b = ByteInfo(i, probs[i]);
		table.push_back(b);
	}
#ifdef COUT_IN_CONSOLE
	cout << "开始编码..." << endl;
#endif
	tree.encode(table);
	
	fout.open(output, ios::out | ios::binary);
	if (!fout.is_open())return;

	map<byte, string> dict;

	char cnt = 0;
	char buffer = 0;

	size_t dict_cnt = table.size();
	fout.write((char*)&dict_cnt, sizeof(dict_cnt));
#ifdef COUT_IN_CONSOLE
	progress = 0;
	proc = 0;
	cout << "开始写入文件..." << endl;
	cout << "Progress:" << proc << endl;
#endif
	for (auto &i : table)
	{
		dict[i.value] = i.word;
		fout.write((char*)&i.value, sizeof(i.value));
		char tmp = i.word.length();
		fout.write((char*)&tmp, sizeof(tmp));

		for (auto &j : i.word)
		{
			if (cnt < 8)
			{
				buffer = (buffer << 1) + (j - '0');
				cnt++;
			}
			if (cnt == 8)
			{
				fout.write((char*)&buffer, sizeof(buffer));
				cnt = 0;
				buffer = 0;
			}
		}
		if (cnt != 0)
		{
			buffer = buffer << (8 - cnt);
			fout.write((char*)&buffer, sizeof(buffer));
			cnt = 0;
			buffer = 0;
		}
	}
	cnt = 0;
	buffer = 0;
	
	int resBit = 0;
	for (auto &i : rawBytes)resBit += dict[i].length();
	resBit %= 8;

	fout.write((char*)&resBit, sizeof(resBit));

	for(auto &i:rawBytes)
	{
#ifdef COUT_IN_CONSOLE
		progress++;
		if (progress / rawBytes.size() > proc + 0.01)
		{
			proc = progress / rawBytes.size();
			cout << "Progress:" << proc << endl;
		}
#endif
		for (auto &j : dict[i])
		{
			if (cnt < 8)
			{
				buffer = (buffer << 1) + (j - '0');
				cnt++;
			}
			if (cnt == 8)
			{
				fout.write((char*)&buffer, sizeof(buffer));
				cnt = 0;
				buffer = 0;
			}
		}
	}
	if (cnt != 0)
	{
		buffer = buffer << (8 - cnt);
		fout.write((char*)&buffer, sizeof(buffer));
		cnt = 0;
		buffer = 0;
	}
	fin.close();
	fout.close();
#ifdef COUT_IN_CONSOLE
	cout << "写入完成" << endl;
#endif
}

void HuffmanHelper::decode(const string &input, const string &output)
{
	fout.open(output, ios::out | ios::binary);
	if (!fout.is_open())return;

	rawBytes.clear();
	table.clear();
	map<string, byte> dict;
	set<string> codeSet;

#ifdef COUT_IN_CONSOLE
	double progress = 0;
	double proc = 0;
	cout << "开始读入文件..." << endl;
#endif

	readFromFile(input.c_str());

	size_t dict_cnt = 0;
	auto pos = rawBytes.begin();
	for (int i = 0; i < sizeof(dict_cnt); i++)
	{
		char * ptr = (char*)&dict_cnt;
		*(ptr + i) = *pos;
		pos++;
	}

#ifdef COUT_IN_CONSOLE
	cout << "开始解码..." << endl;
#endif

	for (size_t i = 0; i < dict_cnt; i++)
	{
		ByteInfo bi;
		bi.value = *pos;
		pos++;
		char length = 0, buffer = 0;
		length = *pos;
		pos++;

		bi.word = "";
		for (int j = 0, cnt = 0; j < length / 8 + 1; j++)
		{
			if (cnt >= length)break;
			buffer = *pos;
			pos++;
			for (int k = 7; k >= 0; k--)
			{
				if (cnt >= length)break;
				bi.word += '0' + ((buffer >> k) & 1);
				cnt++;
			}
		}
		dict[bi.word] = bi.value;
		codeSet.insert(bi.word);
	}

	int resBit;
	for (int i = 0; i < sizeof(resBit); i++)
	{
		char * ptr = (char*)&resBit;
		*(ptr + i) = *pos;
		pos++;
	}

#ifdef COUT_IN_CONSOLE
	cout << "开始写入文件..." << endl;
	cout << "Progress:" << 0 << endl;
#endif

	string s = "";
	int j = 0;
	byte buffer = 0;
	while(pos < rawBytes.end())
	{
#ifdef COUT_IN_CONSOLE
		progress++;
		if (progress / rawBytes.size() > proc + 0.01)
		{
			proc = progress / rawBytes.size();
			cout << "Progress:" << proc << endl;
		}
#endif
		byte i = *pos;
		int th = (pos + 1 == rawBytes.end()) ? resBit : 8;
		while(j < th)
		{
			s += '0' + ((i >> (7 - j)) & 1);
			if (codeSet.find(s) != codeSet.end())
			{
				buffer = dict[s];
				//cout << (char)buffer;
				fout.write((char*)&buffer, sizeof(buffer));
				s.clear();
			}
			j++;
		}
		if (j >= th)
		{
			pos++;
			j -= th;
		}
	}
	fout.close();
#ifdef COUT_IN_CONSOLE
	cout << "写入完成" << endl;
#endif
}

/*void HuffmanHelper::writeTofile(const std::string &filename, ACTION action)
{
	switch (action)
	{
	case HuffmanHelper::ENCODE:
		output_filename = filename;
		writeEncode();
		break;
	case HuffmanHelper::DECODE:
		output_filename = filename;
		writeDecode();
		break;
	default:
		break;
	}
}*/

size_t HuffmanHelper::getFileSize(FILE* file) {
	fseek(file, 0, SEEK_END);
	size_t read_len = ftell(file);
	fseek(file, 0, SEEK_SET);
	return read_len;
}

size_t HuffmanHelper::getFileSize(const char* filePath) {
	FILE* file = fopen(filePath, "rb");
	if (file == nullptr) {
		return 0;
	}
	size_t size = getFileSize(file);
	fclose(file);
	return size;
}

void HuffmanHelper::readFromFile(const char* filePath) {
	rawBytes.clear();
	
	FILE* file = fopen(filePath, "rb");
	if (file == nullptr) {
		return;
	}

	// 获取文件大小，尽量一次读完
	size_t fileSize = getFileSize(file);
	if (fileSize != 0) {
		rawBytes.resize(fileSize);
		size_t n = fread(&rawBytes[0], 1, fileSize, file);
		//assert(n <= fileSize);
		if (n != fileSize) {
			rawBytes.resize(n);
		}
	}

	// 在读取过程当中，有可能文件大小有变化，再尝试读取
	const size_t read_len = 1024;
	char buf[read_len];
	for (;;) {
		size_t n = fread(buf, 1, read_len, file);
		rawBytes.insert(rawBytes.end(), buf, buf + n);
		if (n < read_len) {
			break;
		}
	}
	fclose(file);
}

bool cmpByByte(const ByteInfo &x, const ByteInfo &y)
{
	return x.value < y.value;
}
bool cmpByWord(const ByteInfo &x, const ByteInfo &y)
{
	return x.word < y.word;
}