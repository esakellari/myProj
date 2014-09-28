


#ifndef _FMM_H
#define _FMM_H


void FMM_Init();


BOOLEAN FMM_IsValid(int fileDesc);

int FMM_GetFileInfoByName(fileInfo_t* finfo);

int FMM_GetFileInfoByFD(fileInfo_t* finfo);

BOOLEAN FMM_IsOpen(char* fileName);

int FMM_Open(char* fileName);

int FMM_Close(int fileDesc);

BOOLEAN FMM_HEAD_IsValid(int fileDesc,int blockNum);

int FMM_HEAD_GetFirstBlock(int fileDesc);

int FMM_HEAD_GetNextBlock(int fileDesc,int prev);

int FMM_HEAD_GetFirstEmpty(int fileDesc);

int FMM_HEAD_Set(int fileDesc,int blockNum,BOOLEAN value);

#endif

