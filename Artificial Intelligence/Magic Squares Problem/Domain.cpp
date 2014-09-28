#include "Domain.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

Domain::Domain(int n)
{
	//aarxika gia kathe mia metavliti krataw to domain tis, to opoio arxika einai
	//oles oi times 

	string  variable;
	
	for(int i = 0; i < n ; i++)
	{
		for(int j = 0; j < n; j++)
		{	
	
			string rows = convertToString(i);
			string colu = convertToString(j);

			variable = rows + "_" + colu;

	
	

			
			variables.push_back(variable);
			

		}

		
	}


	for(int i = 0; i < n*n; i++)
	{
		string num = convertToString(i+1);
		domain.push_back(num);
			
	}

	for (int i = 0; i < variables.size(); i++)
		addToDomain( variables[i] , domain);	
}


vector<string> Domain::getDomainOf(string variable) {

	vector<string> v;
	v = varDomains[variable];	//v is the vector with the values of the domain
	
	return v;
}

void Domain::print()
{
     for(int i = 0; i < variables.size(); i++)
     {cout<<variables[i]<<" -";
            for(int j =0; j < varDomains[variables[i]].size(); j++)
             cout<<varDomains[variables[i]][j];
             
             cout << endl;
     }
}

vector<string> Domain::getVariables()
{
	return variables;
}

void Domain::addToDomain(string variable, vector<string> values) {
	int i;
	vector<string> vardomain;
	vardomain = varDomains[variable];
	
	for (i = 0; i < values.size(); i++) 
	{
		vardomain.push_back(values[i]);
		domainsize[variable]++;

		varDomains[variable] = vardomain;
		
	}
}

//synartisi pou diagrafei mia timi apo to domain mias metavlitis
int Domain::removeFromDomain(string value, string variable)
{
     vector<string> &vardomain = varDomains[variable];
	
     for(int i = 0; i < vardomain.size(); i++)
     {
		if(vardomain[i] == value )
        {
			vardomain.erase(vardomain.begin()+i);
			domainsize[variable]--;
	        break;
        }
    }
	return vardomain.size();	
}
 
map <string, int> Domain::getDomainsize()
{
	return domainsize;
}
void Domain::recover(map<string, vector<string> > recov)
{  
	/* allages -> , epanaferei tis times (kathe timi sti swsti thesi) sto domain tis variable, 
	etsi wste na einai taksinomimeno to domain */
   
	for(int i =0; i<variables.size(); i++)
    	{
		for (int j = 0; j < recov[variables[i]].size(); j++)
		{ 
			string strRecov = recov[variables[i]][j];
			int pos = 0;
			int ival = atoi(strRecov.c_str());
			for (int ii = 0; ii < varDomains[variables[i]].size(); ii++) 
			{
				int x = atoi(varDomains[variables[i]][ii].c_str());
				if (x > ival) 
				{
					pos = ii;
					break;
				}
				else
					pos++;
			}
			varDomains[variables[i]].insert(varDomains[variables[i]].begin()+pos,recov[variables[i]][j]);
			domainsize[variables[i]]++;
		}
	}
}

Domain& Domain::operator=(const Domain& d) {
	if (this != &d)
		varDomains = d.varDomains;
	return *this;
}

