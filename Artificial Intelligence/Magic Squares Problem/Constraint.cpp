#include "Constraint.h"

void Constraint::putConstraints(string variable, vector<string> values) {	
	constraints[variable].push_back(values);
}

vector<vector<string > > Constraint::getConstraintsOf(string variable) {
	return constraints[variable];
}

bool Constraint::isSatisfiedWith(Assignment *assignment, string variable, string value,int con) {}

bool Constraint::fc(Assignment *assignment, string variable, string value,Domain &d,
                                    map<string, vector<string> > *recov)  {}
                                    

int Constraint::getSum(){}
