#include <iostream>
#include <vector>
#include "Node.h"

using namespace std;



treeNode:: treeNode(string nam,bool _glob) {
	father = NULL;
	Name = nam;
	symbolTable = new SymbolTable(Name,_glob);
}

treeNode* treeNode::insertNode(string nam) {
	treeNode *newNode = new treeNode(nam,0);
	children.push_back(newNode);
	newNode->setFather(this);
	return newNode;
}

void treeNode::setName(string nam) {
	Name = nam;
}

void treeNode::setFather(treeNode *fath) {
	father = fath;
}

treeNode* treeNode::getFather() {
	return father;
}

vector<treeNode *> treeNode::getChildren() {
	return children;
}


Variable* treeNode::lookupVariable(int mode,string name){


	Variable *temp = symbolTable->lookupVariable(name);

	if ( mode == 1 ) { // psaxnw gia dilwsi
		return temp;
	}
	else { // an psaxnw gia xrisi
		if ( temp == NULL ) {

			treeNode *father = this->getFather();
			if(father != NULL)
				return father->lookupVariable(mode,name);
			else 
				return NULL;
					
		}
		else {
			return temp;
		}
	}
}



Function* treeNode::lookupFunction(string name){

        return symbolTable->lookupFunction(name);     
}


Record* treeNode::lookupRecord(int mode,string name){


        Record *temp = symbolTable->lookupRecord(name);

        if ( mode == 1 ) { // psaxnw gia dilwsi
                return temp;
        }
        else { // an psaxnw gia xrisi
                if ( temp == NULL ) {

                        treeNode *father = this->getFather();
                        if(father != NULL)
                                return father->lookupRecord(mode,name);
                        else
                                return NULL;

                }
                else {
                        return temp;
                }
        }
}


bool treeNode::insertVariable(string name,Variable *var) {
	
	return symbolTable->insertVariable(name,var);
}

bool treeNode::insertFunction(string name,Function *fun) {

        return symbolTable->insertFunction(name,fun);
}

bool treeNode::insertRecord(string name,Record *rec) {

        return symbolTable->insertRecord(name,rec);
}







