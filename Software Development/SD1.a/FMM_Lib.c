#include "bitmap.h"
#include "HDF_Lib.h"

#include "FMM_Lib.h"
#include "bitmap.h"

//-------------------------FMM---------------------------------------//

void FMM_Init()
{
	int i;

	for(i = 0; i < MAXOPENFILES;i++)
	{
		openfiles[i].fp = NULL;
		openfiles[i].header = -1;
		openfiles[i].free = TRUE;
		
	
		headers[i].free = TRUE;
		headers[i].opentimes = 0;
		headers[i].isDirty = FALSE;

		firstEmpty = 0;
	}

}

BOOLEAN FMM_IsValid(int fileDesc)
{
	if((fileDesc>=0)&&(fileDesc<MAXOPENFILES) &&(openfiles[fileDesc].free == FALSE))
	return TRUE;

	else return FALSE;

}


int FMM_GetFileInfoByName(fileInfo_t* finfo)
{
	int p,i;


	for( i = 0 ; i < MAXOPENFILES; i++)
	{

		if( openfiles[i].free == FALSE)
		{
			p = openfiles[i].header;

			//vrethike to arxeio
			if((strcmp(headers[p].filename, finfo->filename))==0)
			{
				finfo->fp = openfiles[i].fp;
				finfo->fd = i;
				return BFE_OK;
			}
		}
	}


	BF_errno = BFE_FILENOTEXISTS;
	return BF_errno;

}

int FMM_GetFileInfoByFD(fileInfo_t* finfo)
{
	int i,p;

	i = finfo->fd;


	if ( FMM_IsValid(i)== FALSE )
	return BFE_FD;

	finfo->fp = openfiles[i].fp;
	
	p = openfiles[i].header;

	strcpy(finfo->filename, headers[p].filename);

	return BFE_OK;
}

BOOLEAN FMM_IsOpen(char* fileName)
{
	int i,p;

	for(i = 0; i < MAXOPENFILES; i++)
	{
		if(openfiles[i].header>=0)
		{
			p = openfiles[i].header;

			if((strcmp(fileName, headers[p].filename))==0)
			return TRUE;
		}
	}
	
	return FALSE;
}


int FMM_Open(char* fileName)
{
	FILE* fp;
	int i,fileDescriptor,p,j,flag;
	char head[BF_BLOCK_SIZE];

	//elegxw prwta an to arxeio einai idi anoixto
	if((FMM_IsOpen(fileName)) == TRUE)
	{
		//printf("%s IS ALREADY OPEN\n", fileName);

		for(i = 0; i < MAXOPENFILES; i++)
		{
			if((strcmp(headers[i].filename, fileName))== 0)
			{
				p = i;

				for( j = 0; j < MAXOPENFILES; j++)
				{
					if(openfiles[j].free == TRUE)	
					{
						openfiles[j].free = FALSE;
						if(HDF_OpenFile(fileName,&openfiles[j].fp)!= BFE_OK)
						return BF_errno;

						headers[i].opentimes++;

						openfiles[j].header = p;
						//printf("POSITION %d TAKEN \n", j);
						return j;

					}
				}
			}
		}
	}

	else if ((FMM_IsOpen(fileName)) == FALSE)
 
	{

		//printf("%s IS NOT OPEN\n", fileName);

		for(i = 0; i < MAXOPENFILES; i++)
		{
			if(openfiles[i].free == TRUE)
			{
				openfiles[i].free = FALSE;
				
				if(HDF_OpenFile(fileName,&openfiles[i].fp)!= BFE_OK)
				return BF_errno;

				for(j = 0; j < MAXOPENFILES; j++)
				{
					if(headers[j].free == TRUE)
					{
						openfiles[j].header = j;
						headers[j].free = FALSE;
						strcpy(headers[j].filename, fileName);
						headers[j].opentimes++;
						

						if((HDF_ReadBlock(0,headers[j].header,openfiles[i].fp)) !=  BFE_OK)
						return BF_errno;

						// printf("POSITION %d TAKEN \n", i);

						return i;
					}
				} 

			}
		}		
	}

	BF_errno = BFE_FTABFULL;

	return BF_errno;
}

int FMM_Close(int fileDesc)
{

	size_t result;
	int p;

	p = openfiles[fileDesc].header;

	if(headers[p].opentimes == 1 )
	{
		//prepei na grapsw tin kefalida sto arxeio
		if(headers[p].isDirty == TRUE)
		{
			if((HDF_WriteBlock(0,headers[p].header, openfiles[fileDesc].fp)) != BFE_OK)
			return BF_errno;

		}

		if((HDF_CloseFile(openfiles[fileDesc].fp)) != BFE_OK)
		return BF_errno;
	
		headers[p].opentimes = 0;

		openfiles[fileDesc].header = -1;
		openfiles[fileDesc].free = TRUE;
		openfiles[fileDesc].fp = NULL;
		
		
		return BFE_OK;

	}

	else if(headers[p].opentimes > 1 )
	{
		if((HDF_CloseFile(openfiles[fileDesc].fp)) != BFE_OK)
		return BF_errno;

		(headers[p].opentimes)--;


		 openfiles[fileDesc].header = -1;
		 openfiles[fileDesc].free = TRUE;
		 openfiles[fileDesc].fp = NULL;

		
		return BFE_OK;
	}

	
	else 
	{	BF_errno = BFE_CANNOTCLOSEFILE;
		return BF_errno;   
	}
}


BOOLEAN FMM_HEAD_IsValid(int fileDesc,int blockNum)
{

	int p;

	if(openfiles[fileDesc].free == TRUE)
	return FALSE;

	 p  = openfiles[fileDesc].header;

//	printf("HEAD IS VALID:HEADER BLOCK = %d\n", p);

	if(( checkbit(headers[p].header, blockNum)) == 1)
	{
		//printf("BIT IS VALID\n");
		return TRUE;
	}

	else 
		{
			//printf("BIT IS INVALID\n");
			return FALSE;
		}



}


int FMM_HEAD_GetFirstBlock(int fileDesc)
{
	int  p,i;

	if(openfiles[fileDesc].free == TRUE)
	{
		BF_errno = BFE_FD;
		return BF_errno;
	}

	p = openfiles[fileDesc].header;

	for(i = 0; i < MAX_BLOCKS; i++)
	{
		if((checkbit(headers[p].header, i)) == 1)
		return i;
	
	}


	BF_errno = BFE_EOF;
	return BF_errno;
}

int FMM_HEAD_GetNextBlock(int fileDesc,int prev)
{
	int p,i;


	if(openfiles[fileDesc].free == TRUE)
	{
		 BF_errno = BFE_FD;
		return BF_errno;

	}


	p = openfiles[fileDesc].header;

	for(i = prev+1; i < MAX_BLOCKS; i++)
	{
		if((checkbit(headers[p].header, i)) == 1)
		return i;
	}


	BF_errno = BFE_EOF;
	return BF_errno;

}

int FMM_HEAD_GetFirstEmpty(int fileDesc)
{

	int p,i;

	 if(openfiles[fileDesc].free == TRUE)
	{
		BF_errno = BFE_FD;
		return BF_errno;

	}


	p = openfiles[fileDesc].header;

	for(i = 0; i < MAX_BLOCKS; i++)
	{
		 if((checkbit(headers[p].header, i)) == 0)
		return i;
	}

	BF_errno = BFE_EOF;
	return BF_errno;

}


int FMM_HEAD_Set(int fileDesc,int blockNum,BOOLEAN value)
{
	int p;

	if(openfiles[fileDesc].free == TRUE)
	{
		BF_errno = BFE_FD;
		return BF_errno;

		
	}

	
	p = openfiles[fileDesc].header;

	//printf("HEADSET:HEADER BLOCK  = %d\n", p);

	if(value == TRUE)
	{
		//elegxw an to bit pou thelw na kanw 1 einai 0
		if(checkbit(headers[p].header,blockNum) == 0)
		{
			 //printf("MPIKE STO IF\n");
			
			setbit(headers[p].header, blockNum);
			headers[p].isDirty = TRUE;

			 //printf("MPIKE STO IF\n");

			return BFE_OK;
		}
	}

	if(value == FALSE)
	{
		if(checkbit(headers[p].header,blockNum) == 1)
		{
			unsetbit(headers[p].header, blockNum);
			headers[p].isDirty = TRUE;
			return BFE_OK;
		}
	}

	BF_errno = -26;

	return BF_errno;
}


