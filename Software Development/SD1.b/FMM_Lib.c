#include "bitmap.h"
#include "HDF_Lib.h"

#include "FMM_Lib.h"


//-------------------------FMM---------------------------------------//


//synartisi pou arxikopoiei tis metavlites twn pinakwn gia ta anoigmata
//kai tis kefalides.Thetei tis metavlites pou deixnoune ama einai adeia i thesi ises
//me TRUE, kanei to deikti gia tis theseis tou pinaka header iso me -1, kai tin meta
//vliti pou deixnei ama exei ginei allagi sto header isi me FALSE
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
	}

}


//i synartisi elegxei ama to file descriptor p exei dwthei einai swsto, dld mikrote-
//ro apo MAXOPENFILES, thetiko kai ama i sygekrimeni thesi einai adeia 
BOOLEAN FMM_IsValid(int fileDesc)
{
	if((fileDesc>=0)&&(fileDesc<MAXOPENFILES) &&(openfiles[fileDesc].free == FALSE))
	return TRUE;

	else return FALSE;

}

//gemizei tis plirofories tis domis finfo me ta stoixeia pou leipoune gia to arxeio
//tou opoiou dinetai to onoma
int FMM_GetFileInfoByName(fileInfo_t* finfo)
{
	int p,i;


	//psaxnw ston pinaka anoigmatwn
	for( i = 0 ; i < MAXOPENFILES; i++)
	{

		//elegxw mono oses theseis einai kateilhmenes
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

	//an de vrethike to arxeio epistrefei kwdiko sfalmatos
	BF_errno = BFE_FILENOTEXISTS;
	return BF_errno;

}

//gemizei me plirofories gia to arxeio ti domi finfo 
//symfwna me to file descriptor pou exei dwthei
int FMM_GetFileInfoByFD(fileInfo_t* finfo)
{
	int i,p;

	i = finfo->fd;


	//elegxw an o file descriptor pou dwthike einai swstos
	if ( FMM_IsValid(i)== FALSE )
	{
		BF_errno =  BFE_FD;
		return BF_errno;
	}

	finfo->fp = openfiles[i].fp;
	
	p = openfiles[i].header;

	strcpy(finfo->filename, headers[p].filename);

	return BFE_OK;
}

//elegxei an to arxeio me onoma fileName einai anoixto i oxi
BOOLEAN FMM_IsOpen(char* fileName)
{
	int i,p;

	//psaxnw oles tis theseis tou pinaka anoigmatwn
	for(i = 0; i < MAXOPENFILES; i++)
	{

		//elegxw mono oses theseis antistoixoun se kapoia thesi tou 
		//pinaka kefalidwn kai einai gemates
		if((openfiles[i].header>=0) && ( openfiles[i].free == FALSE ) )
		{

			p = openfiles[i].header;
			
			//an vrethei to arxeio 
			if((strcmp(fileName, headers[p].filename))==0)
			return TRUE;
		}
	}
	
	return FALSE;
}

//synartisi anoigmatos arxeiou
int FMM_Open(char* fileName)
{
	FILE* fp;
	int i,fileDescriptor,p,j,flag;
	char head[BF_BLOCK_SIZE];

	//elegxw prwta an to arxeio einai idi anoixto
	if((FMM_IsOpen(fileName)) == TRUE)
	{
		for(i = 0; i < MAXOPENFILES; i++)
		{
			//elegxw apo ton pinaka kefalidwn an einai to arxeio pou psaxnw symfwna 
			//me to onoma
			if((strcmp(headers[i].filename, fileName))== 0)
			{
				p = i;

				//psaxnw ston pinaka anoigmatwn gia mia eleytheri thesi
				for( j = 0; j < MAXOPENFILES; j++)
				{
					if(openfiles[j].free == TRUE)	
					{//vrika eleytheri thesi

						openfiles[j].free = FALSE;
						if(HDF_OpenFile(fileName,&openfiles[j].fp)!= BFE_OK)
						return BF_errno;
			
						 strcpy(headers[i].filename, fileName);

						//ayksanw ton metriti pou deixnei poses fores exw anoiksei to arxeio
						headers[i].opentimes++;

						openfiles[j].header = p;
				
						return j;

					}
				}
			}
		}
	}

	else if ((FMM_IsOpen(fileName)) == FALSE)
 
	{   //to arxeio den einai idi anoixto

		//psaxnw adeia thesi ston pinaka anoigmatwn
		for(i = 0; i < MAXOPENFILES; i++)
		{
			if(openfiles[i].free == TRUE)
			{
				openfiles[i].free = FALSE;
				
				if(HDF_OpenFile(fileName,&openfiles[i].fp)!= BFE_OK)
				return BF_errno;

				//psaxnw adeia thesi ston pinaka kedalidwn
				for(j = 0; j < MAXOPENFILES; j++)
				{
					if(headers[j].free == TRUE)
					{
						openfiles[j].header = j;
						headers[j].free = FALSE;

						strcpy(headers[j].filename, fileName);

						//ayksanw ton metriti anoigmatwn
						headers[j].opentimes++;
						
						//diavazw to header kai to vazw stin thesi tou pinaka twn header
						if((HDF_ReadBlock(0,headers[j].header,openfiles[i].fp)) !=  BFE_OK)
						return BF_errno;


						return i;
					}
				} 

			}
		}		
	}

	BF_errno = BFE_FTABFULL;

	return BF_errno;
}


//synartisi kleisimatos arxeiou
int FMM_Close(int fileDesc)
{

	size_t result;
	int p;


	//elegxos an einai valid o fd
	if(FMM_IsValid(fileDesc)  == FALSE)
	{
		BF_errno = BFE_FD;
		return BF_errno;
	}

	p = openfiles[fileDesc].header;

	//an einai i teleytaia fora p kleinw to arxeio
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
	
		//rythmizw kai tis ypoloipes parametrous tis domis header kai openfile
		headers[p].opentimes = 0;
		headers[p].free =  TRUE;

		openfiles[fileDesc].header = -1;
		openfiles[fileDesc].free = TRUE;
		openfiles[fileDesc].fp = NULL;
		
		
		return BFE_OK;

	}

	//an den einai i teleytaia fora pou kleinw to arxeio 
	else if(headers[p].opentimes > 1 )
	{
		if((HDF_CloseFile(openfiles[fileDesc].fp)) != BFE_OK)
		return BF_errno;

		//meiwnw ton metriti anoigmatwn
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

//synartisi pou elegxei an ena blok einai egyro
BOOLEAN FMM_HEAD_IsValid(int fileDesc,int blockNum)
{

	int p;

	if( FMM_IsValid(fileDesc) == FALSE)
	return FALSE;

	 p  = openfiles[fileDesc].header;

	//elegxw an to antistoixo bit tou bitmap einai 0 i 1 
	if(( checkbit(headers[p].header, blockNum)) == 1)
	return TRUE;
	
	else return FALSE;
		
}

//epistrefei to prwto egyro block
int FMM_HEAD_GetFirstBlock(int fileDesc)
{
	int  p,i;

	if(FMM_IsValid(fileDesc) == FALSE)
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

//epistrefei to prwto egyro block meta to prev
int FMM_HEAD_GetNextBlock(int fileDesc,int prev)
{
	int p,i;


	if(FMM_IsValid(fileDesc) == FALSE)
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


//epistrefei to prwto mi egyro block
int FMM_HEAD_GetFirstEmpty(int fileDesc)
{

	int p,i;

	 if(FMM_IsValid(fileDesc) == FALSE)
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

	if(FMM_IsValid(fileDesc) == FALSE)
	{
		BF_errno = BFE_FD;
		return BF_errno;
	}

	p = openfiles[fileDesc].header;

	if(value == TRUE)
	{
		//elegxw an to bit pou thelw na kanw 1 einai 0
		if(checkbit(headers[p].header,blockNum) == 0)
		{
			setbit(headers[p].header, blockNum);
			//thetw true ti metavliti pou deixnei oti exei ginei allagi sto header
			headers[p].isDirty = TRUE;

			return BFE_OK;
		}
	}

	if(value == FALSE)
	{
		//elegxw an to bit pou thelw na kanw 0 einai 1
		if(checkbit(headers[p].header,blockNum) == 1)
		{
			unsetbit(headers[p].header, blockNum);
			headers[p].isDirty = TRUE;
			return BFE_OK;
		}
	}

	BF_errno = BFE_WRONGHEADSET;

	return BF_errno;
}


