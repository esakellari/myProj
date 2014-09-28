#ifndef _ASSIGNMENT_H_
#define _ASSIGNMENT_H_
#include <list>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "Converter.h"

using namespace std;


class Assignment {
	private:
		map<string, string> assign;
		vector<string> variables;
		vector<string> assignedvars;
		vector<string> remainingVals;
		map<string , vector<string> > rv; //remaining values
		
		bool everyVariableIsAKeyAndHasAValue();
	public:
		Assignment() {};
		~Assignment() {}
		Assignment(vector<string> v);
		void setAssignment(string variable, string value);
		string getAssignment(string variable);
		bool isComplete();
		void remove(string variable);
		string selectFirstUnassignedVariable();
		bool hasAssignmentFor(string variable);
		void print(int size);
		vector<string> getAssignedVariables();
		vector<string> getRemaining();
		void setRemaining(vector<string> rem);
		vector<string> getVariables();
	
};

#endif
