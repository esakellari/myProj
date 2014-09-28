#include <iostream>
#include <string>
#include <vector>

#include "Object.h"

using namespace std;


Variable:: Variable(int _type,string _name,int _size,bool _byRef,bool Array ,int _ArraySize) 
	:Parameter()
{ 
	byRef = _byRef;
	type = _type;
	name = _name;
	size = _size;
	ArrayVar = Array;
	ArraySize = _ArraySize;
}

string Variable:: getName() 
{
	return name;
}

int Variable:: getType() const 
{
	return type;
}

void Variable:: setType(int newtype) 
{
	type = newtype;
}

bool Variable:: isArray() 
{
	return ArrayVar;
}

bool Variable:: isByref()
{
	return byRef;
}		

int Variable:: getArraySize() 
{
	return ArraySize;
}


int Variable::getKind()
{
	return VARIABLE;
}
	
Variable::~Variable() {  }


Function:: Function(int _type,string _recname,string _name,vector<Parameter *> *params) 
{
	recName = _recname;
	parameters = new vector<Parameter *>();
	if (params != NULL) {
		for(int i = 0; i < params->size(); i++) {
			parameters->push_back((*params)[i]);
		}
	}
	type = _type;
	name = _name;
}

string Function::getRecName() {
	return recName;
}


string Function::TakeParam(int x) {
	for (int i = parameters->size()-1; i >= 0 ; i--) {	
		if ( i == x) {
			string str = "(";
			int ptype = (*parameters)[i]->getType();
			if (ptype == INT) 
				str += "int ";
			else if(ptype == FLOAT)
				str += "float ";
			else if(ptype == CHAR)
				str += "char ";
			else if(ptype == STRING)
				str += "string ";
			else if(ptype == RECORD)
				str += "record "+(*parameters)[i]->getRecordName()+" ";
			if ((*parameters)[i]->isByref())
				str += "& ";
			str += (*parameters)[i]->getName();
			if ((*parameters)[i]->isArray())
				str += "[]";
			str += ")";
			return str;
		}
	}

}

string Function::TakePrototypeString() {
	string str = "(Prototype) : ";

	if (type == INT)
		str += "int";
	else if(type == FLOAT)
		str += "float";
	else if(type == CHAR)
		str += "char";
	else if(type == STRING)
		str += "string";
	else if(type == VOID)
		str += "void";
	else if(type == RECORD)
		str += "record "+getRecName();
	
	str += " "+name+"(";
	if (parameters != NULL) {
		for (int i = parameters->size()-1; i >= 0 ; i--) {
			int ptype = (*parameters)[i]->getType();
			if (ptype == INT) 
				str += "int";
			else if(ptype == FLOAT)
				str += "float";
			else if(ptype == CHAR)
				str += "char";
			else if(ptype == STRING)
				str += "string";
			else if(ptype == RECORD)
				str += "record "+(*parameters)[i]->getRecordName();
			if ((*parameters)[i]->isByref())
				str += " &";
			if ((*parameters)[i]->isArray())
				str += "[]";
			if (i-1 >= 0)
				str += ", ";
		}
	}
	str += ")";
	return str;
}

int Function:: getNumParams() {
	if (parameters != NULL)
		return parameters->size();
	return 0;
}

int Function::getType() {
	return type;
}

vector<Parameter *> * Function::getParams()
{
	return parameters;
}


Record::Record(string _name,string _recname, vector<Variable *> *memb, bool _byRef,bool _isArray, int _size)  
	: Parameter()
{
	RecordName = _recname;
	members = new vector<Variable *>();
	name = _name;
	size = _size;
	isArray = _isArray;
	byRef = _byRef;
	if (memb != NULL) {
		for (int i = 0 ; i < memb->size(); i++) 
			members->push_back((*memb)[i]);
	}
}
		
void Record::AddMember( Variable * newMember)
{
	members->push_back(newMember);
}
		

Variable* Record::searchMember(string nam) 
{
	for (int i =0; i < members->size(); i++) {
		if ( (*members)[i]->getName() == nam) {
			return (*members)[i];
		}
	}
	return NULL;
}

vector<Variable*> * Record::getMembers() {
	return members;
}

int Record::getType() const{
	return RECORD;
}

string Record::getName()
{
	return name;
}

string  Record::getRecordName()
{
	return RecordName;
}

int  Record::getSize()
{
	return size;
}

bool Record::isArrayRec()
{
	return isArray;
}

int Record::getKind()
{

	return RECORD;
}


bool Record::isByref()
{
	return byRef;
}
