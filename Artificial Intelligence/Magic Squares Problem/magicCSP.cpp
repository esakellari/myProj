#include "magicCSP.h"
#include "Constraint.h"
#include "Domain.h"
#include "CSP.h"
#include "Converter.h"
#include <vector>
#include <string>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>



magicCSP::magicCSP(int n)
{
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

int magicCSP::GetSize()
{
	return size;
}

void magicCSP::SetSize(int n)
{

	size = n;
}
