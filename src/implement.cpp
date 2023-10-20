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
							else {
								cout << "<Define Error at line " + to_string(cmd->line) + "> '" + value + "' is unknown type." << endl;
								return;
							}

							var->hasType = true;
						}
						else if (keyType == VariableKey::VALUE) {
							if (!var->hasType) {
								cout << "<Define Error at line "  + to_string(cmd->line) + "> Set Variable type first!!" << endl;
								return;
							}

							if (value.compare("null") == 0 || value.compare("NULL")) {
								var->SetNull();
							}

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
							else if (key == "refer") {
								arg->reference = true;
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

			function->SetToken(parseToken(token, 0));

			scope->DefineFunction(function);
		}
		else if (cmd->type == CommandType::callFunction) {
			string name = cmd->elements[0];

			name.erase(0, 1);
			Func* function = scope->FindFunction(name);
			if (function == NULL) continue;

			function->GetScope()->parent = scope;

			vector<Variable*> references;

			function->GetScope()->ClearScope();

			int argIndex = 0;
			for (int i = 1; i < cmd->elements.size(); i++) {
				string argOrigin = cmd->elements[i];
				FuncArg* arg = function->args[argIndex];

				if (arg == NULL) continue;

				function->args[argIndex]->ClearVar();

				Variable* var = NULL;

				if (argOrigin[0] == '@') {
					argOrigin.erase(remove_if(argOrigin.begin(), argOrigin.end(), [](char c) {
						return (isspace(c) || c == '@' || c == '*' || c == ';');
						}), argOrigin.end());

					Variable* realVar = scope->FindVariable(argOrigin);
					if (realVar != NULL) {
						if (arg->reference) {
							realVar->asArg.resize(realVar->asArg.size() + 1);
							realVar->asArg[realVar->asArg.size() - 1] = arg->GetName();

							var = realVar;

							references.resize(references.size() + 1);
							references[references.size() - 1] = realVar;

							arg->SetVar(realVar);
						}
						else {
							var = new Variable(arg->GetName());
							var->SetType(arg->GetType());

							realVar->Move(var);

							arg->SetVar(var);
						}
					}
				}
				
				if (var == NULL) {
					var = new Variable(arg->GetName());
					var->SetType(arg->GetType());

					if (arg->GetType() == VariableType::INT) {
						var->SetInt(atoi(argOrigin.c_str()));
					}
					else if (arg->GetType() == VariableType::FLOAT) {
						var->SetFloat(atof(argOrigin.c_str()));
					}
					else if (arg->GetType() == VariableType::DOUBLE) {
						var->SetDouble(atof(argOrigin.c_str()));
					}
					else if (arg->GetType() == VariableType::STRING) {
						var->SetString(argOrigin);
					}
					else if (arg->GetType() == VariableType::BOOLEAN) {
						if (argOrigin.compare("true") == 0) var->SetBool(true);
						else var->SetBool(false);
					}
					else if (arg->GetType() == VariableType::FUNCTION) {
						while (isspace(argOrigin[0])) {
							argOrigin.erase(0, 1);
						}

						string tokenStr = argOrigin.substr(1, argOrigin.size() - 3);
						char* token = const_cast<char*>(tokenStr.c_str());

						var->SetFuncToken(token, function->GetScope());
					}
					else if (arg->GetType() == VariableType::ANY) {}

					function->GetScope()->DefineVariable(var);
					arg->SetVar(var);
				}

				if (var != NULL && var->IsNull()) {
					if (arg->nullable) {
					}
					else {
						cout << "Function '" + function->GetName() + "' is not nullable!" << endl;
						return;
					}
				}

				argIndex++;
			}

			function->Action(function->args);
			Implementing(function->GetScope(), function->token);

			for (int i = 0; i < references.size(); i++) {
				Variable* refer = references[i];

				if (refer->asArg.size() > 0) refer->asArg.resize(refer->asArg.size() - 1);
			}

			function->GetScope()->ClearScope();
		}
	}
}

string varToString(Variable* var) {
	if (var->GetType() == VariableType::STRING) return var->GetString();
	else if (var->GetType() == VariableType::INT) return to_string(var->GetInt());
	else if (var->GetType() == VariableType::FLOAT) return to_string(var->GetFloat());
	else if (var->GetType() == VariableType::DOUBLE) return to_string(var->GetDouble());
	else if (var->GetType() == VariableType::BOOLEAN) {
		if (var->GetBool()) return "true";
		else return "false";
	}
	else if (var->GetType() == VariableType::FUNCTION) {
		return "[function]";
	}

	return "undefined";
}

string patchString(Scope* scope, string str)
{
	bool parseVar = false;
	int parseIndex = 0;
	bool inString = false;

	for (int i = 0; i < str.size(); i++) {
		char ch = str[i];

		if (parseVar) {
			if (ch == ' ') parseVar = false;
			else if (ch == '*' || ch == ';') {
				parseVar = false;

				string varName = str.substr(parseIndex, i - parseIndex);
				Variable* var = scope->FindVariable(varName);

				if (var != NULL) {
					str.erase(parseIndex - 1, i - parseIndex + 2);
					str.insert(parseIndex - 1, varToString(var));

					i -= (i - parseIndex + 2);
				}
				else {

					str.erase(parseIndex - 1, i - parseIndex + 2);
					str.insert(parseIndex - 1, "undefined");

					i -= (i - parseIndex + 2);
				}
			}
		}
		else {
			if (ch == '@') {
				if (i > 0) {
					if (str[i - 1] != '\\') parseVar = true;
				}
				else {
					parseVar = true;
				}

				if (parseVar) parseIndex = i + 1;
			}
		}
	}

	if (parseVar) {
		parseVar = false;

		string varName = str.substr(parseIndex, str.size());
		Variable* var = scope->FindVariable(varName);

		if (var != NULL) {
			str.erase(parseIndex - 1, str.size());
			str.insert(parseIndex - 1, to_string(var->GetInt()));
		}
	}

	inString = false;
	for (int i = 0; i < str.size(); i++) {
		char ch = str[i];

		if (!inString && ch == ';') {
			str.erase(i, 1);

			i--;
		}

		if (ch == '"') {
			if (i > 0) {
				if (str[i - 1] != '\\') {
					str.erase(i, 1);

					i--;

					if (inString) inString = false;
					else inString = true;
				}
			}
			else {
				str.erase(i, 1);

				i--;

				if (inString) inString = false;
				else inString = true;
			}
		}
	}
	return str;
}
