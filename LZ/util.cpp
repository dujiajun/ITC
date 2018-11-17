#include "util.h"

using namespace std;

void print_label(FILE* fp, int label, int max_label) {
	static unsigned char buffer = 0;
	static int how_full = 0;
	int mask;

	if (max_label == 0) return;
	for (mask = 1; max_label > 1; max_label /= 2) mask *= 2;

	for (; mask != 0; mask /= 2) {
		buffer = buffer * 2 + ((label & mask) / mask);
		++how_full;
		if (how_full == 8) {
			fwrite(&buffer, sizeof(unsigned char), 1, fp);
			buffer = 0;
			how_full = 0;
		}
	}
}

void print_letter(FILE* fp, char c) {
	print_label(fp, (unsigned char)c, 128);
}

void print_final_label(FILE* fp, int label, int max_label) {
	print_label(fp, label, max_label);
	print_label(fp, 0, 127);
}

int read_label(FILE* fp, int max_label) {
	static int buffer;
	static int how_full = 0;
	int label;

	for (label = 0; max_label != 0; max_label /= 2) {
		if (how_full == 0) {
			buffer = fgetc(fp);
			if (buffer == EOF) return -1;
			how_full = 8;
		}
		label = label * 2 + ((buffer & 128) / 128);
		buffer *= 2;
		how_full--;
	}
	return label;

}

int read_letter(FILE* fp, char &c) {
	int val = read_label(fp, 128);
	if (val == -1) return 0;
	c = (char)val;
	return 1;
}

size_t getFileSize(FILE* fp) {
	fseek(fp, 0, SEEK_END);
	size_t read_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return read_len;
}