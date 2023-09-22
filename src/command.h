#pragma once
#include <vector>
#include <string>

using namespace std;

enum class CommandType {
	comment,
	define,
	defineVariable,
	defineFunction,
	operation,
	callFunction,
};

class Command {
public:
	string fullToken;
	CommandType type;
	vector<string> elements;

	Command(enum class CommandType type_);
	void AddElement(string element);
};