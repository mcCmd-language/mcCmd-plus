#pragma once
#include <iostream>
#include <vector>
#include "command.h"
using namespace std;

vector<Command*> parseToken(vector<char> token, int line);
void ChildToken(Command* cmd, int tkSize);
