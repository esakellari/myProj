#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "Object.h"

using namespace std;

class SymbolTable {

	private:
		string name; /* name tis embeleias*/
		bool isGlobal;
	
		map<string,Variable *>*variables;
		map<string,Function *>*functions;
              	map<string,Record *>  *records;

		map<string,Variable *>::iterator itVar;     
                map<string,Function *>::iterator itFun;
                map<string,Record *>  ::iterator itRec;
		

	public:
		SymbolTable(string nam, bool _glob);

                Variable* lookupVariable(string nam);
		Function* lookupFunction(string nam);
		Record * lookupRecord(string nam);

		bool insertVariable(string nam, Variable *var);
		bool insertFunction(string nam, Function *fun);
		bool insertRecord(string nam, Record *rec);

		bool deleteSymbol(string nam){};
};


