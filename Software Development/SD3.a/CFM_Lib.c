#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

#include "CFM_Lib.h"
#include "HF_Lib.h"

#include "UT_DM_Defines.h"


/*
	relCatFd  -> Το αναγνωριστικό του ανοιχτού αρχείου relCat 
	attrCatFd -> Το αναγνωριστικό του ανοιχτού αρχείου attrCat
*/

int CFM_Create()
{
	int offset;
	relDesc *relBuf;
	attrDesc *attrBuf;
	
	
	relBuf = malloc(sizeof(relDesc));
	if(relBuf == NULL)
		return (UT_errno = UTE_NOMEM);


	/*1. eisagwgi tis grammis tou (relCat) ston pinaka relCat */
	memset(relBuf,'\0', sizeof(relDesc));
	
	strcpy(relBuf->relname,"relCat");
	relBuf->relwidth = sizeof(relDesc);	/* megethos grammis tou relCat */
	relBuf->attrcnt = 4;				/* sunolo pediwn tou relCat */
	relBuf->indexcnt = 0;				/* sunolo euretiriwn tou relCat */

	
	if( HF_InsertRec(relCatFd, (char *)relBuf, sizeof(relDesc)) < 0 )	
		return (UT_errno = UTE_HFERROR);


	/*2. eisagwgi tis grammis tou (attrCat) ston pinaka relCat */
	memset(relBuf,'\0', sizeof(relDesc));
	
	strcpy(relBuf->relname,"attrCat");
	relBuf->relwidth = sizeof(attrDesc);	/* megethos grammis tou attrCat */
	relBuf->attrcnt = 7;					/* sunolo pediwn tou attrCat */
	relBuf->indexcnt = 0;					/* sunolo euretiriwn tou attrCat */
	
	if( HF_InsertRec(relCatFd,(char *)relBuf, sizeof(relDesc)) < 0 )
		return (UT_errno = UTE_HFERROR);

	free(relBuf);

/************************** Eisagwgi grammwn tou attrCat gia tous pinakes relCat-attrCat ******************************/


/*-----------------------------------1-----------------------------------------*/
	

	attrBuf = malloc(sizeof(attrDesc));
	if(attrBuf == NULL)
		return (UT_errno = UTE_NOMEM);

	/* eisagw ston attrCat tis grammes pou antistoixoun sta pedia tou attrCat*/
	
	/*--- pedio relname tou attrCat ---*/
	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "attrCat");
	strcpy(attrBuf->attrname, "relname");
	attrBuf->offset = 0;
	attrBuf->attrlength = MAXNAME;
	attrBuf->attrtype = 'c';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd,(char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
		
	/*--- pedio attrname tou attrCat ---*/
	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "attrCat");
	strcpy(attrBuf->attrname, "attrname");
	attrBuf->offset = MAXNAME;
	attrBuf->attrlength = MAXNAME;
	attrBuf->attrtype = 'c';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd, (char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
		
	
	/*--- pedio offset tou attrCat ---*/
	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "attrCat");
	strcpy(attrBuf->attrname, "offset");
	attrBuf->offset = 2*MAXNAME;
	attrBuf->attrlength = sizeof(int);
	attrBuf->attrtype = 'i';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd, (char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
		
	/*--- pedio attrlength tou attrCat ---*/
	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "attrCat");
	strcpy(attrBuf->attrname, "attrlength");
	attrBuf->offset = 2*MAXNAME + sizeof(int);
	attrBuf->attrlength = sizeof(int);
	attrBuf->attrtype = 'i';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd, (char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
		
	
	/*--- pedio attrtype tou attrCat ---*/
	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "attrCat");
	strcpy(attrBuf->attrname, "attrtype");
	attrBuf->offset = 2*MAXNAME + 2*sizeof(int);
	attrBuf->attrlength = sizeof(char);
	attrBuf->attrtype = 'c';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd, (char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
		
	/*--- pedio indexed tou attrCat ---*/
	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "attrCat");
	strcpy(attrBuf->attrname, "indexed");
	attrBuf->offset = 2*MAXNAME + 2*sizeof(int) + sizeof(char);
	attrBuf->attrlength = sizeof(int);
	attrBuf->attrtype = 'i';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd, (char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
	
	/*--- pedio indexno tou attrCat ---*/
	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "attrCat");
	strcpy(attrBuf->attrname, "indexno");
	attrBuf->offset =  2*MAXNAME + 3*sizeof(int) + sizeof(char);
	attrBuf->attrlength = sizeof(int);
	attrBuf->attrtype = 'i';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd, (char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
		
	
/*----------------------------------------------------------------------------
		Eisagoume tis eggrafes ston pinaka attrCat gia ta pedia tou relCat 
  -----------------------------------------------------------------------------*/
  	
	/*--- pedio relname tou relCat ---*/

	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "relCat");
	strcpy(attrBuf->attrname, "relname");
	attrBuf->offset = 0;
	attrBuf->attrlength = MAXNAME;
	attrBuf->attrtype = 'c';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd, (char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
		
	/*--- pedio relwidth tou relCat ---*/
	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "relCat");
	strcpy(attrBuf->attrname, "relwidth");
	attrBuf->offset = MAXNAME;
	attrBuf->attrlength = sizeof(int);
	attrBuf->attrtype = 'i';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd,(char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
		
	/*--- pedio attrcnt tou relCat ---*/
	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "relCat");
	strcpy(attrBuf->attrname, "attrcnt");
	attrBuf->offset = MAXNAME+sizeof(int);
	attrBuf->attrlength = sizeof(int);
	attrBuf->attrtype = 'i';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd, (char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
		
	/*--- pedio indexcnt tou relCat ---*/
	memset(attrBuf,'\0', sizeof(attrDesc));
	
	strcpy(attrBuf->relname, "relCat");
	strcpy(attrBuf->attrname, "indexcnt");
	attrBuf->offset = MAXNAME+2*sizeof(int);
	attrBuf->attrlength = sizeof(int);
	attrBuf->attrtype = 'i';
	attrBuf->indexed = 0;
	attrBuf->indexno = -1;

	if( HF_InsertRec(attrCatFd, (char *) attrBuf, sizeof(attrDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
		
	free(attrBuf);

	return UTE_OK;
}
	

int CFM_AddRel(relDesc rel, attrDesc* attrs)
{
	int i;
	
	/* eisagoume ta stoixeia tou rel sto arxeio relCat */

	if( HF_InsertRec(relCatFd,(char *)&(rel), sizeof(relDesc)) < 0 )
 		return (UT_errno = UTE_HFERROR);
	
	/* eisagoume ta stoixeia tou *attrs sto arxeio attrCat */
	
	for (i = 0; i < rel.attrcnt; i++)
	{
		if( HF_InsertRec(attrCatFd, (char *)(&(attrs[i])), sizeof(attrDesc)) < 0 )
 			return (UT_errno = UTE_HFERROR);
	}
	return UTE_OK;
}


int CFM_DeleteRel(char* relName)
{
	int i, sd, recId, attrcnt;
	relDesc *relBuf;
	attrDesc *attrBuf;

	/* psaxnoume eggrafes tou pinaka relCat me prwto pedio idio me to relName*/
	if((sd = HF_OpenFileScan(relCatFd, sizeof(relDesc), 'c', MAXNAME, 0, EQUAL, relName )) < 0 )
		return (UT_errno = UTE_HFERROR);

	relBuf = malloc(sizeof(relDesc));
	if( relBuf == NULL )
		return (UT_errno = UTE_NOMEM);
		
	if((recId = HF_FindNextRec(sd, (char *)relBuf)) < 0 )
		return (UT_errno = UTE_HFERROR);

	/* diagrafoume tin grammi */
	if(HF_DeleteRec(relCatFd, recId, sizeof(relDesc)) != HFE_OK )
		return (UT_errno = UTE_HFERROR);

	attrcnt = relBuf->attrcnt;   /* edw kratame ton arithmo twn pediwn tou relName pinaka */

	if(HF_CloseFileScan(sd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
		
	free(relBuf);

	/* psaxnoume eggrafes tou pinaka attrCat me prwto pedio idio me to relName*/
	
	if((sd = HF_OpenFileScan(attrCatFd, sizeof(attrDesc), 'c', MAXNAME, 0, EQUAL, relName )) < 0 )
		return (UT_errno = UTE_HFERROR);

	attrBuf = malloc(sizeof(attrDesc));
	if( attrBuf == NULL )
		return (UT_errno = UTE_NOMEM);


	for(i = 1;i <= attrcnt; i++) {	/* sigoura tha exoume toses grammes */
		memset(attrBuf,'\0',sizeof(attrDesc));
		if((recId = HF_FindNextRec(sd, (char *)attrBuf)) < 0 )
			return (UT_errno = UTE_HFERROR);
		if(HF_DeleteRec(attrCatFd, recId, sizeof(attrDesc)) != HFE_OK )
			return (UT_errno = UTE_HFERROR);
		
	}

	if(HF_CloseFileScan(sd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
	free(attrBuf);

	return UTE_OK;
}


/*
	An to indexNo einai -1 tote svinoume to euretirio apo tin grammi
	An exei kapoia timi >= 0 tote prepei na enimerwsoume kapoia i ola apo ta pedia
	indexed kai indexNo - indexcnt twn attrCat - relCat antistoixa 
*/
int CFM_IndexAttribute(char* relName, char* attrName, int indexNo)
{
	int indexno, i, sd, recId, refreshIcnt;
	int found = 0;
	relDesc *relBuf;
	attrDesc *attrBuf;

	/* psaxnoume eggrafes tou pinaka attrCat me prwto pedio idio me to relName*/
	if((sd = HF_OpenFileScan(attrCatFd, sizeof(attrDesc), 'c', MAXNAME, 0, EQUAL, relName )) < 0 )
		return (UT_errno = UTE_HFERROR);

	attrBuf = malloc(sizeof(attrDesc));
	if( attrBuf == NULL )
		return (UT_errno = UTE_NOMEM);


	while((recId = HF_FindNextRec(sd,(char *)attrBuf)) >= 0)
	{

		//an vrikame to attrubute pou psaxnoume
		if(strncmp(attrBuf->attrname, attrName, MAXNAME) == 0)
		{
			found = 1;
			/* sto indexno kratame tin timi pou eixe sto indexno to pedio attrname tou relname pinaka sto attrCat */
			indexno = attrBuf->indexno;

			if(indexno == -1)	// ean den eixe euretirio
			{
				if (indexNo >= 0)  // ean pame na tou baloume euretirio
				{
					attrBuf->indexno = indexNo;
					refreshIcnt = 1; /* na prostethei 1 sto indexcnt tou relCat tou relname */
					attrBuf->indexed = 1; /* pleon exei index */
				}
				else
					refreshIcnt = 0;
			}
			else	// ean eixe euretirio
			{
				if (indexNo == -1)	// an pame na to diagrapsoume
				{
					attrBuf->indexno = -1;
					attrBuf->indexed = 0;
					refreshIcnt = -1; /* na prostethei -1 sto indexcnt tou relCat tou relname */
				}
				else	/* ean pame na allaksoume to euretirio*/
				{
					attrBuf->indexno = indexNo;
					refreshIcnt = 0;
				}
			}

			// diagrafoume tin grammi gia na tin ksanaeisagoume
			if (HF_DeleteRec(attrCatFd, recId, sizeof(attrDesc))!= HFE_OK)
				return (UT_errno = UTE_HFERROR);

			if(HF_InsertRec(attrCatFd, (char *)attrBuf, sizeof(attrDesc)) < 0)
				return (UT_errno = UTE_HFERROR);

			break;			
		}
		else
			memset(attrBuf,'\0',sizeof(attrDesc));
	}
	
	if (!found)
		return (UT_errno = UTE_REL_ATTRNOTFOUND);

	if(HF_CloseFileScan(sd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	free(attrBuf);
	
	if (refreshIcnt != 0) {		// ama allakse i timi tou index count
	
		/* psaxnoume eggrafes tou pinaka relCat me prwto pedio idio me to relName*/
		if((sd = HF_OpenFileScan(relCatFd, sizeof(relDesc), 'c', MAXNAME, 0, EQUAL, relName )) < 0 )	//1:equal
			return (UT_errno = UTE_HFERROR);

		relBuf = malloc(sizeof(relDesc));
		if( relBuf == NULL )
			return (UT_errno = UTE_NOMEM);

		if ((recId = HF_FindNextRec(sd,(char *)relBuf)) < 0)
			return (UT_errno = UTE_HFERROR);

		relBuf->indexcnt += refreshIcnt;

		if (HF_DeleteRec(relCatFd,recId, sizeof(relDesc)) != HFE_OK)
			return (UT_errno = UTE_HFERROR);
		
		if (HF_InsertRec(relCatFd, (char *)relBuf, sizeof(relDesc)) < 0)
			return (UT_errno = UTE_HFERROR);
	
		if (HF_CloseFileScan(sd) != HFE_OK)
			return (UT_errno = UTE_HFERROR);
			
		free(relBuf);
	}

	return UTE_OK;
	
}


int CFM_Lookup(char *relName,relDesc *rel, attrDesc **attrs)
{
	int sd, recId,i,counter = 0;
	
	/* psaxnoume eggrafes tou pinaka relCat me prwto pedio idio me to relName*/
	if((sd = HF_OpenFileScan(relCatFd, sizeof(relDesc), 'c', MAXNAME, 0, EQUAL, relName )) < 0 )
		return (UT_errno = UTE_HFERROR);
	
	if (HF_FindNextRec(sd,(char *)rel) < 0)
		return (UT_errno = UTE_HFERROR);

	if (HF_CloseFileScan(sd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
	//-------- Pire apto relCat tis aparaitites plirofories.
	
	//-------- Pairnei plirofories apto arxeio "attrCat".
	/* kanw alloc rel->attrcnt attrDescs */
	
	if ((*attrs = malloc(rel->attrcnt * sizeof(attrDesc))) == NULL)
		return (UT_errno = UTE_NOMEM);

	/* psaxnoume eggrafes tou pinaka attrCat me prwto pedio idio me to relName*/
	if((sd = HF_OpenFileScan(attrCatFd, sizeof(attrDesc), 'c', MAXNAME, 0, EQUAL, relName )) < 0 )
		return (UT_errno = UTE_HFERROR);

	i = 0;
	for (i = 0; i < rel->attrcnt; i++) {
		if (i >= rel->attrcnt)
			return (UT_errno = UTE_OUTOFBOUNDS);

		if (HF_FindNextRec(sd,(char *)(&(*attrs)[i])) < 0)
			break;
	}
		
	if ( i != rel->attrcnt)
		return (UT_errno = UTE_ATTRDATAMISSING);

	if (HF_CloseFileScan(sd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
	
	return UTE_OK;

}











