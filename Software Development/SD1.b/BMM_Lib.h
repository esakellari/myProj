/* **********************************************
 *  						*
 *   						*
 *    сайеккаяг екисабет 111520600152		*
 *  						*
 *						*
 ************************************************/
 





#include "BF_Defines.h"
#include "FMM_Lib.h"
#include "HDF_Lib.h"

#ifndef _BM_
#define _BM_

//global roloi pou ayksanetai opote kapoio block ginetai alloc i get
int glob_clock;

//domi gia ta block
typedef struct {

	char filename[MAX_FILE_NAME];
	char data[BF_BLOCK_SIZE];
	BOOLEAN free;				//metavliti pou deixnei ama i thesi tou pinaka einai free i oxi
	BOOLEAN pinned[MAXOPENFILES];		//pinakas pou krataei ta anoigmata apo ta opoia einai pinned to block
	int blocknum;
	BOOLEAN dirty;
	int pincount;				//arithmos ton forwn pou einai karfwmeno to block
	int lastUsed;				//metavliti pou deixnei pote egine alloci get to sygekrimeno block

	}block;


block Blocks[BF_BUFFER_SIZE];

int Search(char* filename, int blocknum,int* cases);
int BMM_DisposeBlock(fileInfo_t finfo,int blockNum);
int BMM_UnpinBlock(fileInfo_t finfo,int blockNum,BOOLEAN dirty);
int BMM_Clear(char* fileName);
int BMM_AllocBlock(fileInfo_t finfo,int blockNum,char** blockBuf);
int BMM_GetBlock(fileInfo_t finfo,int blockNum,char** blockBuf);
int BMM_Flush(fileInfo_t finfo);
void BMM_Init();

#endif
