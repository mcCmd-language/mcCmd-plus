#include "implement.h"
#include "tokenizing.h"

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


						if (key.compare("type") == 0) {
							keyType = VariableKey::TYPE;
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


						if (keyType == VariableKey::TYPE) {

							if (value.compare("int") == 0) var->SetType(VariableType::INT);
							else if (value.compare("float") == 0) var->SetType(VariableType::FLOAT);
							else if (value.compare("double") == 0) var->SetType(VariableType::DOUBLE);
							else if (value.compare("string") == 0) var->SetType(VariableType::STRING);
							else if (value.compare("bool") == 0) var->SetType(VariableType::BOOLEAN);
							else if (value.compare("boolean") == 0) var->SetType(VariableType::BOOLEAN);
							else if (value.compare("func") == 0) var->SetType(VariableType::FUNCTION);
							else if (value.compare("function") == 0) var->SetType(VariableType::FUNCTION);
						}
						else if (keyType == VariableKey::VALUE) {
							if (value.compare("null") == 0 || value.compare("NULL")) {
								var->SetNull();
							}
							else if (var->GetType() == VariableType::INT) {
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

								string tokenStr = value.substr(1, value.size() - 3);
								char* token = const_cast<char*>(tokenStr.c_str());

								var->SetFuncToken(token, scope);
							}
						}
					}
				}
			}

			scope->DefineVariable(var);
		}
		else if (cmd->type == CommandType::defineFunction) {
			Func* function;

			int index = 0;

			string name = cmd->elements[1].substr(1);
			cout << name << endl;

			function = new Func(name, scope);

			for (int i = 2; i < cmd->elements.size() - 1; i++) {
				string element = cmd->elements[i];

				bool nameEnd = false;
				bool inInfo = false;
				int start = 0;
				int end = element.size() - 1;

				string key;
				bool parsingVal = false;
				int valIndex = 0;

				bool checkInfo = false;

				FuncArg* arg = NULL;

				for (int j = 0; j < element.size(); j++) {
					char ch = element[j];

					if (nameEnd) {
						if (ch == '[') {
							inInfo = true;
							end = j;
							valIndex = j + 1;
						}

						if (inInfo) {
							if (parsingVal) {
								if (ch == ',') {
									parsingVal = false;
									checkInfo = true;
								}
							} else {
								if (ch == '=') {
									parsingVal = true;

									key = element.substr(valIndex, j - valIndex);

									valIndex = j + 1;
								}
							}

							if (ch == ']') {
								inInfo = false;
								if (parsingVal) {
									checkInfo = true;
								}
							}
						}

						if (checkInfo) {
							checkInfo = false;

							valIndex = j + 1;

							string value = element.substr(valIndex, j - valIndex);

							if (key == "nullable") {
								if (value.compare("true")) arg->nullable = true;
								else arg->nullable = false;
							}
							else if (key == "default") {
								arg->def = &value;
							}
						}
					}
					else {
						if (ch == '@') {
							nameEnd = true;

							string name = element.substr(0, j);

							arg = new FuncArg(name);

							start = j + 1;
						}
					}
				}

				if (arg == NULL) continue;

				string typeName = element.substr(start, end - start);

				if (typeName.compare("int") == 0) {
					arg->SetType(VariableType::INT);
				}
				else if (typeName.compare("float") == 0) {
					arg->SetType(VariableType::FLOAT);
				}
				else if (typeName.compare("double") == 0) {
					arg->SetType(VariableType::DOUBLE);
				}
				else if (typeName.compare("string") == 0) {
					arg->SetType(VariableType::STRING);
				}
				else if (typeName.compare("bool") == 0 || typeName.compare("boolean") == 0) {
					arg->SetType(VariableType::BOOLEAN);
				}
				else if (typeName.compare("func") == 0 || typeName.compare("function") == 0) {
					arg->SetType(VariableType::FUNCTION);
				}

				function->InsertArg(arg);
			}

			string last = cmd->elements[cmd->elements.size() - 1];

			while (isspace(last[0])) {
				last.erase(0, 1);
			}

			string tokenStr = last.substr(1, last.size() - 3);
			vector<char> token(tokenStr.begin(), tokenStr.end());

			function->SetToken(parseToken(token));

			scope->DefineFunction(function);
		}
	}
}
