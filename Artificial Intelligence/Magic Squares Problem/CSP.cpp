#include "CSP.h"

/*pithanes times tou CONTROL:
*0: BT
*1: FC+MRV
*2: BT+MRV
*/
#define CONTROL 1



     
vector<string> CSP::getDomainOf(string variable) {
	return varDomains[variable];
}

void CSP::addToDomain(string variable, vector<string> values) {	
	varDomains[variable] = values;
}

Assignment* CSP:: backTrackingSearch(Constraint &m, int n) 
{
	nodes = 0;
	checks = 0;	

	if(CONTROL == 0)
	cout <<"------------BT----------"<<endl;

	else if(CONTROL == 1)
	cout<<"------------FC+MRV------------"<<endl;

	else if(CONTROL == 2)
	cout << "------------BT+MRV------------"<<endl;


	Assignment *v = new Assignment(variables);
	Domain d(n);
	return recursiveBackTrackingSearch(v, m,n,d);
}

string CSP::mrv(Domain &d,Assignment *anAssignment,Constraint &c)
{


       if(CONTROL == 1)
       {

		map<string, int> size = d.getDomainsize();
		int min=10000;
		map<string,int>::iterator it;
	
		for ( it = size.begin() ; it != size.end(); it++ )	
		{
			if(((*it).second < min) && (!(anAssignment->hasAssignmentFor((*it).first ))))
			min = (*it).second;
		
		
		}

		for ( it = size.begin() ; it != size.end(); it++ )
		{
			if(((*it).second == min) && ( !(anAssignment->hasAssignmentFor((*it).first ))))
			{
				return  (*it).first;
			}
		}
	}
  
	//`BT+MRV
	//xrhsimopieitai kai pali i synartisi isSatisfiedWith( gia 
	//na elegksw an oi times tou pediou enos komvous einai nomimes i oxi 
	//edw den metavaletai to domain kammias metavlitis apla ginontai elegxoi. H meta
	//vliti me toys ligoterous periorismous epistrefetai 
  	if(CONTROL == 2)
  	{
		vector<string> variables;
		vector<string> domainValues;
		vector<int> sizes;
		variables = anAssignment->getVariables();
	
		for(int i = 0; i < variables.size(); i++)
        	{
			int count  = 0;
			domainValues = d.getDomainOf(variables[i]); 
             	
			for(int j =0; j <  domainValues.size();j++)
             		{
				if(!(anAssignment->hasAssignmentFor(variables[i] )))
				{
                   			if (c.isSatisfiedWith(anAssignment, variables[i],domainValues[j],0 ))  
                   			count++;
				}
                 	}  
             		sizes.push_back(count);
        	}
		int min = 10000;
	
		for(int i =0; i < sizes.size(); i++)
       	 	{
				//anatithetai i mikroteri timi stin metavliti min
				if((sizes[i]< min) && (!(anAssignment->hasAssignmentFor(variables[i] ))))
				min = sizes[i];   
		}    
	
		for(int i =0; i < sizes.size(); i++)
		{
			if((sizes[i] == min) && ( !(anAssignment->hasAssignmentFor((variables[i] )))))
			return  variables[i];
			
		}   
    
  	}
}

Assignment* CSP::recursiveBackTrackingSearch(Assignment *anAssignment, Constraint &m, int n,Domain &d) 
{
	int i;

	//nodes expanded
	nodes++;
	
	if (anAssignment->isComplete()) 
	{
		cout << "Complete assignment" << endl;
		cout<<"Nodes expanded: " << nodes<<endl;
		cout<<"Consistency checks: " << checks<< endl;
		return anAssignment;
	}

	string variable;
	vector<string> domainValues;


	//BT
	if(CONTROL == 0 )
	{
		variable = anAssignment->selectFirstUnassignedVariable();
		domainValues = d.getDomainOf(variable);

	}
	
	//FC+MRV
	else if((CONTROL == 1) ||(CONTROL == 2 )  )
	{
		variable = mrv(d,anAssignment, m );
		//variable = anAssignment->selectFirstUnassignedVariable();
		domainValues = d.getDomainOf(variable);
	}

	for (i = 0; i < domainValues.size(); i++) 
	{
	        bool valid = true;
		string value = domainValues[i];
	
		/***************BT / BT+MRV ******************************************/
		if((CONTROL  == 0) || (CONTROL == 2))
		{
			//consistency checks
			checks++;

			if (m.isSatisfiedWith(anAssignment, variable, value,1)) 
			{
				anAssignment->setAssignment(variable, value);
				Assignment* result = recursiveBackTrackingSearch(anAssignment, m,n,d);
				if (result != NULL) 
				return result;
				
				anAssignment->remove(variable);
			}		
		}

		/******************FC + MRV***************************************/
		if(CONTROL == 1)
		{

			map<string, vector<string> > *recov = new map<string, vector<string> >() ;             
			anAssignment->setAssignment(variable, value);
			checks++;	

			if(m.fc(anAssignment, variable, value,d, recov)==false)
			{
				d.recover(*recov);

	                	anAssignment->remove(variable);
        	        	valid = false;
			}
            
			// ean to valid einai false tote ekane recover auta pou diegrapse kai delete to assignment
			
			if (valid) 
			{
			   	Assignment* result = recursiveBackTrackingSearch(anAssignment, m,n,d);
				if (result != NULL) 
					return result;
				
				anAssignment->remove(variable);               
       	        		d.recover(*recov);
			}
		}
	}
	return NULL;	/* failure */
}



