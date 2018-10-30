#include"HuffmanTree.h"

void HuffmanTree::generateHuffmanTree()
{
	if (heap.empty())return;
	while (heap.size() > 1)
	{
		Node *a = heap.top();
		heap.pop();
		Node *b = heap.top();
		heap.pop();
		Node *c = new Node();
		c->prob = a->prob + b->prob;
		a->father = c;
		b->father = c;
		if (a->prob > b->prob)
		{
			c->left = a;
			c->right = b;
		}
		else
		{
			c->right = a;
			c->left = b;
		}
		heap.push(c);
	}
	root = heap.top();
	heap.pop();
}

void HuffmanTree::dfs(Node * x, std::string code)
{
	if (x == nullptr)return;
	if (x->info != nullptr)
	{
		x->info->word = code;
	}
	if (x->left != nullptr)dfs(x->left, code + '0');
	if (x->right != nullptr)dfs(x->right, code + '1');
}

void HuffmanTree::deconstruction(Node * &x)
{
	if (x == nullptr)
		return;
	if (x->left != nullptr)deconstruction(x->left);
	if (x->right != nullptr)deconstruction(x->right);
	delete x;
}

void HuffmanTree::insert(ByteInfo &n)
{
	Node * node = new Node(&n);
	node->prob = n.prob;
	heap.push(node);
}

void HuffmanTree::encode(std::vector<ByteInfo> &v)
{

	for (auto &i : v)
	{
		insert(i);
	}
	generateHuffmanTree();
	dfs(root, "");
}

HuffmanTree:: ~HuffmanTree()
{
	deconstruction(root);
}