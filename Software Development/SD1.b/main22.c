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

	char *buffer;
	int fd11, fd12;
	int fd21, fd22, blockNum;
	int i, randNum, randFd, number;
	fileInfo_t finfo;


	// ---------------------- FMM epipedo ----------------------

	printf("------------------------------------\n");
	printf("OPENING  AND PROCESSING FILES\n");
	printf("------------------------------------\n\n");

	FMM_Init();
	BMM_Init();

	// 1o anoigma tou fileName1
	printf("Opening file %s (1)...\n\n", fileName1);
	if ((fd11 = FMM_Open(fileName1)) < 0) {
		printf("Checkpoint 1:\n");
		printf("FMM_Open() returned %d!\n\n", fd11);
	}

	// 2o anoigma tou fileName1
	printf("Opening file %s (2)...\n\n", fileName1);
	if ((fd12 = FMM_Open(fileName1)) < 0) {
		printf("Checkpoint 2:\n");
		printf("FMM_Open() returned %d!\n\n", fd12);
	}



	// 1o anoigma tou fileName2
	printf("Opening file %s (1)...\n\n", fileName2);
	if ((fd21 = FMM_Open(fileName2)) < 0) {
		printf("Checkpoint 3:\n");
		printf("FMM_Open() returned %d!\n\n", fd21);
	}

	// 2o anoigma tou fileName2
	printf("Opening file %s (2)...\n\n", fileName2);
	if ((fd22 = FMM_Open(fileName2)) < 0) {
		printf("Checkpoint 4:\n");
		printf("FMM_Open() returned %d!\n\n", fd21);
	}


	// Dialekse ena apo ta dyo anoigmata gia to filename1
	randNum = rand()%2;

	if (randNum==1)
		randFd = fd22;
	else
		randFd = fd21;

	finfo.fd = randFd;

	if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
		printf("Checkpoint 5:\n");
		printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
	}

	i=0;

	// Fere ola ta blocks, kai typwse diadoxika tous arithmous
	printf("%s: (1st try)\n", fileName1);
	while (BMM_GetBlock(finfo, i, &buffer)==BFE_OK){
		i++;
		memcpy(&number,buffer,sizeof(int));
		printf("%d ",number);

	}

	// Katharise ti mnimi apo ola ta periexomena tis


	if ((rvalue = BMM_Clear(fileName1)) < 0) {
		printf("Checkpoint 6:\n");
		printf("BMM_Clear() returned %d!\n\n", rvalue);
	}

	if ((rvalue = BMM_Clear(fileName2)) < 0) {
		printf("Checkpoint 7:\n");
		printf("BMM_Clear() returned %d!\n\n", rvalue);
	}


	// Fere pali ola ta blocks, kai typwse diadoxika tous arithmous
	i=0;
	printf("\n\n%s: (2nd try)\n", fileName1);
	while (BMM_GetBlock(finfo, i, &buffer)==BFE_OK){

		memcpy(&number,buffer,sizeof(int));
		printf("%d ",number);

		if ((rvalue = BMM_UnpinBlock(finfo,i,FALSE)) < 0) {
			printf("Checkpoint 8:\n");
			printf("BMM_UnpinBlock returned %d!\n\n", rvalue);
		}

		randNum = rand()%2;

		if (randNum==1)
			randFd = fd22;
		else
			randFd = fd21;

		finfo.fd = randFd;

		if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
			printf("Checkpoint 9\n");
			printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
		}

		i++;
	}

	printf("\n\n");


	blockNum = rand()%BLOCKS;

	// Fere to blockNum block apo to anoigma fd21
	finfo.fd = fd21;

	if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
		printf("Checkpoint 10:\n");
		printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
	}


	if( BMM_GetBlock(finfo, blockNum, &buffer)!=BFE_OK) {
		printf("Checkpoint 11:\n");
		printf("BMM_GetBlock() returned %d!\n\n", rvalue);
	}


	// Fere to blockNum block apo to anoigma fd22
	finfo.fd = fd22;

	if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
		printf("Checkpoint 12:\n");
		printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
	}

	if( BMM_GetBlock(finfo, blockNum, &buffer)!=BFE_OK) {
		printf("Checkpoint 13:\n");
		printf("BMM_GetBlock() returned %d!\n\n", rvalue);
	}


	finfo.fd = fd21;

	if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
		printf("Checkpoint 14:\n");
		printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
	}

	// To fd21 kanei Unpin to blockNum block
	if( BMM_UnpinBlock(finfo, blockNum, FALSE)!=BFE_OK) {
		printf("Checkpoint 15:\n");
		printf("BMM_UnpinBlock() returned %d!\n\n", rvalue);
	}


	// To fd22 kanei Dispose to blockNum block
	if( BMM_DisposeBlock(finfo, blockNum)==BFE_OK) {
		printf("Checkpoint 16:\n");
		printf("BMM_DisposeBlock() returned %d!\n\n", rvalue);
	}
	else {
		printf("Checkpoint 16:\n");
		printf("BMM_DisposeBlock() failed! Nice!!\n\n");
	}


	// To fd22 kanei Unpin to blockNum block
	finfo.fd = fd22;

	if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
		printf("Checkpoint 17:\n");
		printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
	}

	// To fd21 kanei Unpin to blockNum block
	if( BMM_UnpinBlock(finfo, blockNum, FALSE)!=BFE_OK) {
		printf("Checkpoint 18:\n");
		printf("BMM_UnpinBlock() returned %d!\n\n", rvalue);
	}


	// Twra i Dispose prepei na petyxei
	if( BMM_DisposeBlock(finfo, blockNum)!=BFE_OK) {
		printf("Checkpoint 19:\n");
		printf("BMM_DisposeBlock() returned %d!\n\n", rvalue);
	}
	else {
		printf("Checkpoint 19:\n");
		printf("BMM_DisposeBlock() now succeeded!! Nice!!\n\n");
	}



	// Dialekse ena apo ta dyo anoigmata gia to filename1
	randNum = rand()%2;

	if (randNum==1)
		randFd = fd12;
	else
		randFd = fd11;

	finfo.fd = randFd;

	if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
		printf("Checkpoint 20:\n");
		printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
	}

	// Fere ola ta blocks tou fileName2, kai typwse diadoxika tous arithmous
	i=0;
	printf("\n%s:\n", fileName1);
	while (BMM_GetBlock(finfo, i, &buffer)==BFE_OK){

		memcpy(&number,buffer,sizeof(int));
		printf("%d ",number);

		if ((rvalue = BMM_UnpinBlock(finfo,i,FALSE)) < 0) {
			printf("Checkpoint 21:\n");
			printf("BMM_UnpinBlock returned %d!\n\n", rvalue);
		}

		randNum = rand()%2;

		if (randNum==1)
			randFd = fd12;
		else
			randFd = fd11;

		finfo.fd = randFd;

		if ((rvalue = FMM_GetFileInfoByFD(&finfo)) < 0) {
			printf("Checkpoint 22\n");
			printf("FMM_GetFileInfoByFD() returned %d!\n\n", rvalue);
		}

		i++;
	}

	printf("\n\n");

	printf("\n-------------\n");
	printf("CLOSING FILES\n");
	printf("-------------\n\n");


	// Kleisimo 1ou anoigmatos tou fileName1
	printf("Closing file %s (1)...\n\n", fileName1);
	if ((rvalue = FMM_Close(fd11)) != BFE_OK) {
		printf("Checkpoint 23:\n");
		printf("FMM_Close() returned %d!\n\n", rvalue);
	}

	// Kleisimo 2ou anoigmatos tou fileName1
	printf("Closing file %s (2)...\n\n", fileName1);
	if ((rvalue = FMM_Close(fd12)) != BFE_OK) {
		printf("Checkpoint 24:\n");
		printf("FMM_Close() returned %d!\n\n", rvalue);
	}




	// Kleisimo 1ou anoigmatos tou fileName2
	printf("Closing file %s (1)...\n\n", fileName2);
	if ((rvalue = FMM_Close(fd21)) != BFE_OK) {
		printf("Checkpoint 25:\n");
		printf("FMM_Close() returned %d!\n\n", rvalue);
	}

	// Kleisimo 2ou anoigmatos tou fileName2
	printf("Closing file %s (2)...\n\n", fileName2);
	if ((rvalue = FMM_Close(fd22)) != BFE_OK) {
		printf("Checkpoint 26:\n");
		printf("FMM_Close() returned %d!\n\n", rvalue);
	}

	return 0;
}
