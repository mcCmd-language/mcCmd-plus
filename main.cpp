#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "src/tokenizing.h"
#include "src/command.h"
#include "src/implement.h"

using namespace std;

long fsize(FILE* fp) {
	int prev = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	int sz = ftell(fp);
	fseek(fp, prev, SEEK_SET);
	return sz;
}

int main(int argc, char** argv) {
	FILE* fp;

	//if (argc < 2) {
	//	printf("\nplease enter file path");

	//	return 0;
	//}

	//string filepath = argv[1];
	string filepath = "../../../test/test.mcCmd";

	fp = fopen(filepath.c_str(), "r");

	if (fp == NULL) {
		printf("\n%s doesn't exist", argv[1]);

		return 0;
	}

	int size = fsize(fp);
	vector<char> token(size);

	char c = fgetc(fp);
	int i = 0;
	while (c != EOF) {
		token[i] = c;

		c = fgetc(fp);
		i++;
	}

	vector<Command*> commands = parseToken(token, 1);

	cout << "\n\n------\ncommands: " + to_string(commands.size()) + "\n------\n" << endl;

	Scope* mainScope = new Scope();
	mainScope->id = "main";

	SetSystemFunc(mainScope);
	Implementing(mainScope, commands);
}