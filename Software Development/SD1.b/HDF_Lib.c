#include "HDF_Lib.h"
#include "BF_Defines.h"

/*************************************************************HDF*********************************************/

//anoigma arxeiou
int HDF_OpenFile(char* fileName,FILE** file)
{


	
	*file = fopen(fileName, "rb+");

	if ( *file == NULL)
	{
		BF_errno = BFE_CANNOTOPENFILE;
		return BF_errno;
	}
	else return BFE_OK;



}

//kleisimo arxeiou
int HDF_CloseFile(FILE* file)
{


	if((fclose(file))==0)
	return BFE_OK;

	else 
	{
		BF_errno = BFE_CANNOTCLOSEFILE;
		return  BF_errno;     
	}


}

//an yparxei to arxeio epistrefei true, alliws false
BOOLEAN HDF_ExistsFile(char* fileName)
{

	FILE* fp;

	fp =  fopen(fileName, "rb");

	if(fp != NULL)
	{
		if(HDF_CloseFile(fp)!= BFE_OK)
		return FALSE;

		return TRUE;
	}

	else if( fp == NULL)
	return FALSE;

}



//dimiourgia arxeiou
int HDF_CreateFile(char* fileName)
{
	FILE* fp;

	//elegxw a=an yparxei idi to arxeio, an yparxei epistrefei error
	if( HDF_ExistsFile(fileName) == TRUE)

	{

		BF_errno = BFE_FILEEXISTS  ;
		return BF_errno;
	}
	

	fp = fopen(fileName, "wb+");

	if( fp == NULL)
	{
		BF_errno =  BFE_CANNOTCREATEFILE;
		return BF_errno;
	}

	else 
	{
		if(HDF_CloseFile(fp) !=BFE_OK)
		return BF_errno;

		return BFE_OK;     
	}
}


//katastrofi arxeiou
int HDF_DestroyFile(char* fileName)
{

	if( remove(fileName) != 0)
	{
		BF_errno =  BFE_CANNOTDESTROYFILE;
		return BF_errno;
	}

	else return BFE_OK; 

}



//diavasma enos block apo to arxeio
int HDF_ReadBlock(int blockIndex ,char* blockBuf,FILE* file)
{

	size_t result;

	
	fseek(file, blockIndex*BF_BLOCK_SIZE, SEEK_SET);

	result = fread(blockBuf, 1,  BF_BLOCK_SIZE, file);

	if( result !=  BF_BLOCK_SIZE)
	{
		BF_errno = BFE_INCOMPLETEREAD;
		return BF_errno;
	}
	else return BFE_OK;

}

//grapsimo block sto arxeio
int HDF_WriteBlock(int blockIndex,char* blockBuf,FILE* file)
{

	size_t result;


	fseek(file, blockIndex*BF_BLOCK_SIZE, SEEK_SET);

	result = fwrite(blockBuf, 1, BF_BLOCK_SIZE, file);

	if(result != BF_BLOCK_SIZE)
	{
		BF_errno = BFE_INCOMPLETEWRITE;
		return BF_errno;
	}

	else return BFE_OK;


}
