#ifndef _MAGIC_SQUARES_
#define _MAGIC_SQUARES_

#include "Constraint.h"
#include "CSP.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>


class magicCSP : public CSP {

	private:
		int size;

	public:
		
		//accesors
		int GetSize();

		//mutators
		void SetSize(int n);

		magicCSP(int n);
		~magicCSP(){}

};


#endif
