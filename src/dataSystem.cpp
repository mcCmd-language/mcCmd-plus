#include "dataSystem.h"

Scope::Scope() {
	ClearScope();
}

Scope::~Scope() {
	ClearScope();
}

Variable* Scope::FindVariable(string name) {
	Scope* sc = this;

	while (sc) {
		for (int i = 0; i < sc->variables.size(); i++) {
			Variable* var = sc->variables[i];
			if (!var) continue;

			if (var->GetName().compare(name) == 0) {
				return var;
			}
		}

		sc = sc->GetParent();
	}

	return NULL;
}

void Scope::DefineVariable(Variable* variable) {
	int size = variables.size();
	variables.resize(size + 1);

	variables[size] = variable;
}

void Scope::ClearScope() {
	for (int i = 0; i < commands.size(); i++) {
		delete commands[i];
	}

	for (int i = 0; i < variables.size(); i++) {
		delete variables[i];
	}

	for (int i = 0; i < functions.size(); i++) {
		delete functions[i];
	}

	commands.resize(0);
	variables.resize(0);
	functions.resize(0);
}

bool Scope::IsChild() {
	return false;
}

Scope* Scope::GetParent() {
	return NULL;
}

ChildScope::ChildScope(Scope* parent_) {
	parent = parent_;
}

bool ChildScope::IsChild() {
	return true;
}

Scope* ChildScope::GetParent() {
	return parent;
}

Variable::Variable(string name_) {
	name = name_;

	if (GetType() == VariableType::FUNCTION) {
		scope = new Scope();
	}
	else {
		scope = NULL;
	}
}

void Variable::SetType(enum class VariableType type_) {
	type = type_;
}

Variable::~Variable() {
	if (scope != NULL) delete scope;
}

string Variable::GetName() {
	return name;
}

enum class VariableType Variable::GetType() {
	return type;
}

bool Variable::IsNull() {
	return false;
}

int Variable::GetInt() {
	return intVal;
}

float Variable::GetFloat() {
	return floatVal;
}

double Variable::GetDouble() {
	return doubleVal;
}

string Variable::GetString() {
	return stringVal;
}

bool Variable::GetBool() {
	return boolVal;
}

char* Variable::GetFuncToken() {
	return funcVal;
}

void Variable::SetInt(int vl) {
	intVal = vl;
}

void Variable::SetFloat(float vl) {
	floatVal = vl;
}

void Variable::SetDouble(double vl) {
	doubleVal = vl;
}

void Variable::SetString(string vl) {
	stringVal = vl;
}

void Variable::SetBool(bool vl) {
	boolVal = vl;
}

void Variable::SetFuncToken(char* vl) {
	funcVal = vl;
}
