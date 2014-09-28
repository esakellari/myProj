/************************************************
 *						*
 *						*
 *	��������� �������� 1115200600152	*
 *						*
 *						*
 ***********************************************/


#include "BF_Lib.h"
#include "BMM_Lib.h"
#include "FMM_Lib.h"
#include "HDF_Lib.h"

/*����������� ��� ���������� ����� ��� ������������ � ������ ����������� �������. ����� ������������� 
 *����������� / ������������� ������� (��� ������������ ���������� � ��� FMM), ������� ������� �.��.
 */
void BF_Init( void )
{
	FMM_Init();
	BMM_Init();
}

/*� ������� ���� ���������� ��� ������ �� ����� fileName. �� ������ ��� ������ �� ������� ���.
 *� ������� ���������� BFE_OK ��� �� ���������� ������ ������������ �� ��������, ���' ������� ������� ������ ���������.
 */
int BF_CreateFile(char *fileName)
{
	char header[BF_BLOCK_SIZE];
	FILE* fp;

	//dimiourgw to header kai to gemizw me mhdenika
	memset(header, 0, BF_BLOCK_SIZE);

	//elegxw an yparxei idi to arxeio, an yparxei epistrefei error
	if( HDF_ExistsFile(fileName) == TRUE )
	{	
		BF_Errno  = BFE_FILEEXISTS;
		return BF_Errno;  
	}

	//dimiourgia tou arxeiou kai grapsimo tou header stin arxi
	if(HDF_CreateFile(fileName) != BFE_OK)
	return BF_Errno;

	if(HDF_OpenFile(fileName,&fp)!= BFE_OK)
	return BF_Errno;

	if(HDF_WriteBlock(0,header,fp)!= BFE_OK)
	return BF_Errno;

	if(HDF_CloseFile(fp) != BFE_OK)
	return BF_Errno;

	return BFE_OK;

}


/*� ������� ���� ����������� �� ������ fileName, ��������� ��� �� �������� ���. 
 *�� ������ ������ �� ������� ��� ���� �� ��� ����� �������. � ������� ���������� BFE_OK ��� 
 *��������, ���' ������� ������� ������ ���������.
 */
int BF_DestroyFile(char *fileName)
{

	//elegxw an den yparxei to arxeio, tote epistrefei error
	if(HDF_ExistsFile ( fileName ) == FALSE)
	{
		BF_Errno = BFE_FILENOTEXISTS;
		return BF_Errno;	
	}	
	
	//an einai anoixto epistrefei error
	if( FMM_IsOpen(fileName)== TRUE )
	{
		BF_Errno = BFE_CANNOTDESTROYFILE;
		return BF_Errno;
	}

	//adeiazw tin endiamesi mnimi apo block tou arxeiou aytou
	if( BMM_Clear(fileName)!= BFE_OK)
	{
		BF_Errno = BFE_FILEHASFIXEDBLOCKS;
		return BF_Errno;
	}

	if(HDF_DestroyFile(fileName)!= BFE_OK)
	return BF_Errno;
	
	return BFE_OK;

}

/*� ������� ���� ������� �� ������ fileName. ��� �� ������ �������� ��������, � ������� ���������� ���� ����� �� �������� �������,
 *� ������ ��������������� ��� �� ������������� �� ������ (���� ������������ ��������). ���' ������� ���������� ������� ������ ���������.
 */
int BF_OpenFile (char *fileName)
{
	int fd;

	//an den yparxei to arxeio epistrefei lathos
	if(HDF_ExistsFile(fileName) == FALSE)
	{
		BF_Errno = BFE_FILENOTEXISTS;
		return BF_Errno;
	}

	//anoigma tou arxeiou kai epistrofi tou file descriptor
	if((fd=FMM_Open(fileName))<0)
	return BF_Errno;
	
	else return fd;

}

/*� ������� ���� ������� �� ������ �� ������������� ������ fileDesc. ������ ������ ��� ��� ���������� ��� ����������� ��� ������ ���� ���� 
 *������ �������� �������. ���������� BFE_OK ��� �� ������ ������� ��������, ���' ������� ������� ������ ���������. ��� ��� �� �������� 
 *����������� �� �������� ����������� ������� ��� ������ ����� ��� ����� ``���������'' ���� ��������� �����.
 */
int BF_CloseFile (int fileDesc)
{

	fileInfo_t finfo;

	finfo.fd = fileDesc;

	//elegxw an einai valid o filedescriptor pou dothike
	if ( FMM_IsValid ( fileDesc ) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}

	//gemizw me tis plirofories tou arxeioy to finfo
	if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
	return BF_Errno;

	//grafei ta blocks tou arxeiou prin to kleisei
	if(BMM_Flush(finfo)!= BFE_OK)
	return BF_Errno;

	if(FMM_Close(fileDesc)!= BFE_OK)
	return BF_Errno;
	
	return BFE_OK;

}

/*� ������� ���� �������� ���� ��������� ����� �� ����� ������ ����� ��� ������� �� ������������� ������ fileDesc ��� ���� 
 *����� ��� ������ *blockBuf �� ������� �' ����. ������ �������� ��� *blockNum ��� ������ ��� ����� ��� ����������. 
 *�������� ��� ���� ��� ����� ���������� �� ����� ��' ������� 1 (�� ���������� ��� � ������� ��� ����� ������ ��� ����), 
 *������ �� ��������� (��� ���� ��� ����) ������ �� ���� ���������� ��� �� ������ ��� ������� ��� ����������. �� ����� ��� ����������
 *������ �� ``��������'' ��� ����� ��� �� ���������� �' ����� ��� ��������� ����� �� ``����������'' ���� ��� �������� BF_UnpinBlock(), 
 *� ����� ������������ ��������. � ������� ���������� BFE_OK ��� ��������, BFE_EOF ��� ������ ������ ��� ����� ��� ������� (������, ��� �� ������ 
 *��� ���� ������ �����), � ������� ������ ��������� ��� ���� ��� ������������ ���� ������.
 */
int BF_GetFirstBlock(int fileDesc,int *blockNum ,char **blockBuf )
{
	fileInfo_t finfo;

	//elegxos an to fd einai valid
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}

	//epistrefei sto blocknum to prwto egyro block
	if((*blockNum = FMM_HEAD_GetFirstBlock(fileDesc))<0)
	{
		BF_Errno = BFE_EOF;
		return BF_Errno;
	}

	else 
	{
		finfo.fd = fileDesc;

		//gemizw me plirofories ti domi finfo kai vazw to block sti mnimi
		if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
		return BF_Errno;

		if(BMM_GetBlock(finfo,*blockNum,blockBuf)!= BFE_OK)
		return BF_Errno;

	}
	
	return BFE_OK;
}

/*� ������� ���� �������� ���� ��������� ����� �� ����� ������ ����� ��� ������� �� ������������� ������ 
 *fileDesc ���� ��� �� ����� �� ������ *blockNum ��� ���� ����� ��� ������ *blockBuf �� ������� �' ����. ������, ��� ����� ��� �����������,
 *���������� �� *blockNum ���� �� �������� ��� ������ ��� ���� ����� ��� ����������. �������� ��� ���� ��� ���� ��� ����� ���������� 
 *�� ���������� ������ ������� ��� *blockNum, ������ �� ��������� (��� ���� ��� ����) ������ �� ���� ���������� ��� �� ������ ��� ������� 
 *��� ����������. �� ����� ��� ���������� ������ �� ``��������'' ��� ����� ��� �� ���������� �' ����� ��� ��������� ����� �� ``����������'' 
 *���� ��� �������� BF_UnpinBlock(), � ����� ������������ ��������. � ������� ���������� BFE_OK ��� ��������, BFE_EOF ��� ������ ������ ���
 *����� ��� ������� (������ ��� ������� ������� �����), BFE_INVALIDBLOCK ��� � ������ �������� ������� ����� (*blockNum) ��� ���������� �� 
 *������ �����, � ������� ����� ������ ��������� ��� ���� ���� ��� ������������ ���� ������. 
 */
int BF_GetNextBlock(int fileDesc ,int *blockNum ,char **blockBuf )
{
	fileInfo_t finfo;

	//elegxos an to fd einai valid
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}

	//elegxw an to block einai egyro 
	if(FMM_HEAD_IsValid(fileDesc,*blockNum) == FALSE)
	{
		BF_Errno = BFE_INVALIDBLOCK;
		return BF_Errno;
	}

	//epistrefei to prwto egyro block amesws meta to blocknum kai to vazei sto blocknum
	if((*blockNum = FMM_HEAD_GetNextBlock(fileDesc,*blockNum))< 0)
	return BF_Errno;

	else
	{
		finfo.fd = fileDesc;

		//gemizw me plirofories gia to arxeio to finfo kai vazw to block sti mnimi
		if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
		return BF_Errno;

		if(BMM_GetBlock(finfo,*blockNum,blockBuf)!= BFE_OK)
		return BF_Errno;
	}

	return BFE_OK;
}


/*� ������� ���� �������� ���� ��������� ����� �� ����� �� ������ blockNum ��� ������� �� ������������� ������
 *fileDesc ��� ���� ����� ��� ������ *blockBuf �� ������� �' ����. � ������� blockNum ������ �� ���������� �� 
 *������ �����. �� ����� ��� ���������� ������ �� ``��������'' ��� ����� ��� �� ���������� �' ����� ��� ���������
 *����� �� ``����������'' ���� ��� �������� BF_UnpinBlock(), � ����� ������������ ��������. � ������� ����������
 * BFE_OK ��� ��������, BFE_INVALIDBLOCK ��� � ������ �������� ������� ����� (blockNum) ��� ���������� �� ������ 
 *�����, � ������� ����� ������ ��������� ��� ���� ���� ��� ������������ ���� ������. 
 */
int BF_GetThisBlock(int fileDesc,int blockNum,char **blockBuf)
{
	fileInfo_t finfo;

	//elegxw an to fd einai egyro
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}

	//elegxw an to block blocknum einai egyro
	if(FMM_HEAD_IsValid(fileDesc,blockNum) == FALSE)
	{
		BF_Errno = BFE_INVALIDBLOCK;
		return BF_Errno;

	}

	//gemizw me plirofories ti domi kai vazw to block sti mnimi
	finfo.fd = fileDesc;

	if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
	return BF_Errno;
	
	if(BMM_GetBlock(finfo,blockNum,blockBuf)!= BFE_OK)
	return BF_Errno;

	return BFE_OK;

}

/*� ������� ���� ��������� ��� ��� ����� ����� ��� ������ �� ������������� ������ fileDesc. 
 *�������� ��� ������ ��� ���� ����� ��� *blockNum ��� ����� ��� ������ *blockBuf �� �������
 *�� ���������� ���� ���� ��������� �����. �� ��� ����� ������ �� ����� ����������������� � 
 *�� ���������� ��� ����������. ������ �� ��� ����� ������ �� ``��������'' ��� ����� ��� �� 
 *���������� �' ����� ��� ��������� ����� �� ``����������'' ���� ��� �������� BF_UnpinBlock(),
 *� ����� ������������ ��������. � ������� ���������� BFE_OK ��� �������� � ������� ������ 
 *��������� ��� ���� ��� ������������ ���� ������.
 */
int BF_AllocBlock(int fileDesc,int *blockNum ,char **blockBuf)
{

	fileInfo_t finfo;
	
	//elegxos an to fd einai egyro
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}
	
	//epistrefei to prwto mi egyro block
	if((*blockNum=FMM_HEAD_GetFirstEmpty(fileDesc))<0)
	return BF_Errno;

	//gemizw me plirofories ti domi
	finfo.fd = fileDesc;
	
	if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
	return BF_Errno;

	//prosthetw to block sto arxeio
	if(BMM_AllocBlock(finfo,*blockNum,blockBuf) != BFE_OK)
	return BF_Errno;

	//kanw to antistoixo bit tou header 1
	if(FMM_HEAD_Set(fileDesc,*blockNum,TRUE) != BFE_OK)
	return BF_Errno;

	return BFE_OK;
}


/*� ������� ���� ���������� ���� ���������� �� ����� �� ������ blockNum ��� ������� �� ������������� ������ fileDesc. 
 *To ����� ��� ������ �� ����� ``���������'' ���� ��������� �����. � ������� ���������� BFE_OK ��� �������� � �������
 *������ ��������� ��� ���� ��� ������������ ���� ������.
 */
int BF_DisposeBlock(int fileDesc,int blockNum)
{
	 fileInfo_t finfo;

	//elegxos an to fd einai egyro	
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}

	//an to block den einai egyro, epistrefei lathos
	if(FMM_HEAD_IsValid(fileDesc,blockNum) == FALSE)
	{
		BF_Errno = BFE_INVALIDBLOCK;
		return BF_Errno;
	}

	//gemizw me plirofories gia to arxeio to finfo
	finfo.fd = fileDesc;

	if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
	return BF_Errno;

	//diwxnw t block apo ti mnimi
	if(BMM_DisposeBlock(finfo,blockNum)!= BFE_OK)
	return BF_Errno;
	
	//thetei to antistoixo bit tou header se 0
	if(FMM_HEAD_Set(fileDesc,blockNum,FALSE)!= BFE_OK)
	return BF_Errno;
	
	return BFE_OK;

}

/*� ������� ���� ��������� ��� �� ����� �� ������ blockNum ��� ������� �� ������������� ������ fileDesc ��� ���������� ��� ����
 *��������� ����� ��� ������ �� ``����������''. � ���������� dirty ����� ��� �� TRUE=1 ��� �� ����� ���� ������� ���� ��������� ����� ��� �� ������ ��� 
 *``���������'' ���� (����� �� ��������� �������������� ��� ������ ����� �� ������� ��� ���� ��� ��� �����), ���' ������� ����� ��� �� FALSE=0. 
 *� ������� ���������� BFE_OK ��� �������� � ������� ������ ��������� ��� ���� ��� ������������ ���� ������. 
 */
int BF_UnpinBlock(int fileDesc,int blockNum,int dirty)
{
	fileInfo_t finfo;
         //metavliti pou elegxei an to block einai dirty i oxi
	BOOLEAN dirt;

	//elegxos an to fd einai valid
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}
	
	//elegxos an to block einai egyro
	if(FMM_HEAD_IsValid ( fileDesc, blockNum ) == FALSE)
	{
		BF_Errno = BFE_INVALIDBLOCK;
		return BF_Errno;
	}

	//an den exei ginei allagi sto block thetw ti metavliti dirt =  false
	if(dirty == 0)
	dirt = FALSE;

	//an exei ginei allagi ti thetei true
	else dirt = TRUE;

	//gemizei me plirofories to finfo kai ginetai unpin to block
	finfo.fd = fileDesc;

	if ( FMM_GetFileInfoByFD ( &finfo ) != BFE_OK )	
	return BF_Errno;

	if(BMM_UnpinBlock(finfo,blockNum,dirt) != BFE_OK)
	return BF_Errno;

	return BFE_OK;

}

/*� ������� ������� �� ������� ��� ������� � ���������� errString, ��� ���� ������� �� ������ ��� ����������� 
 *��� ��������� ������ ��� �������� ��� ����������� ��� ��� ��������. ��� ��� ����� ����, � ������� ���� 
 *������������ ��� �������� (global) ��������� BF_errno � ����� ���������� ����� ��� ������ ��� ����� ���������
 *���������. � ������� ����� ��������� ������ ����� �� ������������ ����� �� ���� ��� ����� ��������.
 *� ������� ���� ��� ���� ���� ��� ���� ����������.
 */
void BF_PrintError(char *errString )
{
	printf("%s\n",errString);

	 if(BF_Errno == BFE_NOMEM )
	puts("No memory available");              /* ��� ������� ��������� ����� */

	if (BF_Errno==BFE_CANNOTOPENFILE)
	puts("Cannot open file");                 /* ������� �� ������� ��� ������� */

	if( BF_Errno==BFE_CANNOTCLOSEFILE)
	puts("Cannot Close file");                /* ������� �� �������� ��� ������� */

	if (BF_Errno == BFE_CANNOTCREATEFILE)
	puts("Cannot create file");              /* ������� � ���������� ��� ������� */

	if(BF_Errno== BFE_INCOMPLETEREAD)
	puts("Incomplete read");                 /* �������� �������� */

	if (BF_Errno == BFE_INCOMPLETEWRITE)
	puts("Incomplete write");                /* �������� ������� */

	if(BF_Errno == BFE_FILEEXISTS)
	puts("File already exists");              /* �� ������ ������� ��� */

	if(BF_Errno == BFE_NOBUF)
	puts("No space in buffer available");     /* ������� ����� ���� ��������� ����� */

	if(BF_Errno==BFE_FILEOPEN)
	puts("File already open");                /* �� ������ ����� ��� ������� */

	if(BF_Errno==BFE_FD)
	puts("Wrong fd");                          /* ���������� ������������� ������� */

	if(BF_Errno == BFE_FILENOTEXISTS)
	puts("file does not exist");               /* �� ������ ��� ������� */

	if(BF_Errno == BFE_FTABFULL)
	puts("Tab is full");                        /* ����� �������� ������� ������ */

	if(BF_Errno == BFE_HEADOVERFLOW)
	puts("Head overflow");                      /* ����������� �������  */

	if(BF_Errno== BFE_BLOCKFIXED)
	puts("Block already fixed");                /* ����� ��� "���������" ��� ����� */

	if(BF_Errno == BFE_BLOCKUNFIXED)
	puts("Block already unfixed");              /* ����� ��� ����������� */

	if(BF_Errno==BFE_EOF)
	puts("End of file");                        /* ����� ������� */

	if(BF_Errno== BFE_FILEHASFIXEDBLOCKS)
	puts("the file has fixed blocks");          /* �� ������ ���� ��������� block */

	if(BF_Errno==BFE_BLOCKFREE)
	puts("Block already free");	                /* ����� ��� ��������� ���� ���������� */

	if(BF_Errno == BFE_BLOCKINBUF)
	puts("Block already in buffer");            /* ����� ��� ��� ����� */

	if(BF_Errno == BFE_BLOCKNOTINBUF);
	puts("Block not in buffer");                /* ����� ��� ��� ����� */

	if(BF_Errno == BFE_INVALIDBLOCK)
	puts("invalid block code");                 /* ����������� ������� block */

	if(BF_Errno == BFE_CANNOTDESTROYFILE)
	puts("Cannot destroy file");                /* ������� � ���������� ��� ������� */


}
