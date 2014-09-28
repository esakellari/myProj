#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)

#include "BF_Lib.h"
#include "HF_Lib.h"
#include "AM_Lib.h"
#include "CFM_Lib.h"
#include "UT_DM_Defines.h"
#include "UT_Lib.h"
#include "parser4.h"


/*
int DM_select(int argc, char* argv[])
{
	int i;
	printf("%d",argc);
	for(i=0;i <argc;i++)
		printf("%s\n",argv[i]);
	UT_print("athlete");
	return 0;


}*/

void printArgs(int argc, char **argv){
	int i;
	for(i=0;i<=argc-1;++i)
		printf("argv[%d]=%s, ",i,argv[i]);
	printf("argv[%d]=(null)", argc);
	printf("\n\n");
}


int main(int argc, char* argv[]){
	char* dbname;

	if(argc!=2){
		printf("Usage: %s dbname \n",argv[0]);
		return 1;
	}

	dbname=argv[1];

	if(chdir(dbname)){
		printf("Cannot open database!\n");
		return 1;
	}

	BF_Init();
	HF_Init();
	AM_Init();

	if ((relCatFd = HF_OpenFile("relCat")) < 0) {
		printf("Cannot open file relCat\n");
		return 1;
	}

	if ((attrCatFd = HF_OpenFile("attrCat")) < 0) {
		printf("Cannot open file relCat\n");
		return 1;
	}



	/* Κλήση της συνάρτησης yyparse ή οποία */
	/* θα καλεί αυτόματα τις συναρτήσεις σας */
	while(yyparse() == RESTART);
	system("Pause");
	return 0;
}