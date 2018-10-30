#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include "HuffmanTree.h"
#include "ByteIO.h"
using namespace std;

HuffmanTree tree;

vector<ByteInfo> ls;
vector<byte> ByteList;

set<byte> byteSet;
map<byte, int> probs;
map<byte, string> dict;


int main(int argc, char* argv[])
{
	IOHelper* helper = IOHelper::getInstance();
	helper->setOutputFile("output.bin");

	ByteList = helper->readFromFile("news.docx");

	for (auto &i : ByteList)
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
		ls.push_back(b);
	}

	tree.encode(ls);

	for (auto &i : ls)
	{
		dict[i.value] = i.word;
	}

	for (auto &i : ByteList)
	{
		//cout << codes[i] << ' ';
		helper->write(dict[i]);
	}
	helper->writeResidue();

	system("pause");
	return 0;
}