#include <stdio.h>
#include <stdlib.h>

#include "HF_Lib.h"
#include "CFM_Lib.h"
#include "UT_DM_Defines.h"

int relCatFd2,attrCatFd2;
int main(int argc, char* argv[])
{

  	 char command[80];
   	 char* dbname;

	if(argc!=2)
	{
        	printf("Χρήση: %s dbname \n",argv[0]);
       		 return(1);
    	}
   
   	dbname=argv[1];
   	sprintf(command,"mkdir %s",dbname);
    	system(command);
   
    	if(chdir(dbname))
	{
       		 printf("Δεν είναι δυνατή η δημιουργία της βάσης");
       		 return(2);
    	}

    /* Δημιουργία των πινάκων relCat και attrCat με χρήση των
        συναρτήσεων ΗF_CreateFile */
	
	HF_Init();
	BF_Init();
	
	if(HF_CreateFile("relCat")< 0)
	{
		 printf("Δεν είναι δυνατή η δημιουργία της βάσης");
	 	return(2);
	}
	
	if(HF_CreateFile("attrCat")< 0)
	{
		 printf("Δεν είναι δυνατή η δημιουργία της βάσης");
	 	return(2);
	}
  	
	if ((relCatFd = HF_OpenFile("relCat")) < 0) {
		printf("Cannot open file relCat\n");
		return 2;
	}
	
	if ((attrCatFd = HF_OpenFile("attrCat")) < 0) {
		printf("Cannot open file attrCat\n");
		return 2;
	}

	if (CFM_Create() != UTE_OK) {
		printf("Error in inserting relCat - attrCat default values\n");
		return (2);
	}

	if(HF_CloseFile(relCatFd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	if(HF_CloseFile(attrCatFd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	printf("Database created successfully\n");
	system("pause");
	return 0;
}

