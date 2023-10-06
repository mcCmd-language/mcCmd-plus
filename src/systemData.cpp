#include "implement.h"

void Say(vector<FuncArg*> args, ChildScope* scope) {
	Variable* var = args[0]->GetVar();

	if (var == NULL) return;
	if (var->GetType() == VariableType::STRING) {
		cout << args[0]->GetVar()->GetString() << endl;
	}
}

void SetSystemFunc(Scope* mainScope) {

	//say command
	Func* say = new Func("say", mainScope);
	say->InsertArg(new FuncArg("message", VariableType::STRING));
	say->SetActon(Say);

	mainScope->DefineFunction(say);
	cout << "ddfine" << endl;
}