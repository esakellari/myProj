#ifndef _CSP_H_
#define _CSP_H_
#include "Constraint.h"
#include "Assignment.h"
#include "Domain.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>

 //BT/ MRV/ FC


using namespace std;

class CSP {
    	private:
        		Assignment* recursiveBackTrackingSearch(Assignment* anAssignment, Constraint &m, int n,Domain &d); 
			int nodes;   
			int checks;
	protected:
			map<string, vector<string> > varDomains;
			vector<string> variables;
			vector<string> domain;   
	public:
			CSP() {}
			~CSP() {}
		
			vector<string> getDomainOf(string variable);
			void addToDomain(string variable, vector<string> values);
			Assignment* backTrackingSearch(Constraint &m,int n);
			string mrv(Domain &d,Assignment *anAssignment, Constraint &c);
};

#endif
