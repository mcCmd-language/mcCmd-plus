#include "command.h"
#include <iostream>

Command::Command(enum class CommandType type_) {
	type = type_;
	elements.resize(0);
}

void Command::AddElement(string element) {
	elements.resize(elements.size() + 1);

	elements[elements.size() - 1] = element;
}