#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "Object.h"
#include "SymbolTable.h"

using namespace std;


SymbolTable :: SymbolTable(string nam,bool _glob):isGlobal(_glob),name(nam) {

	variables = new map<string,Variable *>();
	records   = new map<string,Record *>();
	if(isGlobal) {
		functions = new map<string,Function *>();
		
	}
	else {
		functions = NULL;
		
	}
}



Variable* SymbolTable:: lookupVariable(string nam) {
	
	itVar =  variables->find(nam);

	if (itVar == variables->end()) {
		return NULL;
	}
	return (itVar->second);
}

Function* SymbolTable:: lookupFunction(string nam) {

	itFun = functions->find(nam);
	if (itFun == functions->end() ) {
		return NULL;
	}
	return (itFun->second);
}

Record* SymbolTable:: lookupRecord(string nam) {

	itRec = records->find(nam);
	if (itRec == records->end() ) {
		return NULL;
	}
	return (itRec->second);

}


bool SymbolTable:: insertVariable(string nam, Variable *var){
	if (var != NULL) {
		(*variables)[nam] = var ;
		return true;
	}
	return false;
}

bool SymbolTable:: insertFunction(string nam, Function *fun){
	if (fun != NULL) {
	        (*functions)[nam] = fun ;
		return true;
	}
	return false;
}

bool SymbolTable:: insertRecord(string nam, Record *rec){
	if (rec != NULL) {
		(*records)[nam] = rec;
		return true;
	}
	return false;
}


