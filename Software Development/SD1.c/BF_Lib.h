
/************************************************
 *  						*
 *   						*
 *    	сайеккаяг екисабет 1115200600152	*
 *     						*
 *   						*
 ************************************************/


#include "BF_Defines.h"


#ifndef _BF_
#define _BF_


void BF_Init( void );

int BF_CreateFile(char *fileName);

int BF_DestroyFile(char *fileName);

int BF_OpenFile (char *fileName);

int BF_CloseFile (int fileDesc);

int BF_GetFirstBlock(int fileDesc,int *blockNum ,char **blockBuf );

int BF_GetNextBlock(int fileDesc ,int *blockNum ,char **blockBuf );

int BF_GetThisBlock(int fileDesc,int blockNum,char **blockBuf);

int BF_AllocBlock(int fileDesc,int *blockNum ,char **blockBuf);


int BF_DisposeBlock(int fileDesc,int blockNum);

int BF_UnpinBlock(int fileDesc,int blockNum,int dirty);

void BF_PrintError(char *errString );


#endif

