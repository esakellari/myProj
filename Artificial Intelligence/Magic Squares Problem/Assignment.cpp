#include "Assignment.h"

Assignment:: Assignment(vector<string> v) {
	variables = v;
	
	for(int i = 1; i <= v.size(); i++)
	{
		remainingVals.push_back(  convertToString(i) );
	}
}

void Assignment::setAssignment(string variable, string value) {
	assign[variable] = value;
	assignedvars.push_back(variable);
}

string Assignment::getAssignment(string variable) {
	return assign[variable];
}

vector<string> Assignment::getVariables()
{
                return variables;               
}

vector<string> Assignment::getAssignedVariables()
{
               return assignedvars;
}

bool Assignment::isComplete() {
	return everyVariableIsAKeyAndHasAValue();
}

bool Assignment::everyVariableIsAKeyAndHasAValue() {
	int i;
	string temp;
	map<string, string>::iterator p;
	
	for (i = 0; i < variables.size(); i++) {
		string v = variables[i];
		p = assign.find(v);
		
		if(p == assign.end())
			return false;
	}

	return true;
}

void Assignment::remove(string variable) {
	map<string, string>::iterator p;
	string v;

    remainingVals.push_back(assign[variable]);
	
	p = assign.find(variable);
	assign.erase(variable);
	
	
	for(int i = 0; i < assignedvars.size(); i++)
	{
            if(assignedvars[i] == variable)
            assignedvars.erase(assignedvars.begin()+i);
    }
}
vector<string> Assignment::getRemaining()
{
               return remainingVals;
}

void Assignment::setRemaining(vector<string> rem)
{
     remainingVals = rem;
}

string Assignment::selectFirstUnassignedVariable() {
	vector<string>::iterator p;
	map<string, string>::iterator m;
	string temp;

	for (p = variables.begin(); p != variables.end(); p++) {
		temp = *p;
		m = assign.find(temp);
		if (m == assign.end())	//no assignment for variable temp
			return temp;
	}

	return NULL;
}

bool Assignment::hasAssignmentFor(string variable) {
	map<string, string>::iterator m;

	m = assign.find(variable);

	if (m != assign.end())
		return true;
	else
		return false;
}

void Assignment::print(int size)
{
	int i;
	string temp;
	
	for(i = 0; i < variables.size(); i++) {
		temp = assign[variables[i]];
		cout << "variable " << variables[i] << " has value " << temp << endl;
	}

	int count = 0;

	for(i = 0; i < variables.size(); i++) 
	{
		temp = assign[variables[i]];
		cout<<temp<<" ";
		count++;

		if(count == size)
		{
			count = 0;
			cout<<endl;
		}

	}		
}
