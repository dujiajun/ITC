#pragma once

#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include<vector>
#include<queue>
#include"DataType.h"

class HuffmanTree
{
private:
	struct Node
	{
		ByteInfo * info;
		Node * father;
		Node * left;
		Node * right;
		int prob;

		Node(ByteInfo * p = nullptr)
		{
			info = p;
			father = nullptr;
			left = nullptr;
			right = nullptr;
			prob = 0;
		}
	};

	struct cmp
	{
		bool operator()(Node* &a, Node* &b)const
		{
			return (a->prob) > (b->prob);
		}
	};
	Node* root;
	std::priority_queue<Node*, std::vector<Node*>, cmp > heap;
	void generateHuffmanTree();
	void dfs(Node * x, std::string code);
	void deconstruction(Node * &x);
	void insert(ByteInfo &n);
public:
	HuffmanTree() :root(nullptr) {}
	void encode(std::vector<ByteInfo> &v);
	~HuffmanTree();
};
#endif // HUFFMANTREE_H
