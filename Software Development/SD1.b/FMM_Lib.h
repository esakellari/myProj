/*
 * File:   OpenFiles_Lib.h
 * Author: Herald
 *
 * Created on March 6, 2008, 2:08 AM
 */

#include "BF_Defines.h"

#ifndef _OPENFILES_LIB_H
#define	_OPENFILES_LIB_H

void FMM_Init();

int FMM_Open(char* fileName);
int FMM_Close(int fileDesc);
BOOLEAN FMM_IsValid(int fileDesc);
BOOLEAN FMM_IsOpen(char* fileName);

int FMM_GetFileInfoByName(fileInfo_t* finfo);
int FMM_GetFileInfoByFD(fileInfo_t* finfo);

BOOLEAN FMM_HEAD_IsValid(int fileDesc, int block);
int FMM_HEAD_GetFirstBlock(int fileDesc);
int FMM_HEAD_GetNextBlock(int fileDesc, int prev);
int FMM_HEAD_GetFirstEmpty(int fileDesc);
int FMM_HEAD_Set(int fileDesc, int block, BOOLEAN value);

#endif	/* _OPENFILES_LIB_H */
