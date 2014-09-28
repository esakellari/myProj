#ifndef _DOMAIN_H_
#define _DOMAIN_H_
#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include "Converter.h"

using namespace std;


class Domain {
	private:
		map<string, vector<string> > varDomains; //maps a variable with a list of domain values of this variable	
		map<string, int> domainsize;//map me ta megethi twn domains, dld to poses times exei kathe metavliti
		vector<string> domain;
		vector<string> variables;	
	public:
		Domain(int n);
		~Domain() {}
		vector<string> getDomainOf(string variable);
		void addToDomain(string variable, vector<string> values);
		void check_value(string variable, string value);
		Domain& operator= (const Domain&);
		int removeFromDomain(string value, string variable);
		vector<string> getVariables();
		void print();
		void recover(map<string, vector<string> > recov);
		map <string, int> getDomainsize();
};

#endif
