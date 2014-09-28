#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DM_Lib.h"
#include "SCM_Lib.h"
#include "CFM_Lib.h"
#include "AM_Lib.h"
#include "HF_Lib.h"
#include "UT_Lib.h"

#include "UT_DM_Defines.h"


int DM_insert(int argc, char* argv[])
{
	int fd,rec,i,j,wrt,ifd;
	int *indexedAttrs, *indexedValues;
	char *recBuf,*am_value;

	int ival;
	float fval;

	
	relDesc rel;
	attrDesc *attrs;


	if(SCM_insert(argc, argv) != UTE_OK)			// Elegxw ta argv values
		return UT_errno;

	if(CFM_Lookup(argv[1], &rel, &attrs) != UTE_OK)	// Pairnw plirofories gia to arxeio argv[1] apo relCat-attrCat
		return UT_errno;

	if((fd = HF_OpenFile(argv[1])) < 0)
		return (UT_errno = UTE_HFERROR);

	if ((recBuf = malloc(rel.relwidth)) == NULL)
		return (UT_errno = UTE_NOMEM);

	if ((indexedAttrs = malloc(rel.attrcnt*sizeof(int))) == NULL) // edw tha kratisw tis times tou j tou index 
		return (UT_errno = UTE_NOMEM);							// gia kathe attrs[j] pedio pou exei euretirio
	if ((indexedValues = malloc(rel.attrcnt*sizeof(int))) == NULL) // edw tha kratisw tis times tou index tou argv (i+1 ) tis value dld
		return (UT_errno = UTE_NOMEM);							// gia kathe attrs[j] pedio pou exei euretirio
	
	wrt = 0;
	for (i = 0; i < rel.attrcnt; i++) {
		indexedAttrs[i] = -1;
		indexedValues[i] = -1;
	}

	
	for(i=2 ; i<argc ; i+=2)						// gia kathe pedio
	{
		for(j=0 ; j<rel.attrcnt ; j++)
			if(strcmp(attrs[j].attrname, argv[i]) == 0) 
				break;		// ean to brika ston pinaka attrs (attrCat records)

		if(j == rel.attrcnt) // ean den to brika
			return (UT_errno = UTE_WRONGATTRNUMS);

		if (attrs[j].indexed == 1) {
			indexedAttrs[wrt] = j;
			indexedValues[wrt++] = i+1;
		}

		if(attrs[j].attrtype == 'c')
		{
			memcpy((void *)(recBuf+attrs[j].offset), (void *)argv[i+1], attrs[j].attrlength);
		}

		else if(attrs[j].attrtype == 'i')
		{
			ival = atoi(argv[i+1]);
			memcpy((void *)(recBuf+attrs[j].offset), (void *)&ival, sizeof(int) );
		}

		else if(attrs[j].attrtype == 'f')
		{
			fval = atof(argv[i+1]);
			memcpy((void *) (recBuf+attrs[j].offset), (void *)&fval, sizeof(float) );
		}

	}

	if((rec = HF_InsertRec(fd, recBuf, rel.relwidth)) < 0)
		return (UT_errno = UTE_HFERROR);

	if(HF_CloseFile(fd) != HFE_OK)		// kleinw to arxeio argv[1]
		return (UT_errno = UTE_HFERROR);

	for(i = 0; i < wrt; i++)	// gia ta pedia pou eixan euretirio
	{
		j = indexedAttrs[i];

		if ((ifd = AM_OpenIndex (argv[1], attrs[j].indexno)) < 0)
			return (UT_errno = UTE_AMERROR);

		if (attrs[j].attrtype == 'i') {
			ival = atoi(argv[indexedValues[i]]);
			am_value = (char *)(&ival);
		}
		else if (attrs[j].attrtype == 'f') {
			fval = atof(argv[indexedValues[i]]);
			am_value = (char *)(&fval);
		}
		else if (attrs[j].attrtype == 'c')
			am_value = argv[indexedValues[i]];
			
		/* bazw sto euretirio to zeugari timi-recid */
		if (AM_InsertEntry(ifd, attrs[j].attrtype, attrs[j].attrlength, am_value, rec) < 0)
			return (UT_errno = UTE_AMERROR);

		if(AM_CloseIndex (ifd) != AME_OK)
			return (UT_errno = UTE_AMERROR);
	}

	free(recBuf);
	free(indexedAttrs);
	free(indexedValues);

	return UTE_OK;
}


int DM_delete(int argc, char* argv[])
{
	int recId,i,j,op;
	int amfd,hffd,amsd,hfsd,amfd2;

	char *condVal, *recbuf,*am_value;
	char attrType;
	int attrLength,attrOffset,indexed,indexno,ival,ival2;
	float fval,fval2;
	
	relDesc rel;
	attrDesc *attrs;
	
	condVal = NULL;
	op =-1;
	attrLength = -1;
	attrOffset = -1;
	attrType = 0;
	indexed = -1;

	if(SCM_delete(argc, argv) != UTE_OK)
		return UT_errno;

	if(CFM_Lookup(argv[1], &rel, &attrs) != UTE_OK)
		return UT_errno;

	if((hffd = HF_OpenFile(argv[1])) < 0)
		return (UT_errno = UTE_HFERROR);

	if(argc > 2) {	//an exei kapoia sunthiki
		if (strcmp(argv[argc-2], ">") == 0)
			op = GREATER_THAN;
		else if (strcmp(argv[argc-2], ">=") == 0)
			op = GREATER_THAN_OR_EQUAL;
		else if (strcmp(argv[argc-2], "=") == 0)
			op = EQUAL;
		else if (strcmp(argv[argc-2], "<") == 0)
			op = LESS_THAN;
		else if (strcmp(argv[argc-2], "<=") == 0)
			op = LESS_THAN_OR_EQUAL;
		else if (strcmp(argv[argc-2], "!=") == 0)
			op = NOT_EQUAL;

		for(i = 0; i < rel.attrcnt; i++) {	// briskw tin kataxwrisi tou pediou tis sunthikis ston attrCat
			if(strcmp(attrs[i].attrname, argv[argc-3]) == 0) {
				attrType = attrs[i].attrtype;
				attrLength = attrs[i].attrlength;
				attrOffset = attrs[i].offset;
				indexed = attrs[i].indexed;
				indexno = attrs[i].indexno;
				break;
			}
		}
	
	
		if (attrType == 'i') {
			ival = atoi(argv[argc-1]);
			condVal = (char *)(&ival);
		}
		else if (attrType == 'f') {
			fval = atof(argv[argc-1]);
			condVal = (char *)(&fval);
		}
		else if (attrType == 'c')
			condVal = argv[argc-1];
	}
	if (indexed == 1) {	// ean exei euretirio to pedio ths sunthikis
		if ( (amfd = AM_OpenIndex(argv[1],indexno)) < 0)
			return (UT_errno = UTE_AMERROR);
		
		if ( (amsd = AM_OpenIndexScan(amfd, attrType, attrLength, op, condVal)) < 0)
			return (UT_errno = UTE_AMERROR);
	}
	else {
		if ( (hfsd = HF_OpenFileScan(hffd,rel.relwidth,attrType,attrLength,attrOffset,op,condVal)) < 0 )
			return (UT_errno = UTE_HFERROR);
	}

	
	if( (recbuf = malloc(rel.relwidth*sizeof(char))) == NULL )
		return ( UT_errno = UTE_NOMEM);

	while(1) {

		if (indexed == 1) {
			if ( (recId = AM_FindNextEntry(amsd)) < 0)
				break;
			if (HF_GetThisRec(hffd, recId, recbuf, rel.relwidth) != HFE_OK)
				return (UT_errno = UTE_HFERROR);				
		}
		else {
			if ( (recId = HF_FindNextRec(hfsd,recbuf)) < 0)
				break;
		}

		for(j = 0; j < rel.attrcnt; j++)	// gia kathe pedio tis grammis p paw na diagrapsw
		{
			if(attrs[j].indexed == 1)		// an exei euretirio
			{								// to diagrafw k apo to euretirio
				if ((amfd2 = AM_OpenIndex (argv[1], attrs[j].indexno)) < 0)
					return (UT_errno = UTE_AMERROR);
				
				if (attrs[j].attrtype == 'i') {
				
					memcpy((void *)&ival2, (void *)(recbuf+attrs[j].offset), attrs[j].attrlength);
					am_value = (char *)(&ival2);
				}
				else if (attrs[j].attrtype == 'f') {
					
					memcpy((void *) &fval2, (void *)(recbuf+attrs[j].offset), attrs[j].attrlength);
					am_value = (char *)(&fval2);
				}
				else if (attrs[j].attrtype == 'c')
					am_value = recbuf+attrs[j].offset;

				if (AM_DeleteEntry(amfd2, attrs[j].attrtype,  attrs[j].attrlength, am_value, recId) != AME_OK)
					return (UT_errno = UTE_AMERROR);

				if(AM_CloseIndex (amfd2) != AME_OK)
					return (UT_errno = UTE_AMERROR);
			}

		}
			// diagrafw tin eggrafi k apo to hf file
		if( HF_DeleteRec(hffd, recId, rel.relwidth) != HFE_OK)
			return (UT_errno = UTE_HFERROR);
	}

	if (indexed == 1) {
		if (AM_CloseIndexScan(amsd) != AME_OK)
			return (UT_errno = UTE_AMERROR);
		if (AM_CloseIndex(amfd) != AME_OK)
			return (UT_errno = UTE_AMERROR);
	}
	else {	
		if(HF_CloseFileScan(hfsd) != HFE_OK)
			return (UT_errno = UTE_HFERROR);
	}

	if(HF_CloseFile(hffd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);


	free(recbuf);

	return UTE_OK;
}





int DM_select(int argc, char* argv[])
{
	char *outFile,**tmpArgv, *outbuf, *relName; /* outFile = onoma arxeiou sto opoio grafontai ta apotelesmata*/
	int numOutAttrs,i_start,i_end, i,wrt, j, tmpArgc,numattrs;
	
	char *condVal, *recbuf,*am_value;
	char attrType;
	int attrLength,attrOffset,curOffset,indexed,indexno,ival,totalength,recId,op;
	int amfd,hffd,outHffd,amsd,hfsd;
	float fval;
	
	relDesc rel;
	attrDesc **outAttrs,*attrs;

	condVal = NULL;
	op =-1;
	attrLength = -1;
	attrOffset = -1;
	attrType = 0;
	indexed = -1;
	numattrs = 1;	// sto numattrs exw to sunolo twn orismatwn pou dothikan kata tin klisi tis DM_select
					// xwris na uparxei sunthiki sto telos (metraw dld ta orismata mexri to 1o orisma tis sunthikis)
					// an uparxei... ksekinaei apo 1 logo tou argv[0] = "select"
	totalength = 0;

	
	if(SCM_select(argc, argv) != UTE_OK)		// elegxw tin orthotita twn orismatwn argc,argv
		return UT_errno;

	if (argc%2 == 0) {		// ean den dothike onoma arxeiou-sxesis sto opoio tha provalw ta apotelesmata
	
		if ( (outFile = malloc(sizeof(char)*(strlen("Onoufrios") +1))) == NULL)
			return (UT_errno = UTE_NOMEM);
		strcpy(outFile,"Onoufrios");	// tou dinw ena monadiko onoma
	
		numOutAttrs = atoi(argv[1]);	// to plithos twn pediwn pou probalontai px  select (ath.name,ath.athid) einai 2
		relName = argv[2];				// to onoma tis sxesis twn pediwn pou tha probalw
		i_start = 3;					// enas arithmos apo ton opoio tha ksekinisei to loop sta probalomena pedia
	}
	else {
							// ean dothike onoma arxeiou sto opoio tha provalw ta apotelesmata
		if ( (outFile = malloc(sizeof(char)*(strlen(argv[1]) +1) )) == NULL)
			return (UT_errno = UTE_NOMEM);
		strcpy(outFile, argv[1]);

		numOutAttrs = atoi(argv[2]);
		relName = argv[3];
		i_start = 4;
		numattrs += 1;		// argv[1] einai to onoma tou arxeiou sto opoio tha provalw ta apotelesmata
	}
	i_end = i_start + numOutAttrs*2 -1;

	numattrs += (numOutAttrs*2+1);	// kathe provalomeno pedio exei 2 orismata ara 2*numOutAttrs orismata
									// +1 logo tou arithmoun ton provalomenwn pediwn

	if((hffd = HF_OpenFile(relName)) < 0)	// anoigw to arxeio sto opoio tha psaksw gia apotelesmata
		return (UT_errno = UTE_HFERROR);

	
	if (CFM_Lookup( relName, &rel, &attrs) != UTE_OK)	// fernw sta rel-attrs tis kataxwriseis tou relName sta attrCat-relCat
		return UT_errno;

	if ( (outAttrs = malloc(numOutAttrs * sizeof(attrDesc *))) == NULL)
		return (UT_errno = UTE_NOMEM);
	
	wrt = 0;
	for (i = i_start; i < i_end; i+=2) {
		// gia kathe orisma pou tha provlithei
		for (j = 0; j < rel.attrcnt; j++) {
			// psaxnw ston pinaka me ta attrs tin diki tou eggrafi
			if(strcmp(attrs[j].attrname, argv[i]) == 0) {
				outAttrs[wrt++] = &(attrs[j]);
				break;
			}
		}
	}

	// ftiaxnw ta arguments gia tin UT_create 
	
	tmpArgc = wrt*2 + 2;
	
	if ( (tmpArgv = malloc (tmpArgc * sizeof(char *))) == NULL)
		return (UT_errno = UTE_NOMEM);

	// to tmpArgv[0] tha einai to onoma tis entolis = create
	if ((tmpArgv[0] = malloc (sizeof(char)*(strlen("create")+1))) == NULL)
		return (UT_errno = UTE_NOMEM);
	strcpy(tmpArgv[0],"create");
	
	// to tmpArgv[1] tha einai to onoma tou arxeiou pou tha dimiourgithei
	if ((tmpArgv[1] = malloc (sizeof(char)*(strlen(outFile)+1))) == NULL)
		return (UT_errno = UTE_NOMEM);
	strcpy(tmpArgv[1], outFile);

	
	for (i = 0; i < wrt; i++) {
		// gia kathe provalomeno pedio dimiourgw 2 orismata ena to onoma tou pediou
		// kai to tupo tou pediou

		if ((tmpArgv[(i+1)*2] = malloc (sizeof(char)*(strlen(outAttrs[i]->attrname)+1))) == NULL)
			return (UT_errno = UTE_NOMEM);
		strcpy(tmpArgv[(i+1)*2], outAttrs[i]->attrname);
		
		// o tupos tou pediou einai tis morfis 'i' 'f' i 'cXXX' opou gia length = 32 to attrLength tha einai
		// to string "'c032'"
		if ((tmpArgv[(i+1)*2 + 1] = malloc (sizeof(char)*8)) == NULL)
			return (UT_errno = UTE_NOMEM);
		tmpArgv[(i+1)*2 + 1][0] = '\'';	// '
		tmpArgv[(i+1)*2 + 1][1] = outAttrs[i]->attrtype; //'c
		if (outAttrs[i]->attrtype == 'c') {
			char cbuf[10];
			sprintf(cbuf,"%d",outAttrs[i]->attrlength);
			if (strlen(cbuf) < 3) {
				tmpArgv[(i+1)*2 + 1][2] = '0';		// 'c0
				tmpArgv[(i+1)*2 + 1][3] = cbuf[0];	// 'c03
				tmpArgv[(i+1)*2 + 1][4] = cbuf[1];	// 'c032
			}
			else {
				tmpArgv[(i+1)*2 + 1][2] = cbuf[0];  // 'c1
				tmpArgv[(i+1)*2 + 1][3] = cbuf[1];  // 'c12
				tmpArgv[(i+1)*2 + 1][4] = cbuf[2];  // 'c120
			}
			tmpArgv[(i+1)*2 + 1][5] = '\'';			// 'c032'
			tmpArgv[(i+1)*2 + 1][6] = '\0';

		}
		else {
			tmpArgv[(i+1)*2 + 1][2] = '\'';			// 'i'
			tmpArgv[(i+1)*2 + 1][3] = '\0';
		}

	}
	tmpArgv[tmpArgc] = NULL;

	if (UT_create(tmpArgc,tmpArgv) != UTE_OK)		// Dimiourgw to arxeio sto opoio tha provalw ta apotelesmata
		return UT_errno;
	
	if ((outHffd = HF_OpenFile(outFile)) < 0)		// anoigw to arxeio eksodou
		return (UT_errno = UTE_HFERROR);

	if(argc > numattrs)
	{												// an exei k sunthiki
		if (strcmp(argv[argc-2], ">") == 0)
			op = GREATER_THAN;
		else if (strcmp(argv[argc-2], ">=") == 0)
			op = GREATER_THAN_OR_EQUAL;
		else if (strcmp(argv[argc-2], "=") == 0)
			op = EQUAL;
		else if (strcmp(argv[argc-2], "<") == 0)
			op = LESS_THAN;
		else if (strcmp(argv[argc-2], "<=") == 0)
			op = LESS_THAN_OR_EQUAL;
		else if (strcmp(argv[argc-2], "!=") == 0)
			op = NOT_EQUAL;

		for(i = 0; i < rel.attrcnt; i++) {
			if(strcmp(attrs[i].attrname, argv[argc-3]) == 0) {
						// briskw tis times twn parakatw stin kataxwrisi tou pediou sunthikis ston attrCat
				attrType = attrs[i].attrtype;
				attrLength = attrs[i].attrlength;
				attrOffset = attrs[i].offset;
				indexed = attrs[i].indexed;
				indexno = attrs[i].indexno;
				break;
			}
		}
	
	
		if (attrType == 'i') {
			ival = atoi(argv[argc-1]);
			condVal = (char *)(&ival);
		}
		else if (attrType == 'f') {
			fval = atof(argv[argc-1]);
			condVal = (char *)(&fval);
		}
		else if (attrType == 'c')
			condVal = argv[argc-1];
	}
	
	if (indexed == 1) {	// ean exei euretirio to pedio ths sunthikis
		if ( (amfd = AM_OpenIndex(argv[argc-4],indexno)) < 0)
			return (UT_errno = UTE_AMERROR);
		
		if ( (amsd = AM_OpenIndexScan(amfd, attrType, attrLength, op, condVal)) < 0)
			return (UT_errno = UTE_AMERROR);
	}
	else {
		if ( (hfsd = HF_OpenFileScan(hffd,rel.relwidth,attrType,attrLength,attrOffset,op,condVal)) < 0 )
			return (UT_errno = UTE_HFERROR);
	}

	
	if( (recbuf = malloc(rel.relwidth*sizeof(char))) == NULL )
		return ( UT_errno = UTE_NOMEM);

	for(i = 0; i < wrt; i++)
	{
		totalength += outAttrs[i]->attrlength;	// briskw to megethos tis eggrafis sto arxei provolis
	}

	if( (outbuf = malloc(totalength*sizeof(char))) == NULL )
		return ( UT_errno = UTE_NOMEM);

	while(1) {
		int recd;
		memset (outbuf, '\0', totalength);
		if (indexed == 1) {
			if ( (recId = AM_FindNextEntry(amsd)) < 0)
				break;
			if (HF_GetThisRec(hffd, recId, recbuf, rel.relwidth) != HFE_OK)
				return (UT_errno = UTE_HFERROR);				
		}
		else {
			if ( (recId = HF_FindNextRec(hfsd,recbuf)) < 0)
				break;
		}

		// recId i eggrafi pou diavastike
		curOffset = 0;	// to current offset tou output buffer
		for(i = 0; i < wrt; i++)
		{
			// antigrafw apo ton recbuf ston outbuf to provalomeno pedio
			memcpy((void *)(outbuf+curOffset), (void *)(recbuf+outAttrs[i]->offset), outAttrs[i]->attrlength);
			curOffset += outAttrs[i]->attrlength;
		}

		if(HF_InsertRec(outHffd, outbuf, totalength) < 0)
			return (UT_errno = UTE_HFERROR);
	}

	if (indexed == 1) {	// ean eixe euretirio
		if (AM_CloseIndexScan(amsd) != AME_OK)
			return (UT_errno = UTE_AMERROR);
		if (AM_CloseIndex(amfd) != AME_OK)
			return (UT_errno = UTE_AMERROR);
	}
	else {	
		if(HF_CloseFileScan(hfsd) != HFE_OK)
			return (UT_errno = UTE_HFERROR);
	}

	if (HF_CloseFile(hffd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	if (UT_print(outFile) != UTE_OK)		// emfanizw ta apotelesmata sto stdout
		return UT_errno;

	if (HF_CloseFile(outHffd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	if (argc % 2 == 0) {
				// ean den eixe dothei onoma arxeiou gia ta apotelesmata
				// diagrafw to temp
		char **tmpArgv2;

		tmpArgc = 2;
		if ( (tmpArgv2 = malloc (tmpArgc * sizeof(char *))) == NULL)
			return (UT_errno = UTE_NOMEM);
		if ((tmpArgv2[0] = malloc (sizeof(char)*(strlen("destroy")+1))) == NULL)
			return (UT_errno = UTE_NOMEM);
		strcpy(tmpArgv2[0],"destroy");
		
		if ((tmpArgv2[1] = malloc (sizeof(char)*(strlen(outFile)+1))) == NULL)
			return (UT_errno = UTE_NOMEM);
		strcpy(tmpArgv2[1], outFile);
		tmpArgv2[2] = NULL;
		
		if (UT_destroy(tmpArgc, tmpArgv2) != UTE_OK)
			return UT_errno;

		for (i = 0; i < tmpArgc; i++)
			free(tmpArgv2[i]);
	}

	for (i = 0; i < tmpArgc; i++)
		free(tmpArgv[i]);
	
	free(outAttrs);
	free(recbuf);
	free(outbuf);


	return UTE_OK;
}




int DM_join(int argc, char* argv[]){
	/* 
		Stin dm_join exoume to polu 2 sxeseis pou tha anoiksoume,provaloume ktlp, i 1i sxesi einai to
		1o melos tis sunthikis kai i 2i an uparxei einai to 2o melos tis sunthikis an einai diaforetiki apo
		tin sxesi tou 1ou melous 
	*/
	   
	char *relname1, *relname2;
	char  *recbuf1,*recbuf2,*outbuf;
	char *outFile,**tmpArgv; /* outFile = onoma arxeiou sto opoio grafontai ta apotelesmata*/
	
	int numOutAttrs,i_start,i_end, i,wrt, j, tmpArgc;
	
	char *condVal,*am_value;
	int curOffset,ival,totalength,recId1,recId2,op;
	float fval;

	int outHffd,amfd1,amfd2,amsd1,amsd2,hffd1,hffd2,hfsd1,hfsd2;
	/* oswn afora ta fd-sd exoume tin eksis katanomi
		hffd1  ->   to descriptor tou arxeiou tou 1ou melous tis sunthikis
		hffd2  ->   to descriptor tou arxeiou tou 2ou melous tis sunthikis
		amfd1  ->	to descriptor tou euretiriou tou 1ou melous tis sunthikis
		amfd2  ->	to descriptor tou euretiriou tou 2ou melous tis sunthikis

		hfsd1  ->	to descriptor tou open scan tou 1ou melous tis sunthikis
		hfsd2  ->	to descriptor tou open scan tou 2ou melous tis sunthikis
		amsd1  ->	to descriptor tou open scan tou 1ou melous tis sunthikis an exei euretirio
		amsd2  ->	to descriptor tou open scan tou 2ou melous tis sunthikis an exei euretirio

	   outHffd ->   to descriptor tou arxeiou sto opoio apothikeuontai ta apotelesmata  
	*/
	
	relDesc rel1,rel2;
	attrDesc **outAttrs,*attrs1,*attrs2,*condAttr1,*condAttr2;

	condVal = NULL;
	op =-1;
	totalength = 0;

	if(SCM_join(argc, argv) != UTE_OK)
		return UT_errno;

	if (argc%2 == 1) {		// ean den dothike onoma arxeiou-sxesis sto opoio tha provalw ta apotelesmata
	
		if ( (outFile = malloc(sizeof(char)*(strlen("Onoufrios") +1))) == NULL)
			return (UT_errno = UTE_NOMEM);
		strcpy(outFile,"Onoufrios");	// tou dinw ena monadiko onoma
	
		numOutAttrs = atoi(argv[1]);	// to plithos twn pediwn pou probalontai px  select (ath.name,ath.athid) einai 2
		i_start = 2;					// enas arithmos apo ton opoio tha ksekinisei to loop sta probalomena pedia
	}
	else {
							// ean dothike onoma arxeiou sto opoio tha provalw ta apotelesmata
		if ( (outFile = malloc(sizeof(char)*(strlen(argv[1]) +1) )) == NULL)
			return (UT_errno = UTE_NOMEM);
		strcpy(outFile, argv[1]);

		numOutAttrs = atoi(argv[2]);
		i_start = 3;
	}
	i_end = i_start + numOutAttrs*2;
 
	relname1 = argv[argc-5];
	
	if((hffd1 = HF_OpenFile(relname1)) < 0)	// anoigw to arxeio tou 1ou melous tis sunthikis
		return (UT_errno = UTE_HFERROR);

	if(strcmp(relname1, argv[argc-2]) != 0) {
		relname2 = argv[argc-2];
		if((hffd2 = HF_OpenFile(relname2)) < 0)	// anoigw to arxeio tou 2ou melous tis sunthikis
			return (UT_errno = UTE_HFERROR);
	}
	else {
		relname2 = relname1;
		hffd2 = hffd1;					// ean aristera k deksia tis sunthikis einai to idio arxeio
	}
		
	// den uparxei problima an einai ta idia relname1 = relname2

	if (CFM_Lookup( relname1, &rel1, &attrs1) != UTE_OK)	// fernw sta rel-attrs tis kataxwriseis tou relname1 sta attrCat-relCat
		return UT_errno;

	if (CFM_Lookup( relname2, &rel2, &attrs2) != UTE_OK)	// fernw sta rel-attrs tis kataxwriseis tou relname2 sta attrCat-relCat
		return UT_errno;

	if ( (outAttrs = malloc(numOutAttrs * sizeof(attrDesc *))) == NULL)
		return (UT_errno = UTE_NOMEM);

	wrt = 0;
	for (i = i_start+1; i < i_end; i+=2) 
	{
		// gia kathe pedio pou tha provlithei
		if (strcmp(argv[i-1] , relname1) == 0) 
		{
			for (j = 0; j < rel1.attrcnt; j++) 
			{
				// psaxnw ston pinaka me ta attrs tin diki tou eggrafi
				if(strcmp(attrs1[j].attrname, argv[i]) == 0) 
				{
					outAttrs[wrt++] = &(attrs1[j]);
					break;
				}
			}
		}
		else if (strcmp(argv[i-1] , relname2) == 0) 
		{
			for (j = 0; j < rel2.attrcnt; j++) 
			{
				// psaxnw ston pinaka me ta attrs tin diki tou eggrafi
				if(strcmp(attrs2[j].attrname, argv[i]) == 0) 
				{
					outAttrs[wrt++] = &(attrs2[j]);
					break;
				}
			}
		}
	}
	
	
	/* briskw tin kataxwrisi tou (onoma_sxesis-pediou) tou 1ou melous tis sunthikis ston attrCat*/
	/* to relname1 diladi to 1o arxeio einai sigoura to 1o melos tis sunthikis*/
	for (j = 0; j < rel1.attrcnt; j++) 
	{
		if(strcmp(attrs1[j].attrname, argv[argc-4]) == 0) 
		{
			condAttr1 = &(attrs1[j]);
			break;
		}
	}

	/* briskw tin kataxwrisi tou (onoma_sxesis-pediou) tou 2ou melous tis sunthikis ston attrCat
		Mporei na einai eite to relaname1 an einai idia me tin sxesi tou 1ou melous i to relname2
		an einai diaforetika px whete athlete.athid = records.athid
	*/

	if (strcmp(argv[argc-2] , relname1) == 0) 
	{
		for (j = 0; j < rel1.attrcnt; j++) 
		{
			if(strcmp(attrs1[j].attrname, argv[argc-1]) == 0) 
			{
				condAttr2 = &(attrs1[j]);
				break;
			}
		}
	}
	else if (strcmp(argv[argc-2] , relname2) == 0) 
	{
		for (j = 0; j < rel2.attrcnt; j++) 
		{
			if(strcmp(attrs2[j].attrname, argv[argc-1]) == 0) 
			{
				condAttr2 = &(attrs2[j]);
				break;
			}
		}
	}



	// ftiaxnw ta arguments gia tin UT_create 
	
	tmpArgc = wrt*2 + 2;
	
	if ( (tmpArgv = malloc (tmpArgc * sizeof(char *))) == NULL)
		return (UT_errno = UTE_NOMEM);

	// to tmpArgv[0] tha einai to onoma tis entolis = create
	if ((tmpArgv[0] = malloc (sizeof(char)*(strlen("create")+1))) == NULL)
		return (UT_errno = UTE_NOMEM);
	strcpy(tmpArgv[0],"create");
	
	// to tmpArgv[1] tha einai to onoma tou arxeiou pou tha dimiourgithei
	if ((tmpArgv[1] = malloc (sizeof(char)*(strlen(outFile)+1))) == NULL)
		return (UT_errno = UTE_NOMEM);
	strcpy(tmpArgv[1], outFile);

	
	for (i = 0; i < wrt; i++) {
		// gia kathe provalomeno pedio dimiourgw 2 orismata ena to onoma tou pediou
		// kai to tupo tou pediou

		if ((tmpArgv[(i+1)*2] = malloc (sizeof(char)*(strlen(outAttrs[i]->attrname)+1))) == NULL)
			return (UT_errno = UTE_NOMEM);
		strcpy(tmpArgv[(i+1)*2], outAttrs[i]->attrname);
		
		// o tupos tou pediou einai tis morfis 'i' 'f' i 'cXXX' opou gia length = 32 to attrLength tha einai
		// to string "'c032'"
		if ((tmpArgv[(i+1)*2 + 1] = malloc (sizeof(char)*8)) == NULL)
			return (UT_errno = UTE_NOMEM);
		tmpArgv[(i+1)*2 + 1][0] = '\'';	// '
		tmpArgv[(i+1)*2 + 1][1] = outAttrs[i]->attrtype; //'c
		if (outAttrs[i]->attrtype == 'c') {
			char cbuf[10];
			sprintf(cbuf,"%d",outAttrs[i]->attrlength);
			if (strlen(cbuf) < 3) {
				tmpArgv[(i+1)*2 + 1][2] = '0';		// 'c0
				tmpArgv[(i+1)*2 + 1][3] = cbuf[0];	// 'c03
				tmpArgv[(i+1)*2 + 1][4] = cbuf[1];	// 'c032
			}
			else {
				tmpArgv[(i+1)*2 + 1][2] = cbuf[0];  // 'c1
				tmpArgv[(i+1)*2 + 1][3] = cbuf[1];  // 'c12
				tmpArgv[(i+1)*2 + 1][4] = cbuf[2];  // 'c120
			}
			tmpArgv[(i+1)*2 + 1][5] = '\'';			// 'c032'
			tmpArgv[(i+1)*2 + 1][6] = '\0';

		}
		else {
			tmpArgv[(i+1)*2 + 1][2] = '\'';			// 'i'
			tmpArgv[(i+1)*2 + 1][3] = '\0';
		}

	}
	tmpArgv[tmpArgc] = NULL;

	if (UT_create(tmpArgc,tmpArgv) != UTE_OK)		// Dimiourgw to arxeio sto opoio tha provalw ta apotelesmata
		return UT_errno;
	
	if ((outHffd = HF_OpenFile(outFile)) < 0)		// anoigw to arxeio eksodou
		return (UT_errno = UTE_HFERROR);

	// tha mpoun anapoda sto eswteriko scan epeidi ean exoume where athlete.record > records.record
	// tote theloume gia to record tou athlete px 1 na gurisoume ta records tou records pou einai mikrotera
	// apo to 1, kati p metafrazetai stin openscan os <

	if (strcmp(argv[argc-3], ">") == 0)	
		op = LESS_THAN;
	else if (strcmp(argv[argc-3], ">=") == 0)
		op = LESS_THAN_OR_EQUAL;
	else if (strcmp(argv[argc-3], "=") == 0)
		op = EQUAL;
	else if (strcmp(argv[argc-3], "<") == 0)
		op = GREATER_THAN;
	else if (strcmp(argv[argc-3], "<=") == 0)
		op = GREATER_THAN_OR_EQUAL;
	else if (strcmp(argv[argc-3], "!=") == 0)
		op = NOT_EQUAL;



	if (condAttr1->indexed == 1) {	// ean exei euretirio to pedio ths sunthikis
		if ( (amfd1 = AM_OpenIndex(condAttr1->relname,condAttr1->indexno)) < 0)
			return (UT_errno = UTE_AMERROR);
		// theloume na mas gurisei oles tis eggrages tou 1ou ( NULL ) operator
		if ( (amsd1 = AM_OpenIndexScan(amfd1,condAttr1->attrtype, condAttr1->attrlength, op, NULL)) < 0)
			return (UT_errno = UTE_AMERROR);
	}
	else {
		if ( (hfsd1 = HF_OpenFileScan(hffd1,rel1.relwidth,' ',-1,-1,-1,NULL)) < 0 )
			return (UT_errno = UTE_HFERROR);
	}

	// to 2o melos tis sutnthikis den prokeite na allaksei opote an exei euretirio to anoigw
	// apo twra

	if (condAttr2->indexed == 1) {
		if (condAttr1->indexed == 1 &&
			condAttr1->indexno == condAttr2->indexno &&
			strcmp(condAttr1->relname,condAttr2->relname) == 0) {
				// an to 2o melos einai to idio akrivws pedio where athlete.athid = athlete.athid
				amfd2 = amfd1;
		}
		else if ((amfd2 = AM_OpenIndex(condAttr2->relname,condAttr2->indexno)) < 0)
				return (UT_errno = UTE_AMERROR);
	}
	
	
	if( (recbuf1 = malloc(rel1.relwidth*sizeof(char))) == NULL )
		return ( UT_errno = UTE_NOMEM);
	if( (recbuf2 = malloc(rel2.relwidth*sizeof(char))) == NULL )
		return ( UT_errno = UTE_NOMEM);

	for(i = 0; i < wrt; i++)
	{
		totalength += outAttrs[i]->attrlength;	// briskw to megethos tis eggrafis sto arxei provolis
	}

	if( (outbuf = malloc(totalength*sizeof(char))) == NULL )
		return ( UT_errno = UTE_NOMEM);

	while(1) {

		if (condAttr1->indexed == 1) {
			if ( (recId1 = AM_FindNextEntry(amsd1)) < 0)
				break;
			if (HF_GetThisRec(hffd1, recId1, recbuf1, rel1.relwidth) != HFE_OK)
				return (UT_errno = UTE_HFERROR);				
		}
		else {
			if ( (recId1 = HF_FindNextRec(hfsd1,recbuf1)) < 0)
				break;
		}

		if (condAttr1->attrtype == 'i') {
			memcpy((void *)&ival,(void *)(recbuf1+condAttr1->offset), condAttr1->attrlength);
			condVal = (char *)(&ival);
		}
		else if (condAttr1->attrtype == 'f') {
			memcpy((void *)&fval, (void *)(recbuf1+condAttr1->offset), condAttr1->attrlength);
			condVal = (char *)(&fval);
		}
		else if (condAttr1->attrtype == 'c')
			condVal = recbuf1+condAttr1->offset;

		
		// anoigw to 2o scan
		if (condAttr2->indexed == 1) {
			if ( (amsd2 = AM_OpenIndexScan(amfd2,condAttr2->attrtype, condAttr2->attrlength, op, condVal)) < 0)
				return (UT_errno = UTE_AMERROR);
		}
		else {
			if ( (hfsd2 = HF_OpenFileScan(hffd2,rel2.relwidth,condAttr2->attrtype,condAttr2->attrlength,condAttr2->offset,op,condVal)) < 0 )
				return (UT_errno = UTE_HFERROR);
		}
		
		while (1) {
			memset(outbuf, '\0', totalength);

			if (condAttr2->indexed == 1) {
				if ( (recId2 = AM_FindNextEntry(amsd2)) < 0)
					break;
				if (HF_GetThisRec(hffd2, recId2, recbuf2, rel2.relwidth) != HFE_OK)
					return (UT_errno = UTE_HFERROR);				
			}
			else {
				if ( (recId2 = HF_FindNextRec(hfsd2,recbuf2)) < 0)
					break;
			}
		
			// exoume to recbuf1 k to recbuf2 pame na baloume sto outbuf
		
			curOffset = 0;	// to current offset tou output buffer
			for(i = 0; i < wrt; i++)
			{
				// antigrafw apo ton recbuf ston outbuf to provalomeno pedio
				// analoga se poia sxesi anikei to kathe pedio tha parw dedomena apo to recbuf1 i to recbuf2
				if (strcmp (outAttrs[i]->relname,relname1) == 0) {
					memcpy((void *)(outbuf+curOffset), (void *)(recbuf1+outAttrs[i]->offset), outAttrs[i]->attrlength);
				}
				else if (strcmp (outAttrs[i]->relname, relname2) == 0) {
					memcpy((void *)(outbuf+curOffset), (void *)(recbuf2+outAttrs[i]->offset), outAttrs[i]->attrlength);
				}
				curOffset += outAttrs[i]->attrlength;
			}
			// sximatistike o outbuf
		
			if(HF_InsertRec(outHffd, outbuf, totalength) < 0)
				return (UT_errno = UTE_HFERROR);
		}
		if (condAttr2->indexed == 1) {	// ean eixe euretirio
			if (AM_CloseIndexScan(amsd2) != AME_OK)
				return (UT_errno = UTE_AMERROR);
		}
		else if(HF_CloseFileScan(hfsd2) != HFE_OK)
				return (UT_errno = UTE_HFERROR);
	}

	if (condAttr1->indexed == 1) {	// ean eixe euretirio
		if (AM_CloseIndexScan(amsd1) != AME_OK)
			return (UT_errno = UTE_AMERROR);
		if (AM_CloseIndex(amfd1) != AME_OK)
			return (UT_errno = UTE_AMERROR);
	}
	else { 
		if(HF_CloseFileScan(hfsd1) != HFE_OK)
			return (UT_errno = UTE_HFERROR);
	}

	if (condAttr2->indexed == 1) {	// ean eixe euretirio kai eixame 2 arxeia diaforetika stin sunthiki
		if (condAttr1->indexed != 1 ||
			condAttr1->indexno != condAttr2->indexno ||
			strcmp(condAttr1->relname,condAttr2->relname) != 0) {
		
			if (AM_CloseIndex(amfd2) != AME_OK)
				return (UT_errno = UTE_AMERROR);
		}
	}
	
	if (HF_CloseFile(hffd1) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
	if (hffd1 != hffd2) {
		if (HF_CloseFile(hffd2) != HFE_OK)
			return (UT_errno = UTE_HFERROR);
	}

	if (UT_print(outFile) != UTE_OK)		// emfanizw ta apotelesmata sto stdout
		return UT_errno;

	if (HF_CloseFile(outHffd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	if (argc % 2 == 1) {
				// ean den eixe dothei onoma arxeiou gia ta apotelesmata
				// diagrafw to temp
		char **tmpArgv2;

		tmpArgc = 2;
		if ( (tmpArgv2 = malloc (tmpArgc * sizeof(char *))) == NULL)
			return (UT_errno = UTE_NOMEM);
		if ((tmpArgv2[0] = malloc (sizeof(char)*(strlen("destroy")+1))) == NULL)
			return (UT_errno = UTE_NOMEM);
		strcpy(tmpArgv2[0],"destroy");
		
		if ((tmpArgv2[1] = malloc (sizeof(char)*(strlen(outFile)+1))) == NULL)
			return (UT_errno = UTE_NOMEM);
		strcpy(tmpArgv2[1], outFile);
		tmpArgv2[2] = NULL;
		
		if (UT_destroy(tmpArgc, tmpArgv2) != UTE_OK)
			return UT_errno;

		for (i = 0; i < tmpArgc; i++)
			free(tmpArgv2[i]);
	}

	for (i = 0; i < tmpArgc; i++)
		free(tmpArgv[i]);
	
	free(outAttrs);
	free(recbuf1);
	free(recbuf2);
	free(outbuf);
	return UTE_OK;
	
}













