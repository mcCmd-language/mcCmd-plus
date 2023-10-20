#include "implement.h"

void Say(vector<FuncArg*> args, ChildScope* scope) {
	Variable* var = args[0]->GetVar();

	if (var == NULL) return;
	cout << patchString(scope, varToString(var)) << endl;
}

void ReplaceItem(vector<FuncArg*> args, ChildScope* scope) {
	Variable* var = args[0]->GetVar();
	if (var == NULL) return;

	if (var->GetType() == VariableType::INT) {
		int input;
		cin >> input;

		var->SetInt(input);
	}
	else if (var->GetType() == VariableType::FLOAT) {
		float input;
		cin >> input;

		var->SetFloat(input);
	}
	else if (var->GetType() == VariableType::DOUBLE) {
		double input;
		cin >> input;

		var->SetDouble(input);
	}
	else if (var->GetType() == VariableType::STRING) {
		string input;
		cin >> input;

		var->SetString(input);
	}
	else if (var->GetType() == VariableType::BOOLEAN) {
		bool input;
		cin >> input;

		var->SetBool(input);
	}
}

void SetSystemFunc(Scope* mainScope) {

	//say command
	Func* say = new Func("say", mainScope);
	say->InsertArg(new FuncArg("message", VariableType::STRING));
	say->SetActon(Say);

	mainScope->DefineFunction(say);

	//replaceitem
	Func* replaceitem = new Func("replaceitem", mainScope);
	FuncArg* replaceArg = new FuncArg("inputVar", VariableType::ANY);
	replaceArg->reference = true;
	replaceArg->nullable = true;
	replaceitem->InsertArg(replaceArg);
	replaceitem->SetActon(ReplaceItem);

	mainScope->DefineFunction(replaceitem);
}