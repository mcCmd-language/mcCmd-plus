#include <iostream>
#include <vector>
#include <string>
#include "dataSystem.h"
#include "command.h"

void SetSystemFunc(Scope* mainScope);
void Implementing(Scope* mainScope, vector<Command*> commands);
string patchString(Scope* scope, string str);
string varToString(Variable* var);
