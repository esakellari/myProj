#include "SMM_Lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
	int fileDesc;				// to fileDesc tou arxeiou sto opoio scanarw -1 an einai free
	int op;						// o operator tou scannarismatos ( < , <= , >= ...)
	char attrType;				// typos pediou
	int attrLength;				// megethos pediou se byte
	int attrOffset;				// apostasi tou pediou se byte apo tin arxi tis eggrafis
	char *rvalue;				// deiktis stin timi pros sugkrisi
	int recordSize;				// to megethos tis eggrafis
	int lastRecAccessed;		// i thesi tis teleutaias eggrafis pou gurise to scannarisma
}openscan_t;

static openscan_t openScans[MAXSCANS];


void SMM_Init()
{
	int i;
	for (i = 0; i < MAXSCANS; i++)
		openScans[i].fileDesc = -1;
}

int SMM_OpenFileScan(int fileDesc, int recordSize, char attrType, int attrLength,
					 int attrOffset, int op, char *value)
{
	int i;
	
	if (recordSize < 0 )								// ean dwthike arnitiko recordSize
		return (HF_Errno = HFE_RECSIZE);

	if (attrType != 'f' && attrType != 'i' && attrType != 'c')
		return (HF_Errno = HFE_ATTRTYPE);				// elegxos tou attrType

	if (op != EQ && op != LT && op != GT && op != LTEQ && op != GTEQ && op != NEQ)
		return (HF_errno = HFE_INVALIDOP);				// elegxos gia invalid operator


	for (i = 0; i < MAXSCANS; i++) {
		if (openScans[i].fileDesc == -1) {				// an brike eleutheri thesi
			
			openScans[i].fileDesc = fileDesc;
			openScans[i].op = op;
			openScans[i].lastRecAccessed = -1;
			openScans[i].attrLength = attrLength;
			openScans[i].attrType = attrType;
			openScans[i].attrOffset = attrOffset;

			if(attrType == 'c')
			{
				strcpy(openScans[i].rvalue,value);
			}

			else if(attrType == 'i')
			{
				openScans[i].rvalue = *(int*)value;	
			}

			else if(attrType == 'f')
			{
				openScans[i].rvalue = *(float*)value;	
			}
			openScans[i].recordSize = recordSize;
			
			return i;									// epistrefei ti thesi tou anoigmatos
		}
	}
	return (HF_errno = HFE_STFULL);						// ean o pinakas me tis anoixtes sarwseis htan full
}

int SMM_FindNextRec(int scanDesc, char *record)
{
}

int SMM_CloseFileScan(int scanDesc)
{
	openScans[scanDesc].fileDesc = -1;
	return HFE_OK;
}

BOOLEAN SMM_hasOpenScans(int fileDesc)
{
	int i;

	for(i = 0; i < MAXSCANS; i++)
	{
		if(openScans[i].fileDesc == fileDesc)
		return TRUE;
	}

	return FALSE;

}
