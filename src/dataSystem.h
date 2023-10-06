#include <iostream>
#include <string>
#include <vector>
#include "command.h"

using namespace std;

enum class VariableType {
	INT,
	STRING,
	FLOAT,
	DOUBLE,
	BOOLEAN,
	FUNCTION,
};

using namespace std;

class Variable {
private:
	string name;
	enum class VariableType type;
	class ChildScope* scope;

	int intVal;
	float floatVal;
	double doubleVal;
	string stringVal;
	bool boolVal;
	char* funcVal;

	bool isNull;
public:
	vector<string> asArg;
	Variable(string name_);
	~Variable();

	void SetType(enum class VariableType type_);
	enum class VariableType GetType();
	bool IsNull();

	string GetName();

	int GetInt();
	string GetString();
	float GetFloat();
	double GetDouble();
	bool GetBool();
	char* GetFuncToken();

	void SetInt(int in);
	void SetString(string str);
	void SetFloat(float fl);
	void SetDouble(double dou);
	void SetBool(bool boolean);
	void SetFuncToken(char* vl, class Scope* parent);
	void Move(Variable* var);
	void SetNull();

	Variable* Copy();
};

class FuncArg {
private:
	string name;
	enum class VariableType type;

	Variable* var;
public:
	bool nullable = false;
	bool reference = true;
	void* def;

	FuncArg(string name_);
	FuncArg(string name_, enum class VariableType type_);

	void SetType(enum class VariableType type__);
	enum class VariableType GetType();
	string GetName();
	void SetVar(Variable* var_);
	Variable* GetVar();

	void ClearVar();
};

class Func {
private:
	string name;
	class ChildScope* scope;

	void (*action)(vector<FuncArg*> arg, class ChildScope* scope) = NULL;
public:
	vector<Command*> token;
	vector<FuncArg*> args;

	Func(string name_, class Scope* parent_);

	string GetName();
	void SetToken(vector<Command*> token_);
	void SetActon(void (*action_)(vector<FuncArg*> arg, class ChildScope* scope));
	void Action(vector<FuncArg*> arg);
	class ChildScope* GetScope();
	void InsertArg(FuncArg* arg);

	void Call();
};

class Scope {
private:
	vector<Command*> commands;
public:
	Scope();

	vector<Variable*> variables;
	vector<Func*> functions;

	Variable* FindVariable(string name);
	Func* FindFunction(string name);

	void DefineVariable(Variable* variable);
	void DefineFunction(Func* function);

	vector<Scope*> GetChildScopes();
	void ClearScope();

	virtual bool IsChild();
	virtual Scope* GetParent();
};

class ChildScope : public Scope {
private:
	Scope* parent;
public:
	ChildScope(Scope* parent);

	bool IsChild() override;
	Scope* GetParent() override;
};

