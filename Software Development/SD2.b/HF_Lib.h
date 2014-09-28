#ifndef _HFLIB_
#define _HFLIB_

void HF_Init();
int HF_CreateFile(char *fileName);
int HF_DestroyFile(char *fileName);
int HF_OpenFile(char *fileName);
int HF_CloseFile(int fileDesc);
int HF_InsertRec(int fileDesc, char *record, int recordSize);
int HF_DeleteRec(int fileDesc, int recId, int recordSize);
int HF_GetFirstRec(int fileDesc, char *record, int recordSize);
int HF_GetNextRec(int fileDesc, int recId, char *record, int recordSize);
int HF_GetThisRec(int fileDesc, int recId, char *record, int recordSize);
int HF_OpenFileScan(int fileDesc, int recordSize, char attrType, int attrLength,
					 int attrOffset, int op, char *value);
int HF_FindNextRec(int scanDesc, char *record);
int HF_CloseFileScan(int scanDesc);
int HF_PrintError(char *errString);

#endif