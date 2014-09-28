/*
 * File:   MemoryBlocks.h
 * Author: Herald
 *
 * Created on March 6, 2008, 2:48 AM
 */

#include "BF_Defines.h"

#ifndef _MEMORYBLOCKS_H
#define	_MEMORYBLOCKS_H

void BMM_Init();

int BMM_GetBlock(fileInfo_t finfo, int blockNum, char** blockBuf);
int BMM_AllocBlock(fileInfo_t finfo, int blockNum, char** blockBuf);
int BMM_UnpinBlock(fileInfo_t finfo, int blockNum, BOOLEAN dirty);
int BMM_DisposeBlock(fileInfo_t finfo, int blockNum);

int BMM_Flush(fileInfo_t finfo);
int BMM_Clear(char* fileName);

#endif	/* _MEMORYBLOCKS_H */
