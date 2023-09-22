#include "2913.h"

vector<string> split(string str, char Delimiter) {
    istringstream iss(str);
    string buffer;

    vector<string> result;

    while (getline(iss, buffer, Delimiter)) {
        result.push_back(buffer);
    }

    return result;
}
