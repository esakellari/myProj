#ifndef UT_DM_DEFINES_H
#define UT_DM_DEFINES_H

#define MAXNAME 32

#define UTE_OK		  0
#define UTE_NOMEM	 -1
#define UTE_HFERROR	 -2
#define UTE_AMERROR	 -3
#define UTE_MALLOC	 -4
#define UTE_ATTRNOTFOUND -5
#define UTE_RELNOTFOUND  -6

/*****************************************SCM ERRORS****************************/
#define UTE_SCM_INSUFFICIENTARGS   		-7
#define UTE_SCM_WRONGNUMOFARGS			-8	/* otan to argc exei diaforetiki timi apo to plithos twn orismatwn*/
#define UTE_SCM_WRONGFIRSTARG			-9	/* otan to prwto orisma "insert" ktlp exei lathos timi */
#define UTE_SCM_WRONGNAMELEN			-10 /* otan to mikos tou relname-attrname einai megalutero apo MAXNAME */
#define UTE_SCM_NULLNAME				-11 /* otan to orisma name tou pediou einai NULL*/
#define UTE_SCM_NULLFORMAT				-12
#define UTE_SCM_LONGNAME				-13
#define UTE_SCM_SAMENAME				-14
#define UTE_SCM_INVALIDNAME				-15
#define UTE_SCM_WRONGARGSBUILD			-16
#define UTE_SCM_WRONGARGSDESTROY		-17
#define UTE_SCM_WRONGARGS				-18
#define UTE_SCM_RELNOTEXISTS			-19	/* otan to arxeio-sxesi den uparxei*/
#define UTE_SCM_RELEXISTS				-20 /* otan to arxeio-sxesi uparxei*/
#define UTE_REL_ATTRNOTFOUND			-21
#define UTE_OUTOFBOUNDS					-22
#define UTE_ATTRDATAMISSING				-23
#define UTE_WRONGATTRNUMS				-24 /* otan o arithmos ton pediwn pou dothike px (insert) einai diaforetikos apo to attrcnt*/
#define UTE_ATTRNOTVALID				-25 /* otan kapoio attr den brethike mesa sto attrCat*/
#define UTE_SCM_NULLVALUE				-26 /* otan to orisma value tou pediou einai NULL*/
#define UTE_WRONGCVALLEN				-27 /* otan to mikos tou c val einai megalutero apo to mikos p orizetai sto attrlength*/
#define UTE_WRONGINTVAL					-28 /* otan to pedio einai int k periexei xaraktira p den einai psifio*/
#define UTE_WRONGFLOATVAL				-29 /* otan to pedio einai float k den exei swsti morfi*/

#define UTE_SCM_ZEROARGS	-100
#define UTE_SCM_DUPLICATEARGS	-101
#define UTE_SCM_ARGNUM		-102
#define UTE_SCM_RELEXISTS	-103
#define UTE_SCM_RELNOTEXIST	-104
#define UTE_SCM_WRONGOP		-105
#define UTE_SCM_INDEXEXISTS	-106

#pragma pack(1)

typedef struct {
    char relname[MAXNAME];
    int relwidth;
    int attrcnt;
    int indexcnt;
} relDesc;

typedef struct {
    char relname[MAXNAME];
    char attrname[MAXNAME];
    int offset;
    int attrlength;
    char attrtype;
    int indexed;
    int indexno;
} attrDesc;


int relCatFd;
int attrCatFd;
int UT_errno;

#endif /* UT_DM_DEFINES_H */
