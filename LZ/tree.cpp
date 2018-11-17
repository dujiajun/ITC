#include "tree.h"

using namespace std;

Tree_node* find_child(Tree_node *parent, char c) {
	Tree_node *child = NULL;
	if (parent->first_child != NULL) {
		child = parent->first_child;
	}

	Tree_node* current_sibling = child;
	while (current_sibling != NULL) {
		if (current_sibling->byte == c) {
			return current_sibling;
		}
		current_sibling = current_sibling->sibling;
	}

	return NULL;
}

Tree_node* insert_child(Tree_node *parent, char byte, int label) {
	Tree_node* child = new Tree_node();

	child->byte = byte;
	child->label = label;
	child->parent = parent;
	if (parent == NULL) {
		return child;
	}

	if (parent->first_child != NULL) {
		Tree_node* siblings = parent->first_child;
		while (siblings->sibling != NULL) {
			siblings = siblings->sibling;
		}
		siblings->sibling = child;
	}
	else {
		parent->first_child = child;
	}

	return child;
}

void print_path(Tree_node *last_node) {
	if (last_node->parent != NULL) {
		print_path(last_node->parent);
		cout << last_node->byte;
	}
}

char* path(Tree_node* last_node) {
	Tree_node* current = last_node;

	stack<char> rev_path;
	while (current->byte != '\0') {
		rev_path.push(current->byte);
		current = current->parent;
	}

	vector<char> path;

	while (!rev_path.empty()) {
		path.push_back(rev_path.top());
		rev_path.pop();
	}
	int len = path.size();
	char* path_str = new char[len + 1];

	for (int i = 0; i < path.size(); i++) {
		path_str[i] = path[i];
	}
	path_str[len] = '\0';
	return path_str;
}

void print_tree(Tree_node *root) {
	Tree_node *temp;

	if (root == NULL) return;
	cout << "label " << root->label << ", byte ";
	if (root->byte == '\n') cout << "'\\n'";
	else cout << "'" << root->byte << "'\n";
	if (root->first_child) cout << "  children: ";
	for (temp = root->first_child; temp; temp = temp->sibling) {
		cout << "(" << temp->label << ", ";
		if (temp->byte == '\n') cout << "'\\n') ";
		else cout << "'" << temp->byte << "') ";
	}
	cout << endl;
	for (temp = root->first_child; temp; temp = temp->sibling)
		print_tree(temp);
}
