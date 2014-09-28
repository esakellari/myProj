#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_
#include "Assignment.h"
#include "Domain.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class Constraint {
	protected:
		map<string, vector<vector<string > > > constraints;
	public:
		Constraint() {}
		~Constraint() {}
		vector<vector<string > > getConstraintsOf(string variable);
		void putConstraints(string variable, vector<string> values);
		virtual bool isSatisfiedWith(Assignment *assignment, string variable, string value,int con);
		virtual bool fc(Assignment *assignment, string variable, string value,Domain &d,
                                    map<string, vector<string> > *recov) ;
        virtual int getSum();
};

#endif
