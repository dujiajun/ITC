#include "LZ.h"
#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

int main(int argc,char* argv[])
{
	if (argc != 4)
	{
		int i;
		string input, output;
		cin >> i >> input >> output;
		if (i == 1)
			compress(input.c_str(), output.c_str());
		else if (i == 2)
			decompress(input.c_str(), output.c_str());
		system("pause");
		return 0;
	}
	DWORD dwGTCBegin = GetTickCount64();
	if (strcmp(argv[1], "-encode") == 0)
	{
		compress(argv[2], argv[3]);
	}
	if (strcmp(argv[1], "-decode") == 0)
	{
		decompress(argv[2], argv[3]);
	}
	DWORD dwGTCEnd = GetTickCount64();
	cout  << (dwGTCEnd - dwGTCBegin) << "ms" << endl;
	return 0;
}
