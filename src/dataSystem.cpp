#include "dataSystem.h"

Scope::Scope() {
}

Variable* Scope::FindVariable(string name) {
	Scope* sc = this;

	while (sc) {
		for (int i = 0; i < sc->variables.size() - 1; i++) {
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

Func* Scope::FindFunction(string name)
{
	Scope* sc = this;

	name.erase(remove_if(name.begin(), name.end(), [](char c) {
		return isspace(c);
		}), name.end());

	while (sc != NULL) {
		for (int i = 0; i < sc->functions.size(); i++) {
			Func* func = sc->functions[i];
			if (func == NULL) continue;

			if (func->GetName().compare(name) == 0) {
				return func;
			}
		}

		sc = sc->GetParent();
	}

	return NULL;
}

void Scope::DefineVariable(Variable* variable) {
	size_t size = variables.size();
	variables.resize(size + 1);

	variables[size] = variable;
}

void Scope::DefineFunction(Func* function)
{
	size_t size = functions.size();
	functions.resize(size + 1);

	functions[size] = function;
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

	scope = NULL;

	isNull = true;
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
	return isNull;
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

	isNull = false;
}

void Variable::SetFloat(float vl) {
	floatVal = vl;

	isNull = false;
}

void Variable::SetDouble(double vl) {
	doubleVal = vl;

	isNull = false;
}

void Variable::SetString(string vl) {
	stringVal = vl;

	isNull = false;
}

void Variable::SetBool(bool vl) {
	boolVal = vl;

	isNull = false;
}

void Variable::SetFuncToken(char* vl, class Scope* parent) {
	funcVal = vl;
	scope = new ChildScope(parent);
}

void Variable::Move(Variable* var)
{
	var->intVal = intVal;
	var->floatVal = floatVal;
	var->doubleVal = doubleVal;
	var->stringVal = stringVal;
	var->boolVal = boolVal;
	var->funcVal = funcVal;
}

void Variable::SetNull()
{
	intVal = NULL;
	floatVal = NULL;
	doubleVal = NULL;
	boolVal = NULL;
	funcVal = NULL;
	stringVal = "";

	isNull = true;
}

Variable* Variable::Copy() {
	Variable* newVar = new Variable(name);

	newVar->type = type;
	newVar->intVal = intVal;
	newVar->floatVal = floatVal;
	newVar->doubleVal = doubleVal;
	newVar->boolVal = boolVal;
	newVar->stringVal = stringVal;
	newVar->funcVal = funcVal;

	return newVar;
}

FuncArg::FuncArg(string name_)
{
	name = name_;
}

FuncArg::FuncArg(string name_, VariableType type_)
{
	name = name_;
	type = type_;
}

void FuncArg::SetType(VariableType type__)
{
	type = type__;
}

VariableType FuncArg::GetType()
{
	return type;
}

string FuncArg::GetName()
{
	return name;
}

void FuncArg::SetVar(Variable* var_)
{
	var = var_;
}

Variable* FuncArg::GetVar()
{
	return var;
}

void FuncArg::ClearVar()
{
	var = NULL;
}

Func::Func(string name_, class Scope* parent_)
{
	name = name_;
	scope = new ChildScope(parent_);
}

string Func::GetName()
{
	return name;
}

void Func::SetToken(vector<Command*> token_)
{
	token = token_;
}

void Func::SetActon(void(*action_)(vector<FuncArg*>arg, ChildScope* scope))
{
	action = action_;
}

void Func::Action(vector<FuncArg*> arg)
{
	if (scope == NULL) return;

	if (action != NULL) {
		action(arg, scope);
	}
}

ChildScope* Func::GetScope()
{
	return scope;
}

void Func::InsertArg(FuncArg* arg)
{
	int size = args.size();

	args.resize(size + 1);
	args[size] = arg;
}

void Func::Call()
{
}
