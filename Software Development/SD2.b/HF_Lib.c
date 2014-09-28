#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HF_Lib.h"


#include "HF_Defines.h"

#include "RMM_Lib.h"
#include "SMM_Lib.h"
#include "BF_Lib.h"

void HF_Init()
{
	BF_Init();				// initializing bf structures
	SMM_Init();				// initializing smm structures
}

int HF_CreateFile(char *fileName)
{
	int fd;
	char* head;

	if((head = malloc(BF_BLOCK_SIZE)) == NULL)
	{
		HF_errno = HFE_NOMEM;
		return HF_errno;

	}

	if(BF_CreateFile(fileName) < 0)
	{
		HF_errno = HFE_BFERROR;
		return HF_errno;
	}
	if((fd = BF_OpenFile(fileName))< 0)
	{
		HF_errno = HFE_BFERROR;
		return HF_errno;

	}
	if(BF_AllocBlock(fd, 0, &head)<0)
	{
		HF_errno = HFE_BFERROR;
		return HF_errno;
	}	

	if(BF_UnpinBlock(fd, 0, 1)<0)
	{
		HF_errno = HFE_BFERROR;
		return HF_errno;

	}

	if(BF_CloseFile(fd)<0)
	{
		 HF_errno = HFE_BFERROR;
		 return HF_errno;
	}

	return HFE_OK;

}

int HF_DestroyFile(char *fileName)
{
	if(BF_DestroyFile(fileName)<0)
	{
		HF_errno = HFE_BFERROR;
		 return HF_errno;
	}	
}


int HF_OpenFile(char *fileName)
{
	int fd;

	if((fd = BF_OpenFile(fileName))<0)
	{
		HF_errno = HFE_BFERROR;
		return HF_errno;
	}

	return fd;

}


int HF_CloseFile(int fileDesc)
{
	if(SMM_hasOpenScans(fileDesc) == FALSE)
	{
		if(BF_CloseFile(fileDesc) < 0)
		{
			HF_errno = HFE_BFERROR;
			return HF_errno;

		}
	}
	
	else 
	{
		HF_errno = HFE_HASOPENSCANS;
		return HF_errno;
	}

}


int HF_InsertRec(int fileDesc, char *record, int recordSize)
{
	return RMM_InsertRec(fileDesc, record, recordSize);
}

int HF_DeleteRec(int fileDesc, int recId, int recordSize)
{
	return RMM_DeleteRec( fileDesc, recId, recordSize);
}

int HF_GetFirstRec(int fileDesc, char *record, int recordSize)
{
	return RMM_GetFirstRec(fileDesc, record, recordSize);
}

int HF_GetNextRec(int fileDesc, int recId, char *record, int recordSize)
{
	return RMM_GetNextRec(fileDesc, recId, record, recordSize);
}

int HF_GetThisRec(int fileDesc, int recId, char *record, int recordSize)
{
	return RMM_GetThisRec(fileDesc, recId, record, recordSize);
}



int HF_OpenFileScan(int fileDesc, int recordSize, char attrType, int attrLength,
					 int attrOffset, int op, char *value)
{
	return SMM_OpenFileScan(fileDesc,recordSize,attrType,attrLength,attrOffset,op,value);
}

int HF_FindNextRec(int scanDesc, char *record)
{
	return SMM_FindNextRec(scanDesc, record);
}

int HF_CloseFileScan(int scanDesc)
{
	return SMM_CloseFileScan(scanDesc);
}


void HF_PrintError(char *errString)
{
	printf("%s : ",errString);
	if (HF_errno == HFE_OK)
		printf("No Error happened\n");
	else if (HF_errno == HFE_BFERROR)
		printf("Error in a BF function happened\n");
	else if (HF_errno == HFE_EOF)
		printf("End of File reached\n");
	else if (HF_errno == HFE_INVALIDREC)
		printf("Invalid Record Id\n");
	else if (HF_errno == HFE_STFULL)
		printf("open scans table is full\n");
	else if (HF_errno == HFE_INVALIDOP)
		printf("Invalid operator giver for search\n");
	else if (HF_errno == HFE_ATTRTYPE)
		printf("Invalid type of attribute given for search\n");
	else if (HF_errno == HFE_RECSIZE)
		printf("invalid record size given\n");
	else
		printf("No message for this error number\n");
}
