#ifndef _HDF_H
#define _HDF_H

#include "BF_Defines.h"

int HDF_CloseFile(FILE* file);

int HDF_CreateFile(char* fileName);

int HDF_DestroyFile(char* fileName);

BOOLEAN HDF_ExistsFile(char* fileName);

int HDF_ReadBlock(int blockIndex ,char* blockBuf,FILE* file);

int HDF_WriteBlock(int blockIndex,char* blockBuf,FILE* file);

#endif
