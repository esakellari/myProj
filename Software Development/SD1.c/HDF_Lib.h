/*
 * File:   HardDisk_Lib.h
 * Author: Herald
 *
 * Created on March 6, 2008, 1:58 AM
 */
#include "BF_Defines.h"

#ifndef _HARDDISK_LIB_H
#define	_HARDDISK_LIB_H

int HDF_OpenFile(char* fileName, FILE** file);
int HDF_CloseFile(FILE* file);
int HDF_CreateFile(char* fileName);
int HDF_DestroyFile(char* fileName);
BOOLEAN HDF_ExistsFile(char* fileName);

int HDF_ReadBlock(int blockNum, char* blockBuf, FILE* file);
int HDF_WriteBlock(int blockNum, char* blockBuf, FILE* file);

#endif	/* _HARDDISK_LIB_H */
