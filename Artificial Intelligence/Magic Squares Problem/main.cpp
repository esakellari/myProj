#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "magicCSP.h"
#include "magicCon.h"
#include "CSP.h"
#include "Constraint.h"
#include "Assignment.h"
#define NUM 4

using namespace std;

int main(int argc, char* argv[])
{

	int num =  atoi(argv[1]);


	magicCSP magic(num);
	magicCon m(num);

	Assignment *a;
	a = magic.backTrackingSearch(m, num);

	if (a == NULL) 
	{
		cout << "CSP has no solution!" << endl;
		return 0;
	} else {
		cout<<"Found Solutuion"<< endl;

		a->print(num);
	}

  	return 0;

}
