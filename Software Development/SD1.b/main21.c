#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BF_Defines.h"
#include "HDF_Lib.h"
#include "FMM_Lib.h"
#include "BMM_Lib.h"

#define fileName1 "NUMBERS1"
#define fileName2 "NUMBERS2"

#define BLOCKS 300

int main(void) {
	int rvalue;
	FILE *fp1, *fp2;
	char *buffer, *bufStart;
	int fd11, fd12, fd13;
	int fd21, fd22, fd23;
	int i,j, randNum, randFd;
	fileInfo_t finfo;

	// ---------------------- Dimiourgia twn fileName1 kai fileName2 ----------------------

	// ---------------------- Dimiourgia tou fileName1 ----------------------

	printf("\n--------------\n");
	printf("CREATING FILES\n");
	printf("--------------\n\n");

	// Elegxos an yparxei to fileName1
	printf("Checking if file %s exists...\n\n", fileName1);
	if (HDF_ExistsFile(fileName1) == TRUE) {
		printf("Checkpoint 1:\n");
		printf("HDF_ExistsFile returned TRUE!\n\n");
	}

	// Dimiourgia tou arxeiou fileName1
	printf("Creating file %s...\n\n", fileName1);
	if ( (rvalue = HDF_CreateFile(fileName1)) != BFE_OK) {
		printf("Checkpoint 2:\n");
		printf("HDF_CreateFile() returned %d!\n\n", rvalue);
	}

	// Anoigma tou arxeiou fileName1
	printf("Opening file %s...\n\n", fileName1);
	if ((rvalue = HDF_OpenFile(fileName1, &fp1)) != BFE_OK)
	{
		printf("Checkpoint 3:\n");
		printf("HDF_OpenFile() returned %d, should have returned 0!\n\n",rvalue);
	}

	// Dimiourgia epikefalidas tou arxeiou sti mnimi
	buffer = malloc(sizeof(char) * BF_BLOCK_SIZE);
	bufStart = buffer;
	memset(buffer, 0, BF_BLOCK_SIZE);

	// Antigrafi epikefalidas sto block 0 tou arxeiou
	printf("Writing file header to disk...\n\n");
	if ((rvalue = HDF_WriteBlock(0, buffer, fp1)) != BFE_OK)
	{
		printf("Checkpoint 4:");
		printf("HDF_WriteBlock() returned %d!\n\n",rvalue);
	}

	// Kleisimo tou arxeiou fileName1
	printf("Closing file %s\n\n", fileName1);
	if ((rvalue = HDF_CloseFile(fp1)) != BFE_OK)
	{
		printf("Checkpoint 5:\n");
		printf("HDF_CloseFile() returned %d!\n\n", rvalue);
	}

	// ---------------------- Dimiourgia tou fileName2 ----------------------

	// Elegxos an yparxei to fileName2
	printf("Checking if file %s exists...\n\n", fileName2);
	if (HDF_ExistsFile(fileName2) == TRUE) {
		printf("Checkpoint 6:\n");
		printf("HDF_ExistsFile returned TRUE!\n\n");
	}

	// Dimiourgia tou arxeiou fileName2
	printf("Creating file %s...\n\n", fileName2);
	if ( (rvalue=HDF_CreateFile(fileName2)) != BFE_OK) {
		printf("Checkpoint 7:\n");
		printf("HDF_CreateFile() returned %d!\n\n", rvalue);
	}

	// Anoigma tou arxeiou fileName2
	printf("Opening file %s...\n\n", fileName2);
	if ((rvalue = HDF_OpenFile(fileName2, &fp2)) != BFE_OK)
	{
		printf("Checkpoint 8:\n");
		printf("HDF_OpenFile() returned %d, should have returned 0!\n\n",rvalue);
	}

	// Antigrafi epikefalidas sto block 0 tou arxeiou
	printf("Writing file header to disk...\n\n");
	if ((rvalue = HDF_WriteBlock(0, buffer, fp2)) != BFE_OK)
	{
		printf("Checkpoint 9:");
		printf("HDF_WriteBlock() returned %d!\n\n",rvalue);
	}

	// Kleisimo tou arxeiou fileName2
	printf("Closing file %s\n\n", fileName2);
	if ((rvalue = HDF_CloseFile(fp2)) != BFE_OK)
	{
		printf("Checkpoint 10:\n");
		printf("HDF_CloseFile() returned %d!\n\n", rvalue);
	}

	// ---------------------- FMM epipedo ----------------------

	printf("------------------------------------\n");
	printf("OPENING  AND PROCESSING FILES\n");
	printf("------------------------------------\n\n");

	FMM_Init();
	BMM_Init();

	// 1o anoigma tou fileName1
	printf("Opening file %s (1)...\n\n", fileName1);
	if ((fd11 = FMM_Open(fileName1)) < 0) {
		printf("Checkpoint 11:\n");
		printf("FMM_Open() returned %d!\n\n", fd11);
	}

	// 2o anoigma tou fileName1
	printf("Opening file %s (2)...\n\n", fileName1);
	if ((fd12 = FMM_Open(fileName1)) < 0) {
		printf("Checkpoint 12:\n");
		printf("FMM_Open() returned %d!\n\n", fd12);
	}

	// 3o anoigma tou fileName1
	printf("Opening file %s (3)...\n\n", fileName1);
	if ((fd13 = FMM_Open(fileName1)) < 0) {
		printf("Checkpoint 13:\n");
		printf("FMM_Open() returned %d!\n\n", fd12);
	}



	// 1o anoigma tou fileName2
	printf("Opening file %s (1)...\n\n", fileName2);
	if ((fd21 = FMM_Open(fileName2)) < 0) {
		printf("Checkpoint 14:\n");
		printf("FMM_Open() returned %d!\n\n", fd21);
	}

	// 2o anoigma tou fileName2
	printf("Opening file %s (2)...\n\n", fileName2);
	if ((fd22 = FMM_Open(fileName2)) < 0) {
		printf("Checkpoint 15:\n");
		printf("FMM_Open() returned %d!\n\n", fd21);
	}


	// 3o anoigma tou fileName2
	printf("Opening file %s (3)...\n\n", fileName2);
	if ((fd23 = FMM_Open(fileName2)) < 0) {
		printf("Checkpoint 16:\n");
		printf("FMM_Open() returned %d!\n\n", fd21);
	}


	// Ftiakse BLOCKS blocks gia kathe ena apo ta files
	for (i=0;i<BLOCKS;i++){

		randNum = rand()%3;

		if (randNum==1)
			randFd = fd13;
		else if (randNum==2)
			randFd = fd11;
		else
			randFd = fd12;

		finfo.fd = randFd;

		if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
			printf("Checkpoint 17:\n");
			printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
		}

		if ((rvalue = BMM_AllocBlock(finfo,i,&buffer)) < 0) {
			printf("Checkpoint 18:\n");
			printf("BMM_AllocBlock() returned %d!\n\n", rvalue);
		}

		if ((rvalue = FMM_HEAD_Set(fd11, i, TRUE)) < 0) {
			printf("Checkpoint 19:\n");
			printf("FMM_HEAD_Set() returned %d!\n\n", rvalue);
		}

		// Tropopoiisi tou block pou "efere" i BMM_AllocBlock
		j=(300-i)*(300-i);
		printf("%d\n", j);
		memcpy(buffer,&j,sizeof(int));

		if ((rvalue = BMM_UnpinBlock(finfo,i,TRUE)) < 0) {
			printf("Checkpoint 20:\n");
			printf("BMM_UnpinBlock() returned %d!\n\n", rvalue);
		}


		randNum = rand()%3;

		if (randNum==1)
			randFd = fd22;
		else if (randNum==2)
			randFd = fd21;
		else
			randFd = fd23;

		finfo.fd = randFd;

		if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
			printf("Checkpoint 21:\n");
			printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
		}

		if ((rvalue = BMM_AllocBlock(finfo,i,&buffer)) < 0) {
			printf("Checkpoint 22:\n");
			printf("BMM_AllocBlock() returned %d!\n\n", rvalue);
		}

		if ((rvalue = FMM_HEAD_Set(fd21, i, TRUE)) < 0) {
			printf("Checkpoint 23:\n");
			printf("FMM_HEAD_Set() returned %d!\n\n", rvalue);
		}

		// Tropopoiisi tou block pou "efere" i BMM_AllocBlock

		memcpy(buffer,&i,sizeof(int));

		if ((rvalue = BMM_UnpinBlock(finfo,i,TRUE)) < 0) {
			printf("Checkpoint 24:\n");
			printf("BMM_UnpinBlock() returned %d!\n\n", rvalue);
		}
	}


	finfo.fd = fd11;
	if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
		printf("Checkpoint 25:\n");
		printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
	}
	if ((rvalue = BMM_Flush(finfo)) < 0) {
		printf("Checkpoint 26:\n");
		printf("BMM_Flush() returned %d!\n\n", rvalue);
	}

	finfo.fd = fd21;
	if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
		printf("Checkpoint 27:\n");
		printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
	}
	if ((rvalue = BMM_Flush(finfo)) < 0) {
		printf("Checkpoint 28:\n");
		printf("BMM_Flush() returned %d!\n\n", rvalue);
	}



	printf("-------------\n");
	printf("CLOSING FILES\n");
	printf("-------------\n\n");



	// Kleisimo 1ou anoigmatos tou fileName1
	printf("Closing file %s (1)...\n\n", fileName1);
	if ((rvalue = FMM_Close(fd11)) != BFE_OK) {
		printf("Checkpoint 29:\n");
		printf("FMM_Close() returned %d!\n\n", rvalue);
	}

	// Kleisimo 2ou anoigmatos tou fileName1
	printf("Closing file %s (2)...\n\n", fileName1);
	if ((rvalue = FMM_Close(fd12)) != BFE_OK) {
		printf("Checkpoint 31:\n");
		printf("FMM_Close() returned %d!\n\n", rvalue);
	}

	// Kleisimo 3ou anoigmatos tou fileName1
	printf("Closing file %s (3)...\n\n", fileName1);
	if ((rvalue = FMM_Close(fd13)) != BFE_OK) {
		printf("Checkpoint 31:\n");
		printf("FMM_Close() returned %d!\n\n", rvalue);
	}



	// Kleisimo 1ou anoigmatos tou fileName2
	printf("Closing file %s (1)...\n\n", fileName2);
	if ((rvalue = FMM_Close(fd21)) != BFE_OK) {
		printf("Checkpoint 32:\n");
		printf("FMM_Close() returned %d!\n\n", rvalue);
	}

	// Kleisimo 2ou anoigmatos tou fileName2
	printf("Closing file %s (2)...\n\n", fileName2);
	if ((rvalue = FMM_Close(fd22)) != BFE_OK) {
		printf("Checkpoint 33:\n");
		printf("FMM_Close() returned %d!\n\n", rvalue);
	}

	// Kleisimo 3ou anoigmatos tou fileName2
	printf("Closing file %s (3)...\n\n", fileName2);
	if ((rvalue = FMM_Close(fd23)) != BFE_OK) {
		printf("Checkpoint 34:\n");
		printf("FMM_Close() returned %d!\n\n", rvalue);
	}


	free(bufStart);

	return 0;
}
