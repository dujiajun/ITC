#include "tree.h"
#include "LZ.h"
#include "util.h"

using namespace std;

void compress(const char* input_filename, const char* output_filename)
{
	FILE* input_file = fopen(input_filename, "rb"); //以二进制形式打开文件
	if (input_file == NULL) {
		cout << "File not found." << endl;
		return;
	}

	double filesize = getFileSize(input_file);
	double progress = 0, proc = 0;

	FILE* output_file = fopen(output_filename, "wb");

	Tree_node* root = new Tree_node();

	char ch;
	int ch_int;
	Tree_node* current = root;
	int label = 0;

	cout << "Progress:" << 0 << endl;

	while ((ch_int = fgetc(input_file)) != -1)
	{
		progress++;
		if (progress / filesize > proc + 0.01)
		{
			proc = progress / filesize;
			cout << "Progress:" << proc << endl;
		}

		ch = (char)ch_int;

		if (find_child(current, ch) != NULL)
		{
			current = find_child(current, ch);
		}
		else
		{
			print_label(output_file, current->label, label);

			print_letter(output_file, ch);

			Tree_node* child = insert_child(current, ch, ++label);

			current = root;
		}
	}

	print_final_label(output_file, current->label, label);

	cout << "Progress:" << 1 << endl;

	fclose(input_file);
	fclose(output_file);
}

void decompress(const char* input_filename, const char* output_filename)
{
	FILE* input_fp = fopen(input_filename, "rb");
	if (input_fp == NULL) {
		cerr << "File not found\n";
		return;
	}

	double filesize = getFileSize(input_fp);
	double progress = 0, proc = 0;

	FILE* output_fp = fopen(output_filename, "wb");

	Tree_node* root = new Tree_node();

	char ch;
	Tree_node* current = root;
	int p_index;
	int label = 0;
	vector<Tree_node*> tree;
	tree.push_back(root);

	cout << "Progress:" << 0 << endl;

	while ((p_index = read_label(input_fp, label)) >= 0)
	{
		progress++;
		if (progress / filesize > proc + 0.01)
		{
			proc = progress / filesize;
			cout << "Progress:" << proc << endl;
		}

		fprintf(output_fp, "%s", path(tree[p_index]));

		int ch_int = read_letter(input_fp, ch);
		if (!ch_int) break;

		fprintf(output_fp, "%c", ch);
		Tree_node* current = insert_child(tree[p_index], ch, ++label);
		tree.push_back(current);
	}
	cout << "Progress:" << 1 << endl;
	fclose(input_fp);
	fclose(output_fp);
}