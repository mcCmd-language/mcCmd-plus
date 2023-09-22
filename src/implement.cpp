#include "implement.h"

enum class VariableKey {
	TYPE,
	VALUE,
};

void Implementing(Scope* scope, vector<Command*> commands) {
	for (int i = 0; i < commands.size(); i++) {
		Command* cmd = commands[i];

		if (cmd->type == CommandType::defineVariable) {
			Variable* var = NULL;

			bool nameParse = true;
			bool keyParse = false;
			bool valueParse = false;

			bool inString = false;

			enum class VariableKey keyType;

			int codeBlock = 0;

			int index = 0;
			for (int j = 0; j < cmd->elements[1].size(); j++) {
				char ch = cmd->elements[1][j];

				if (nameParse) {
					if (ch == '[') {
						nameParse = false;
						keyParse = true;
						index = j + 1;

						string name = cmd->elements[1].substr(0, j);
						cout << name << endl;

						var = new Variable(name);
					}
				}

				if (keyParse) {
					if (ch == '=') {
						keyParse = false;
						valueParse = true;

						string key = cmd->elements[1].substr(index, j - index);

						index = j + 1;

						key.erase(remove_if(key.begin(), key.end(), [](char c) {
						return isspace(c);
							}), key.end());

						cout << key << endl;

						if (key.compare("type") == 0) {
							keyType = VariableKey::TYPE;
							cout << "\nis type\n";
						}
						else if (key.compare("value") == 0) {
							keyType = VariableKey::VALUE;
						}
					}
				}

				if (valueParse) {
					if (ch == '"' && cmd->elements[1][j - 1] != '\\') {
						if (inString) {
							inString = false;
						}
						else {
							inString = true;
						}
					}

					if (ch == '#' && cmd->elements[1][j - 1] != '!' && cmd->elements[1][j + 1] != '#') {
						codeBlock++;
					}

					if (codeBlock && ch == '!' && cmd->elements[1][j + 1] == '#') {
						codeBlock--;
					}

					if (ch == ',' || ch == ']') {
						valueParse = false;
						if (ch != ']') keyParse = true;

						string value = cmd->elements[1].substr(index, j - index);

						index = j + 1;

						cout << value << endl;

						if (keyType == VariableKey::TYPE) {

							if (value.compare("int") == 0) var->SetType(VariableType::INT);
							else if (value.compare("float") == 0) var->SetType(VariableType::FLOAT);
							else if (value.compare("double") == 0) var->SetType(VariableType::DOUBLE);
							else if (value.compare("string") == 0) var->SetType(VariableType::STRING);
							else if (value.compare("boolean") == 0) var->SetType(VariableType::BOOLEAN);
							else if (value.compare("function") == 0) var->SetType(VariableType::FUNCTION);
						}
						else if (keyType == VariableKey::VALUE) {
							if (var->GetType() == VariableType::INT) {
								var->SetInt(atoi(value.c_str()));
							}
							else if (var->GetType() == VariableType::FLOAT) {
								var->SetFloat((float)atof(value.c_str()));
							}
							else if (var->GetType() == VariableType::DOUBLE) {
								var->SetDouble(atof(value.c_str()));
							}
							else if (var->GetType() == VariableType::STRING) {
								string vl = value.substr(1, value.size() - 2);
								cout << vl << endl;
								var->SetString(vl);
							}
							else if (var->GetType() == VariableType::BOOLEAN) {
								if (value.compare("true") == 0) {
									var->SetBool(true);
								}
								else if (value.compare("false") == 0) {
									var->SetBool(false);
								}
								else if (atoi(value.c_str()) <= 0) {
									var->SetBool(false);
								}
								else
									var->SetBool(true);
							}
							else if (var->GetType() == VariableType::FUNCTION) {
								while (isspace(value[0])) {
									value.erase(0, 1);
								}

								char* token = const_cast<char*>(value.c_str());
								var->SetFuncToken(token);
							}
						}
					}
				}
			}

			scope->DefineVariable(var);
		}
	}
}
