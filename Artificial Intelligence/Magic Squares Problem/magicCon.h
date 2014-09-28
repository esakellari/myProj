#ifndef _MAGIC_CONSTRAINTS_
#define _MAGIC_CONSTRAINTS_
#include "magicCSP.h"
#include "Assignment.h"
#include "Converter.h"
#include "CSP.h"
#include "Domain.h"
#include <stdio.h>


class magicCon : public Constraint {

	private:
		int size;
		int Sum;
        int test;	
		bool existsInVector(string val, vector<string>tmp);
			

	public:
		magicCon(int n);
		~magicCon();

		bool isSatisfiedWith(Assignment *assignment, string variable, string value,int con);
		bool fc(Assignment *assignment, string variable, string value,Domain &d, map<string, vector<string> > *recov ) ;      
};

#endif
