#ifndef _TREENODE_
#define _TREENODE_

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "Object.h"
#include "SymbolTable.h"

using namespace std;

class treeNode{
      
      private:
		treeNode* father;
                vector<treeNode *> children;
                SymbolTable *symbolTable;
		string Name; // ena onoma gia tin embeleia
      public:
		treeNode(string nam, bool _glob);
              treeNode *insertNode(string nam);
		void setName(string nam);
  		treeNode *getFather();
		void setFather(treeNode *);
		vector<treeNode *> getChildren();

		Variable* lookupVariable(int mode,string name);
		Function* lookupFunction(string name);
		Record*   lookupRecord(int mode, string name);

		bool insertVariable(string name,Variable *var);
		bool insertFunction(string name,Function *fun);
		bool insertRecord(string name,Record *rec);
      };

#endif
