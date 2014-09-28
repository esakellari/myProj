#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OS.h"



int main()

{	
    char* arg;
    Rtree root_tr; 
    int k;
	
 
     arg = malloc(15*sizeof(char));
		
	printf("initialize\n");
	printf("insert <ID><name><ParentID><DOB><BirthPlace><SpouceID> <k> <kidID>... <kidID>\n");	
	printf("load <file>\n");
	printf("lookup <ID> <name>\n");
	printf("print <ID> <name>\n");
	printf("allchildren <ID> <name>\n");
	printf("gchildren <ID> <name>\n");
	printf("gKchildren <ID1> <ID2>\n");
	printf("ancestry <ID>\n");
	printf("Kcousins <ID1> <ID2>\n");
	printf("avgOffspring <ID1> k <name>\n");
	printf("end to exit\n");


	//analoga me to orisma pou tha dwthei, ginetai klhsh tis antistoixis synartisis
    	
		while ( 1 )
		{	
            	scanf("%s", arg);
            	
			if( strcmp(arg, "insert") == 0)
            insert_sw(root_tr);
				
			if( strcmp( arg, "lookup") == 0 )
			lookup_sw(root_tr);

			if(strcmp(arg, "load") == 0)
			load(root_tr);

			if(strcmp(arg, "gchildren") == 0 )
			gchildren(root_tr);

			if(strcmp(arg , "print") == 0)
			print(root_tr);

			if(strcmp(arg, "allchildren") == 0)
			allchildren(root_tr);
			
			if(strcmp(arg, "ancestry") == 0 )
			ancestry(root_tr);
			
			if(strcmp ( arg, "gKchildren") == 0)
			gKchildren(root_tr);
		
			if( strcmp(arg ,  "Kcousins") == 0)
			Kcousins(root_tr);
			
			if(strcmp (arg, "end")== 0 )
			break;
				
			if(strcmp(arg, "avgoffspring") == 0 )
			avgoffspring(root_tr);
			
			if(strcmp(arg, "initialize") == 0 )
			{
				root_tr = initialize();
				k = 1;
			}
		
		}
		
	if( k == 1)
	free(root_tr);


    return 0;
}


