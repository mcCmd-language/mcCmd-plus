#include <iostream>
#include <string>
#include <vector>
#include "command.h"

using namespace std;

enum class VariableType {
	INT,
	FLOAT,
	DOUBLE,
	STRING,
	BOOLEAN,
	FUNCTION,
};

using namespace std;

class Variable {
private:
	string name;
	enum class VariableType type;
	class Scope* scope;

	int intVal;
	float floatVal;
	double doubleVal;
	string stringVal;
	bool boolVal;
	char* funcVal;
public:
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
	void SetFuncToken(char* token);
};

class Scope {
private:
	vector<Command*> commands;
public:
	Scope();
	~Scope();

	vector<Variable*> variables;
	vector<Command*> functions;

	Variable* FindVariable(string name);
	void FindFunction(string name);

	void DefineVariable(Variable* variable);
	void DefineFunction();

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

