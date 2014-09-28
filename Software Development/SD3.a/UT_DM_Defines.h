/*
 * File:   UT_DM_Defines.h
 */

#ifndef _UT_DM_DEFINES_H
#define _UT_DM_DEFINES_H

#define MAXNAME 36

#define UTE_OK 0
#define UTE_NOMEM 		-1
#define UTE_HFERROR		-2
#define UTE_AMERROR		-3
/*****************************************SCM ERRORS****************************/
#define UTE_SCM_INSUFFICIENTARGS   		-4
#define UTE_SCM_WRONGNUMOFARGS			-5
#define UTE_SCM_WRONGFIRSTARG			-6	
#define UTE_SCM_WRONGRELNAME			-7
#define UTE_SCM_NULLNAME				-8
#define UTE_SCM_NULLFORMAT				-9
#define UTE_SCM_LONGNAME				-10
#define UTE_SCM_SAMENAME				-11
#define UTE_SCM_INVALIDNAME				-12
#define UTE_SCM_WRONGARGSBUILD			-13
#define UTE_SCM_WRONGARGSDESTROY		-14
#define UTE_SCM_WRONGARGS				-15
#define UTE_SCM_RELNOTEXISTS			-16
#define UTE_SCM_RELEXISTS			-17
#define UTE_REL_ATTRNOTFOUND			-18
#define UTE_OUTOFBOUNDS				-19
#define UTE_ATTRDATAMISSING			-20

int UT_errno;

typedef struct {
    char relname[MAXNAME]; /* ����� ������ */
    int relwidth; /* ����� �������� ������ �� bytes */
     int attrcnt; /* ������� ������ �������� */
    int indexcnt; /* ������� ���������� ������ */
} relDesc;

typedef struct {
    char relname[MAXNAME]; /* ����� ������ */
    char attrname[MAXNAME]; /* ����� ������ ��� ������ */
    int offset; /* �������� ����� ������ ��� ��� ���� ��� �������� �� bytes */
    int attrlength; /* ����� ������ �� bytes */
    char attrtype; /* ����� ������ ('i', 'f', � 'c' */
    int indexed; /* TRUE �� �� ����� ���� ��������� */
    int indexno; /* ����� ������� ��� ���������� �� indexed=TRUE */
} attrDesc;

/* �� ������������� ��� �������� ������� relCat */
int relCatFd;

/* �� ������������� ��� �������� ������� attrCat */
int attrCatFd;

#endif
