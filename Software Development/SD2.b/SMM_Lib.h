#ifndef _SMMLIB_
#define _SMMLIB_

#include "BF_Defines.h"

#define EQ 0
#define LT 1
#define GT 2
#define LTEQ 3
#define GTEQ 4
#define NEQ 5

#define MAXSCANS 20

void SMM_Init();
int SMM_OpenFileScan(int fileDesc, int recordSize, char attrType, int attrLength,
					 int attrOffset, int op, char *value);
int SMM_FindNextRec(int scanDesc, char *record);
int SMM_CloseFileScan(int scanDesc);
BOOLEAN SMM_hasOpenScans(int fileDesc);
#endif