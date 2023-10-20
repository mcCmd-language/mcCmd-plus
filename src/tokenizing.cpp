#include "tokenizing.h"

vector<Command*> parseToken(vector<char> token, int line = 1) {
	vector<Command*> parsed;

	bool tokenLogging = false;

	int parsedSize = 0;

	string fullToken(token.begin(), token.end());

	bool cmdParsing = false;
	int inCodeBlock = false;
	int parsePoint = 0;

	int setInfo = false;
	bool inString = false;
	Command* cmd;
	for (int i = 0; i < token.size(); i++) {
		if (i >= token.size() - 1) continue;
		char ch = token[i];
		char chN = token[i + 1];

		if (ch == '\n') line++;

		if (ch == '"') {
			if (i > 0) {
				if (token[i - 1] != '\\') inString = !inString;
			} else inString = !inString;
		}

		if (!inString) {
			if (ch == '#' && chN == '#') {
				while (token[i] != '\n') {
					i++;
				}
			}
		}

		if (!cmdParsing) {
			if (ch == '@') {
				cmdParsing = true;
				parsePoint = i;
				cmd = new Command(CommandType::define, line);
			}
			else if (ch == '/') {
				cmdParsing = true;
				parsePoint = i;
				cmd = new Command(CommandType::callFunction, line);
			}
		}
		else {
			bool endToken = false;
			if (cmd->type == CommandType::define) {
				if (i == parsePoint + 2) {
					if (ch == '/') {
						cmd->type = CommandType::defineFunction;
					}
					else if (ch != ' ') {
						cmd->type = CommandType::defineVariable;
					}
				}

				if (ch == ';') {
					endToken = true;
				}
			}
			else if (cmd->type == CommandType::defineVariable) {

				if (ch == '[') {
					setInfo = true;
				}
				if (ch == ']') {
					setInfo = false;
				}

				if (ch == ';' && !setInfo) {
					endToken = true;
				}
			}
			else if (cmd->type == CommandType::defineFunction || cmd->type == CommandType::callFunction) {
				if (token[i - 1] != '!' && ch == '#' && chN != '#') {
					inCodeBlock++;
				}

				if (inCodeBlock && ch == '!' && chN == '#') {
					inCodeBlock--;
				}

				if (ch == '[') {
					setInfo = true;
				}
				if (ch == ']') {
					setInfo = false;
				}

				if (!inCodeBlock && !setInfo) {
					if (ch == ';') {
						endToken = true;
					}
				}
			}

			if (endToken) {
				cmdParsing = false;

				int tkSize = i - parsePoint + 1;
				cmd->fullToken = fullToken.substr(parsePoint, tkSize);

				ChildToken(cmd, tkSize);

				int size = parsed.size();
				parsed.resize(size + 1);
				parsed[size] = cmd;

				if (tokenLogging) {
					cout << "\n////////\n\n";
					for (int a = 0; a < cmd->elements.size(); a++) {
						cout << "-" + cmd->elements[a] + "\n";
					}
					cout << to_string(cmd->elements.size()) + "\n";
					cout << "\n\n////////\n";
				}
			}
		}
	}

	return parsed;
}

void ChildToken(Command* cmd, int tkSize) {
	bool tokening = false;
	int index = 0;


	bool string_ = false;
	int codeBlock = 0;
	bool inInfo = false;

	for (int j = 0; j < tkSize; j++) {
		char ch = cmd->fullToken[j];

		if (!tokening) {
			if (ch != ' ') {
				tokening = true;
				index = j;
			}
		}

		if (tokening) {
			if (!string_) {
				if (ch == '[') {
					inInfo = true;
				}
				else if (ch == ']') {
					inInfo = false;
				}
			}

			if (ch == '"' && cmd->fullToken[j - 1] != '\\') {
				if (string_) {
					string_ = false;
				}
				else {
					string_ = true;
				}

			}

			if (j < tkSize - 1 && j > 1 && cmd->fullToken[j - 1] != '!' && ch == '#' && cmd->fullToken[j + 1] != '#') {
				codeBlock++;
			}

			if (j < tkSize - 1 && codeBlock && ch == '!' && cmd->fullToken[j + 1] == '#') {
				codeBlock--;
			}

			if (!string_ && !codeBlock && (ch == ' ' || ch == ';') && !inInfo) {
				tokening = false;
				string_ = false;
				codeBlock = 0;

				bool notEmpty = false;

				for (int k = 0; k < j - index + 1; k++) {
					if (cmd->fullToken[index + k] != ' '
						|| cmd->fullToken[index + k] != '\n'
						|| cmd->fullToken[index + k] != '\t'
						) {
						notEmpty = true;
						break;
					}
				}

				if (notEmpty && j - index + 1 > 1) {

					string element = cmd->fullToken.substr(index, j - index + 1);

					cmd->AddElement(element);
				}
			}
		}
	}
}
