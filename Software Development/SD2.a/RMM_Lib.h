/* 
 * File:   RMM_Lib.h
 */

#ifndef _RMM_LIB_H
#define	_RMM_LIB_H

#include "HF_Defines.h"
#include "BF_Defines.h"
#include "BF_Lib.h"


int RMM_InsertRec(int fileDesc, char* record, int recordSize);
int RMM_DeleteRec(int fileDesc, int recId, int recordSize);
int RMM_GetFirstRec(int fileDesc, char* record, int recordSize);
int RMM_GetNextRec(int fileDesc, int recId, char* record, int recordSize);
int RMM_GetThisRec(int fileDesc, int recId, char* record, int recordSize);

#endif	/* _RMM_LIB_H */

