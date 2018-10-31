#include <iostream>
#include <cstring>
#include "HuffmanHelper.h"
using namespace std;

int main(int argc, char* argv[])
{
	HuffmanHelper* helper = HuffmanHelper::getInstance();
	if (argc != 4)
		return 0;
	if (strcmp(argv[1], "-encode") == 0)
	{
		helper->encode(argv[2], argv[3]);
	}
	if (strcmp(argv[1], "-decode") == 0)
	{
		helper->decode(argv[2], argv[3]);
	}
	return 0;
}