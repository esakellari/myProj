#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

#include "CFM_Lib.h"
#include "SCM_Lib.h"
#include "UT_Lib.h"
#include "HF_Lib.h"
#include "AM_Lib.h"
#include "UT_DM_Defines.h"

/************* ut**********/


/* UT_create()
Η συνάρτηση αυτή καλείται όταν ο χρήστης πληκτρολογήσει μια εντολή CREATE. Για τη δημιουργία 
της νέας σχέσης, η υλοποίησή σας θα πρέπει να χρησιμοποιεί την συνάρτηση HF_CreateFile. 
Επιπλέον θα πρέπει να ενημερώνει τους καταλόγους (σχέσεις relCat και attrCat, κλπ) της βάσης.
Δηλαδή, θα πρέπει να εισάγει μία εγγραφή στη σχέση relCat, η οποία να περιγράφει τα χαρακτηριστικά 
της νέας σχέσης. Επιπρόσθετα, θα πρέπει να εισάγει στη σχέση attrCat τις αντίστοιχες εγγραφές που θα 
περιγράφουν ένα-ένα τα γνωρίσματα της νέας σχέσης.
    argv[0]="create"
    argv[1]=όνομα-σχέσης
    argv[2]=όνομα-πεδίου-1
    argv[3]=φορμάτ-πεδίου-1
        .
        .
        .
    argv[argc-2]=όνομα-πεδίου-Ν
    argv[argc-1]=φορμάτ-πεδίου-Ν
    argv[argc]=NULL

*/


int UT_create(int argc, char* argv[])
{
	int i,j,mult,sumOffset, attrCnt, attrWrt;
	int cLength;

	relDesc *rel;
	attrDesc *attrTable;

	if (SCM_create(argc,argv) != UTE_OK)
		return UT_errno;

	attrCnt = (argc	- 2)/2;

	if ((attrTable = malloc(sizeof(attrDesc) * attrCnt)) == NULL)
		return (UT_errno = UTE_NOMEM);

	
	sumOffset = 0;	/* mesa stin parakatw epanalipsi dinei to trexwn offset gia kathe orisma */
	attrWrt = 0;	/* se poio orisma anaferomaste */

	for (i = 2; i < argc; i += 2) {
		/* Epomeno orisma */

		strcpy(attrTable[attrWrt].relname, argv[1]);
		strcpy(attrTable[attrWrt].attrname, argv[i]);
		attrTable[attrWrt].offset = sumOffset;
		// -> to attrtype einai tis morfis "'i'" ara theloume to 2obyte
		
		attrTable[attrWrt].attrtype = argv[i+1][1];
		if (attrTable[attrWrt].attrtype == 'c') {
			cLength = 0;
			for(j=strlen(argv[i+1])-2, mult=1 ; j >= 2 ; j--, mult*=10)
                if(argv[i+1][j] >= '0'  &&  argv[i+1][j] <= '9')
					cLength += (argv[i+1][j]-'0')*mult;                            
     
			attrTable[attrWrt].attrlength = cLength;
			sumOffset += attrTable[attrWrt].attrlength;
		}
		else if(attrTable[attrWrt].attrtype == 'i') {
			attrTable[attrWrt].attrlength = sizeof(int);
			sumOffset += sizeof(int);
		}
		else {	// if float
			attrTable[attrWrt].attrlength = sizeof(float);
			sumOffset += sizeof(float);
		}
		attrTable[attrWrt].indexed = 0;
		attrTable[attrWrt].indexno = -1;
		attrWrt++;			
	}
	
	// gemizw kai tin eggrafi gia to relCat
	if ((rel = malloc(sizeof(relDesc))) == NULL)
		return (UT_errno = UTE_NOMEM);
		
	strcpy(rel->relname, argv[1]);
	rel->relwidth = sumOffset;
	rel->attrcnt = attrCnt;
	rel->indexcnt = 0;
	
	if (CFM_AddRel(*rel, attrTable) != UTE_OK)
		return UT_errno;	

	
	if (HF_CreateFile(rel->relname) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	free(rel);
	free(attrTable);
	return UTE_OK;
}


/* UT_buildIndex() 
Η συνάρτηση αυτή καλείται όταν ο χρήστης πληκτρολογήσει μια εντολή BUILDINDEX. Η υλοποίηση 
της εντολής αυτής θα πρέπει να δημιουργεί το ευρετήριο χρησιμοποιώντας την συνάρτηση AM_CreateIndex 
και να ενημερώνει κατάλληλα τους καταλόγους της βάσης.
    argv[0]="buildindex"
    argv[1]=όνομα-σχέσης
    argv[2]=όνομα-πεδίου
    argv[3]=NULL
*/

int UT_buildindex(int argc, char* argv[])
{
	int filefd,indexfd,sd, recId, indexNo, relWidth, attrOffset, attrLength;
	int max = -1, curIndexNo,previndexNo;
	int found = 0;
	char attrType;
	
	relDesc *relBuf;
	attrDesc *attrBuf;
	char *recordBuf;

	if (SCM_buildIndex(argc, argv) != UTE_OK)
		return UT_errno;

	/* psaxnw ti grammi tou relCat me relname - > argv[1] */
	if ((sd = HF_OpenFileScan(relCatFd, sizeof(relDesc), 'c', MAXNAME, 0, EQUAL, argv[1])) < 0)
		return (UT_errno = UTE_HFERROR);

	relBuf = malloc(sizeof(relDesc));
	if( relBuf == NULL )
		return (UT_errno = UTE_NOMEM);

	if((recId = HF_FindNextRec(sd, (char *)relBuf)) < 0 )
		return (UT_errno = UTE_HFERROR);
		
	relWidth = relBuf->relwidth;
	
	if (HF_CloseFileScan(sd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
	
	free(relBuf);

	/* psaxnw tis grammi tou attrCat me relname - > argv[1] */
	if ((sd = HF_OpenFileScan(attrCatFd, sizeof(attrDesc), 'c', MAXNAME, 0, EQUAL, argv[1])) < 0)
		return (UT_errno = UTE_HFERROR);
	
	attrBuf = malloc(sizeof(attrDesc));
	if (attrBuf == NULL)
		return (UT_errno = UTE_NOMEM);

	while((recId = HF_FindNextRec(sd,(char *)attrBuf)) >= 0) /* oso briskei pedia */
	{
		curIndexNo = attrBuf->indexno;
		// tha brw to maximum index id kai to indexNo tou new index tha ginei max+ 1
		if (curIndexNo > max)
			max = curIndexNo;

		if (strncmp (attrBuf->attrname, argv[2], MAXNAME) == 0) {  /* an brike to pedio sto opoio tha ftiaksoume euretirio*/
			found = 1;
			/* antigrafw kapoia values pou tha mou xreiastoun parakatw*/
			attrOffset = attrBuf->offset;
			attrLength = attrBuf->attrlength;
			attrType = attrBuf->attrtype;
			previndexNo = attrBuf->indexno;
		}	
	}
	
	if (!found)
		return (UT_errno = UTE_REL_ATTRNOTFOUND);
	if (HF_CloseFileScan(sd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	//-> Diagrafw to palio index tou pediou an eixe
	//if (previndexNo != -1)
	//	if(AM_DestroyIndex(argv[1], previndexNo) != AME_OK)
	//		return (UT_errno = UTE_AMERROR);	
	
	indexNo = max + 1;	// o arithmos tou euretiriou pou tha dimiourgithei

	if (AM_CreateIndex(argv[1], indexNo, attrType, attrLength) != AME_OK)
		return (UT_errno = UTE_AMERROR);

		/* gia na enimerwsei tis sxeseis relCat - attrCat */
	if (CFM_IndexAttribute(argv[1], argv[2], indexNo) != UTE_OK)
		return UT_errno;
	
	free(attrBuf);

	/* GemizW TO INDEX*/
	
	/* Anoigw to arxeio panw sto opoio kanw euretirio */
	if ((filefd = HF_OpenFile(argv[1])) < 0)
		return (UT_errno = UTE_HFERROR);
	
	/* Anoigw to arxeio tou euretiriou*/
	if ((indexfd = AM_OpenIndex (argv[1], indexNo)) < 0)
		return (UT_errno = UTE_AMERROR);
	
	/* fernw mia mia kathe eggrafi tou arxeiou argv[1] */
	if ((sd = HF_OpenFileScan(filefd, relWidth, attrType, -1, -1, -1, NULL)) < 0)
		return (UT_errno = UTE_HFERROR);

	recordBuf = malloc(relWidth*sizeof(char));
	if (recordBuf == NULL)
		return (UT_errno = UTE_NOMEM);

	while ((recId = HF_FindNextRec(sd, recordBuf)) >= 0)
	{	
		if(AM_InsertEntry(indexfd, attrType, attrLength, recordBuf+attrOffset, recId) != AME_OK)
			return (UT_errno = UTE_AMERROR);
	}
	
	
	if (HF_CloseFileScan(sd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
	if (HF_CloseFile(filefd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
	if (AM_CloseIndex (indexfd)!= AME_OK)
		return (UT_errno = UTE_AMERROR);
	
		
	free(recordBuf);
	return UTE_OK;

}
    




/* UT_destroy()
 
Η συνάρτηση αυτή καλείται όταν ο χρήστης πληκτρολογήσει μια εντολή DESTROY. Για την υλοποίηση 
αυτής της εντολής θα πρέπει να χρησιμοποιήσετε τις συναρτήσεις AM_DestroyIndex και HF_DestroyFile 
και να ενημερώσετε κατάλληλα τους καταλόγους της βάσης.
    argv[0]="destroy"
    argv[1]=όνομα-σχέσης
    argv[2]=NULL
*/

int UT_destroy(int argc, char* argv[])
{
	int fd,sd,recId,indexNo;
	attrDesc *attrBuf;

	if (SCM_destroy(argc, argv) != UTE_OK)
		return UT_errno;
	
	if(HF_DestroyFile(argv[1]) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	/* psaxnoume eggrafes tou pinaka attrCat me prwto pedio idio me to argv[1]*/
	if ((sd = HF_OpenFileScan(attrCatFd, sizeof(attrDesc), 'c', MAXNAME, 0, EQUAL, argv[1])) < 0)
		return (UT_errno = UTE_HFERROR);
	
	attrBuf = malloc(sizeof(attrDesc));
	if (attrBuf == NULL)
		return (UT_errno = UTE_NOMEM);

	while((recId = HF_FindNextRec(sd,(char *)attrBuf)) >= 0)
	{
		if (attrBuf->indexno != -1)
			if(AM_DestroyIndex(argv[1] ,attrBuf->indexno) != AME_OK)
				return (UT_errno = UTE_AMERROR);	
	}

	free(attrBuf);

	if(CFM_DeleteRel(argv[1]) != UTE_OK)
		return UT_errno;

	if (HF_CloseFileScan(sd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
	
	return UTE_OK;
}
 



/* UT_quit() 
Η συνάρτηση αυτή καλείται όταν ο χρήστης πληκτρολογήσει την εντολή QUIT. Η συνάρτηση αυτή θα πρέπει
 να κλείνει όλες τις τυχόν ανοικτές σχέσεις και ευρετήρια (συμπεριλαμβανομένων και αυτών που αποτελούν
 τους καταλόγους της βάσης) και στην συνέχεια να καλεί την exit(1).
*/

int UT_quit(void)
{
	/* kleinw ta 2 anoixta arxeia to relCat kai to attrCat */
	if(HF_CloseFile(relCatFd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	if(HF_CloseFile(attrCatFd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
	
	exit(1);
}


/* UT_print()
 Η συνάρτηση αυτή εκτυπώνει στην οθόνη τις εγγραφές της σχέσης relName. Η συνάρτηση αυτή χρησιμοποιείται 
 από τις DM_select και DM_join σε περίπτωση που το αποτέλεσμα πρέπει να εκτυπωθεί στην οθόνη.
*/

int UT_print(char* relName)
{
	int fd, sd, i, j, counter, recId;
	char *recBuf;	
	relDesc *rel;
	attrDesc *attrs;
	
	if ( (rel = malloc(sizeof(relDesc))) == NULL)
		return (UT_errno = UTE_NOMEM);
	
	//------- Pairnei plirofories ap ta arxeia "relCat" - "attrCat"

	if (CFM_Lookup(relName, rel, &attrs) != UTE_OK)
		return UT_errno;
	
	if((recBuf = malloc(rel->relwidth * sizeof(char))) == NULL)
		return (UT_errno = UTE_NOMEM);

	if((fd = HF_OpenFile(relName)) < 0)				//Anoigei to arxeio me onoma tou relName.
		return (UT_errno = UTE_HFERROR);

	counter = 0;
	
	if((recId = HF_GetFirstRec(fd, recBuf, rel->relwidth)) >= 0) {
		do {
			counter++; // ean efere eggrafi 
			for(i=0 ; i < rel->attrcnt ; i++) {
				if(attrs[i].attrtype == 'i')
					printf("%*d\t",5, *(int *)(recBuf + attrs[i].offset));
				else if(attrs[i].attrtype == 'f')
					printf("%*f\t",5, *(float *)(recBuf + attrs[i].offset));
				else {	//if 'c'
					for (j = 0; j < attrs[i].attrlength; j++) {
						if ((recBuf + attrs[i].offset)[j] == '\0')
							break;
						printf("%c",(recBuf + attrs[i].offset)[j]);
					}
					printf("\t");

				}
			}
			putchar('\n');
		} while((recId = HF_GetNextRec(fd, recId, recBuf, rel->relwidth)) >= 0);
	}
	printf("Found %d records\n",counter);
	free(rel);
	free(recBuf);
	free(attrs);

	if(HF_CloseFile(fd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
	return UTE_OK;
}

