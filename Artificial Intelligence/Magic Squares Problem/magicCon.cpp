#include "magicCon.h"



magicCon::magicCon(int n)
{
	size = n;
	size_t found;

	Sum = (n*(n*n + 1))/2;
	cout<<"SUM " <<Sum<<endl;

	string  variable,square;

	vector<string> variables;
		
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

	//vazw sto map ta tetragwna tis idias grammis
	for(int j = 0; j < variables.size(); j++)
	{
		found = variable.find_first_of("_");
            
		string str1 = variables[j].substr(0,found);
		string str2 = variables[j].substr(found+1,variables[j].size());

		int row = convertToInt(str1);
		int colu = convertToInt(str2);
		 vector<string> temp;
		
		for(int i = 0; i < size; i++)
		{
			//gia ola ta alla tetragwnakia stin idia grammi ektos apo to eksetazomeno
			if(i!=colu)
			{
				square = str1 + "_" + convertToString(i);
				temp.push_back(square);
			}
		}

		putConstraints(variables[j], temp); 
		temp.clear();
		
		for(int i = 0; i < size; i++)
		{
			//gia ola ta alla tetragwnakia stin idia grammi ektos apo to eksetazomeno
			if(i!=row)
			{
				square = convertToString(i) + "_" + str2;
				temp.push_back(square);
			}
		}

		putConstraints(variables[j], temp); 
		
		//an vrisketai panw sti diagwnio opou i =j
		if(colu == row)
		{ 
			temp.clear();
			for(int i =0; i < size; i++)
			{
				if(i!=row)
				{
					square = convertToString(i) + "_" + convertToString(i);
              				temp.push_back(square);
				}
			}

			putConstraints(variables[j], temp); 
		}
		
        	//an vrisketai panw kai sti deyteri diagwnio
		if((colu+row)==(size-1))
		{
        		temp.clear();
			for(int i =0; i < size; i++)
			{
                    		if(i!=row)
                    		{
                              		int jj = (size - i -1);
                              		square = convertToString(i) + "_" + convertToString(jj);
    					temp.push_back(square);
        			}
            		}
            
			putConstraints(variables[j], temp); 
        	}
	}
}



bool magicCon::isSatisfiedWith(Assignment *assignment, string variable, string value, int con) 
{
	int sum,val,value2;
	size_t found;
	string square;
	vector<vector<string > > squares;

	found = variable.find_first_of("_");

	string str1 = variable.substr(0,found);
	string str2 = variable.substr(found+1,variable.size());
	
	//oi geitones  tis metavlitis
  	squares = getConstraintsOf(variable);

	int row = convertToInt(str1);
	int colu = convertToInt(str2);	

	value2= convertToInt(value);
	vector<string> variables = assignment->getAssignedVariables();
	
	//elegxw an i time exei ksanadwthei
	//an exei dothei idi, epistrefei false
	for(int i =0; i < variables.size(); i++)
	{     
		string tmp;
		tmp = assignment->getAssignment(variables[i]);
		val = convertToInt(tmp);
		
		if(val == value2)
			return false;
    	}

	int count;

 	vector<string> remainingVals = assignment->getRemaining();
	
	for(int j = 0; j < squares.size(); j++)
    	{ 
		sum = 0;
		count  = 0;
		
      		for(int i = 0; i < squares[j].size() ;  i++)
      		{
			if (assignment->hasAssignmentFor(squares[j][i])) 
        	    	{
            			count++;
            			string tmp;
              		 	tmp = assignment->getAssignment(squares[j][i]);
             			val = convertToInt(tmp);
    	     	 		sum += val;
			}
       		}
       		
		count++;
        	sum+=value2;
        	
        	int left = size - count;
        	int leftsum = Sum -sum;

		//an exei meinei mono ena adeio koutaki akoma
		if( left == 1 )
		{ 
			//an to athroisma einai arnitiko, epistrefei false
			if(leftsum <= 0)
			return false;
			
			int i;

			for(i = 0; i < remainingVals.size(); i++)
			{
				if(leftsum!= convertToInt(remainingVals[i]))
				continue;

				else break;
			}

			if( i == remainingVals.size())
			return false;
		}
    
		//an den exei meinei kanena keno koutaki kai to athroisma 
		//den einai to epithymito, epestrepse false
       		if((left == 0)&&(sum!= Sum))
       		return false;
       			       		
       		if(sum > Sum )
		return false;
    	}

	//BT
      	if(con == 1)
      	{
		//svinw tin timi pou tha dothei apo tis times pou mporei na dothoun se pomena koutakia
		for(int i = 0 ; i < remainingVals.size(); i++)
		{
			if(convertToInt(remainingVals[i]) == value2) 
			{
        			remainingVals.erase(remainingVals.begin()+i);
        			break;
			}
	 	}
	 	assignment->setRemaining(remainingVals);
	}


	//ola pigan kala
	return true;


}


bool magicCon::fc(Assignment *assignment, string variable, string value, Domain &d,map<string, vector<string> > *recov) 
{
	int sum,val,value2;
	size_t found;
	string square;
	vector<vector<string > > squares;
	
	//to vector squares krataei tis metavlites pou sxetizontai me periorismo me kathemia 
   	//metavliti 
  
	squares = getConstraintsOf(variable);  	
	value2= convertToInt(value);

	//pairnw oles tis metavlites
	vector<string> variables = d.getVariables();
 
 
	//afairw apo olwn ta domain tin timi pou edwsa stin meavliti variable ,an uparxei
	for(int i = 0; i < variables.size(); i++)
	{
			/* 2 allages, to fc sbinei times apo ta domain mono twn mi assigned metablitwn*/
        	if(variables[i]!= variable && !assignment->hasAssignmentFor(variables[i]))
       		{
				/* gia na diagrapsei mia timi apo to domain prepei auti na min uparxei */
				if(existsInVector(value, d.getDomainOf(variables[i])))
				{
          				(*recov)[variables[i]].push_back(value);
					//an adeiase to pedio kapoias metavlitis epistrefetai false
					if(d.removeFromDomain(value,variables[i])==0)
						return false;
				}
       		}
	}

	for(int i = 0; i < squares.size(); i++)
	{
		sum = 0;
		sum = sum + value2;
		
		//dimiourgw to athroisma twn geitonwn pou 
		//exoun timi 
		for(int jj = 0; jj < squares[i].size() ;  jj++)
		{
			if(squares[i][jj] !=  variable)
			{
				if (assignment->hasAssignmentFor(squares[i][jj]))
				{                                   
                    			string tmp;
              				tmp = assignment->getAssignment(squares[i][jj]);
             				val = convertToInt(tmp);
    	     	 			sum += val;
                		}
			}
		}

        

		//gia olous tous geitones tis metavlitis elegxw 
		//kai allazxw ta domain tous 
		for(int jj = 0; jj < squares[i].size() ;  jj++)
		{
			if((squares[i][jj] !=  variable) && ( !assignment->hasAssignmentFor(squares[i][jj])))
            		{
				vector<string> dom = d.getDomainOf(squares[i][jj]);
               			for(int j = 0; j < dom.size(); j++)
				{
					 if (!isSatisfiedWith(assignment, squares[i][jj],dom[j],0 ))  
					{
							(*recov)[squares[i][jj]].push_back(dom[j]);
								
							//an adeiae to pedio kapias metavlitis epistrefetai false
							if(d.removeFromDomain(dom[j],squares[i][jj])==0)
								return false;							
					}
				}
			}
		}
	}

	//ola pigan kala 
	return true;
   

}

/* i sunartisi pou koitaei ean to string val einai mesa sto vector<string>tmp*/
bool magicCon::existsInVector(string val, vector<string>tmp)
{
	for (int z = 0; z < tmp.size(); z++) 
		if (tmp[z] == val)
			return true;
	return false;
}

magicCon::~magicCon()
{
}
