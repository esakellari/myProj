#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "BF_Defines.h"
#include "HDF_Lib.h"


#define TESTOPENFILES 5000


int main(void)
{
	int i,fileDesc,rvalue,num,offset;
	char* fileName, *blockBuf, *bufStart, *newBlockBuf;
	FILE* fd;
	BOOLEAN exists;
	
	fileName = malloc(50*sizeof(char));
	
	srand(time(NULL));
	
	// Kataskeui TESTOPENFILES arxeiwn, anoigma, kleisimo kai katastrofi tous
	for (i=0;i<TESTOPENFILES;i++)
	{
		
		sprintf(fileName,"test%d",i);
		
		printf("%s\n",fileName);

		// Elegxos an yparxei to arxeio prin ti dimiourgia tou
		if ((exists = HDF_ExistsFile(fileName)) == TRUE)
		{
			printf("Checkpoint 1:\n");
			printf("HDF_ExistsFile() returned TRUE!\n");
		}

		// Dimiourgia arxeiou
		if ( (rvalue=HDF_CreateFile(fileName))!=BFE_OK)
		{
			printf("Checkpoint 2:\n");
			printf("HDF_CreateFile() returned %d!\n", rvalue);
		}

		// Elegxos an yparxei to arxeio meta ti dimiourgia tou
		if ((exists = HDF_ExistsFile(fileName)) == FALSE)
		{
			printf("Checkpoint 3:\n");
			printf("HDF_ExistsFile() returned FALSE!\n");
		}

		// Anoigma arxeiou
		if ((fileDesc = HDF_OpenFile(fileName,&fd))!=BFE_OK)
		{
			printf("Checkpoint 4:\n");
			printf("HDF_OpenFile() returned %d, should have returned 0!\n",fileDesc);
		}
		
		// Kleisimo arxeiou
		if ( (rvalue=HDF_CloseFile(fd))!=BFE_OK)
		{
			printf("Checkpoint 5:\n");
			printf("HDF_CloseFile() returned %d!\n", rvalue);
		}
		
		// Katastrofi arxeiou
		if ( (rvalue=HDF_DestroyFile(fileName))!=BFE_OK)
		{
			printf("Checkpoint 6:\n");
			printf("HDF_DestroyFile() returned %d!\n", rvalue);
		}
		
		// Elegxos an yparxei to arxeio meta tin katastrofi tou
		if ((exists = HDF_ExistsFile(fileName)) == TRUE)
		{
			printf("Checkpoint 7:\n");
			printf("HDF_ExistsFile() returned TRUE!\n");
		}
		
	}
	
	printf("\n\nFinished 1st part of checking...\n");
	
	//////////////////////////////////
	// Grapsimo kai diavasma blocks //
	//////////////////////////////////
	
	i = rand()%TESTOPENFILES;
	
	sprintf(fileName,"test%d",i);

	// Elegxos an yparxei to arxeio prin ti dimiourgia tou
	if ((exists = HDF_ExistsFile(fileName)) == TRUE)
	{
		printf("Checkpoint 8:\n");
		printf("HDF_ExistsFile() returned TRUE!\n");
	}

	// Dimiourgia arxeiou
	if ( (rvalue=HDF_CreateFile(fileName))!=BFE_OK)
	{
		printf("Checkpoint 9:\n");
		printf("HDF_CreateFile() returned %d!\n", rvalue);
	}

	// Elegxos an yparxei to arxeio meta ti dimiourgia tou
	if ((exists = HDF_ExistsFile(fileName)) == FALSE)
	{
		printf("Checkpoint 10:\n");
		printf("HDF_ExistsFile() returned FALSE!\n");
	}
	
	// Anoigma arxeiou
	if ((fileDesc = HDF_OpenFile(fileName,&fd))!=0)
	{
		printf("Checkpoint 11:\n");
		printf("HDF_OpenFile() returned %d, should have returned 0!\n",fileDesc);
	}
	
	
	// Grapsimo kai diavasma dyo blocks apo to arxeio.
	
	blockBuf = (char*)malloc(BF_BLOCK_SIZE*sizeof(char));
	
	
	// Diavasma tou block 100, prepei na MIN yparxei
	if ((rvalue=HDF_ReadBlock(100,blockBuf,fd))==BFE_OK)
	{
		printf("Checkpoint 12:");
		printf("HDF_ReadBlock() returned BFE_OK, for a block that should not exist!\n");
	}
	
	
	//Grapsimo tou arithmou num se tyxaia thesi tou block num
	num = rand();
	offset = rand()%1000;
	bufStart = blockBuf;
	blockBuf += offset;
	*(int*)blockBuf = num;
	
	//Grapsimo enos block sto arxeio fileName
	if ((rvalue=HDF_WriteBlock(num,bufStart,fd))!=BFE_OK)
	{
		printf("Checkpoint 13:");
		printf("HDF_WriteBlock() returned %d!\n",rvalue);
	}
	
	newBlockBuf = (char*)malloc(BF_BLOCK_SIZE*sizeof(char));
	
	// Diavasma tou block autou, prepei na yparxei
	if ((rvalue=HDF_ReadBlock(num,newBlockBuf,fd))!=BFE_OK)
	{
		printf("Checkpoint 14:");
		printf("HDF_ReadBlock() returned %d!\n",rvalue);
	}
	
	// Elegxos an graftike kai diavastike swsta to block me arithmo num
	if (*(int*)(newBlockBuf+offset)!=num)
	{
		printf("Checkpoint 15:");
		printf("Block either written or read in a wrong way! %d and %d shoulb be equal\n",num,*(int*)(newBlockBuf+offset));
		
	}
	
	// Kleisimo arxeiou
	if ( (rvalue=HDF_CloseFile(fd))!=BFE_OK)
	{
		printf("Checkpoint 16:\n");
		printf("HDF_CloseFile() returned %d!\n", rvalue);
	}
	
	// Katastrofi arxeiou
	if ( (rvalue=HDF_DestroyFile(fileName))!=BFE_OK)
	{
		printf("Checkpoint 17:\n");
		printf("HDF_DestroyFile() returned %d!\n", rvalue);
	}
	
	// Elegxos an yparxei to arxeio meta tin katastrofi tou
	if ((exists = HDF_ExistsFile(fileName)) == TRUE)
	{
		printf("Checkpoint 18:\n");
		printf("HDF_ExistsFile() returned TRUE!\n");
	}
	
	free(bufStart);
	free(newBlockBuf);
	
	printf("\nFinished 2nd part of checking...\n\n");
	
	return 0;
}
