#include <iostream>
#include <cstring>
#include <string>
#include <windows.h>
#include "HuffmanHelper.h"

using namespace std;

int main(int argc, char* argv[])
{
	HuffmanHelper* helper = HuffmanHelper::getInstance();
	if (argc != 4)
	{
		int i;
		string input, output;
		cin >> i >> input >> output;
		if (i == 1)
			helper->encode(input, output);
		else if (i == 2)
			helper->decode(input, output);
		system("pause");
		return 0;
	}
		
#ifdef COUT_IN_CONSOLE
	DWORD t1 = GetTickCount64();
#endif // COUT_IN_CONSOLE

	if (strcmp(argv[1], "-encode") == 0)
	{
		helper->encode(argv[2], argv[3]);
	}
	if (strcmp(argv[1], "-decode") == 0)
	{
		helper->decode(argv[2], argv[3]);
	}
#ifdef COUT_IN_CONSOLE
	DWORD t2 = GetTickCount64();
	cout << (t2 - t1) << "ms" << endl;
#endif // COUT_IN_CONSOLE
	return 0;
}