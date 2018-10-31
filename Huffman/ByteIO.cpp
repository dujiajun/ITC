#include"ByteIO.h"
using namespace std;

static size_t getFileSize(FILE* file) {
	fseek(file, 0, SEEK_END);
	size_t read_len = ftell(file);
	fseek(file, 0, SEEK_SET);
	return read_len;
}

static size_t getFileSize(const char* filePath) {
	FILE* file = fopen(filePath, "rb");
	if (file == nullptr) {
		return 0;
	}
	size_t size = getFileSize(file);
	fclose(file);
	return size;
}

static vector<byte> readFromFile(const char* filePath) {
	FILE* file = fopen(filePath, "rb");
	std::vector<byte> result;
	if (file == nullptr) {
		return result;
	}

	// 获取文件大小，尽量一次读完
	size_t fileSize = getFileSize(file);
	if (fileSize != 0) {
		result.resize(fileSize);
		size_t n = fread(&result[0], 1, fileSize, file);
		//assert(n <= fileSize);
		if (n != fileSize) {
			result.resize(n);
		}
	}

	// 在读取过程当中，有可能文件大小有变化，再尝试读取
	const size_t read_len = 1024;
	char buf[read_len];
	for (;;) {
		size_t n = fread(buf, 1, read_len, file);
		result.insert(result.end(), buf, buf + n);
		if (n < read_len) {
			break;
		}
	}
	fclose(file);
	return result;
}

IOHelper::~IOHelper()
{
	fout.close();
}

void IOHelper::write(const std::string &x)
{
	for (auto &j : x)
	{
		if (cnt < 8)
		{
			buffer = (buffer << 1) + (j - '0');
			cnt++;
		}
		if (cnt == 8)
		{
			fout.write((char*)&buffer, sizeof(buffer));
			fout.flush();
			cnt = 0;
			buffer = 0;
		}

	}
}

void IOHelper::write(const std::vector<std::string>& x)
{
	for (auto &i : x)
	{
		write(i);
	}
}

void IOHelper::writeResidue()
{
	if (cnt != 0)
	{
		buffer = buffer << (8 - cnt);
		fout.write((char*)&buffer, sizeof(buffer));
		fout.flush();
		cnt = 0;
		buffer = 0;
	}
}

void IOHelper::setOutputFile(const char * filename)
{
	fout.open(filename, ios::out | ios::binary);
}


vector<byte> IOHelper::readFromFile(const char* filePath) {
	FILE* file = fopen(filePath, "rb");
	std::vector<byte> result;
	if (file == nullptr) {
		return result;
	}

	// 获取文件大小，尽量一次读完
	size_t fileSize = getFileSize(file);
	if (fileSize != 0) {
		result.resize(fileSize);
		size_t n = fread(&result[0], 1, fileSize, file);
		//assert(n <= fileSize);
		if (n != fileSize) {
			result.resize(n);
		}
	}

	// 在读取过程当中，有可能文件大小有变化，再尝试读取
	const size_t read_len = 1024;
	char buf[read_len];
	for (;;) {
		size_t n = fread(buf, 1, read_len, file);
		result.insert(result.end(), buf, buf + n);
		if (n < read_len) {
			break;
		}
	}
	fclose(file);
	return result;
}
