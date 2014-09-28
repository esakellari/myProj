#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "BF_Defines.h"

#ifndef _BM_
#define _BM_

#define BFE_OK                        0        /* OK */
#define BFE_NOMEM                    -1        /* ��� ������� ��������� ����� */
#define BFE_CANNOTOPENFILE           -2       /* ������� �� ������� ��� ������� */
#define BFE_CANNOTCLOSEFILE          -3      /* ������� �� �������� ��� ������� */
#define BFE_CANNOTCREATEFILE         -4      /* ������� � ���������� ��� ������� */
#define BFE_INCOMPLETEREAD           -5      /* �������� �������� */
#define BFE_INCOMPLETEWRITE          -6      /* �������� ������� */
#define BFE_FILEEXISTS               -7      /* �� ������ ������� ��� */
#define BFE_NOBUF                    -8       /* ������� ����� ���� ��������� ����� */
#define BFE_FILEOPEN                 -10     /* �� ������ ����� ��� ������� */
#define BFE_FD                       -11     /* ���������� ������������� ������� */
#define BFE_FILENOTEXISTS            -12    /* �� ������ ��� ������� */
#define BFE_FTABFULL                 -13    /* ����� �������� ������� ������ */
#define BFE_HEADOVERFLOW             -14    /* ����������� �������  */
#define BFE_BLOCKFIXED               -15    /* ����� ��� "���������" ��� ����� */
#define BFE_BLOCKUNFIXED             -16    /* ����� ��� ����������� */
#define BFE_EOF                      -17    /* ����� ������� */
#define BFE_FILEHASFIXEDBLOCKS       -18    /* �� ������ ���� ��������� block */
#define BFE_BLOCKFREE                -19   /* ����� ��� ��������� ���� ���������� */
#define BFE_BLOCKINBUF               -20   /* ����� ��� ��� ����� */
#define BFE_BLOCKNOTINBUF            -21   /* ����� ��� ��� ����� */
#define BFE_INVALIDBLOCK             -22   /* ����������� ������� block */
#define BFE_CANNOTDESTROYFILE        -23   /* ������� � ���������� ��� ������� */
#define BFE_BLOCKSTILLPINNED	  -24

#define BF_BUFFER_SIZE          20       /* ������� ��� ���������� ������ �� block */
#define MAXOPENFILES            25        /* ������� ������ �������� ������� */
#define BF_BLOCK_SIZE           1024    /* ������� ���� block �� bytes */
#define MAX_BLOCKS              8192     /* ������� ������ ��� ��������� block ��� ������ */
#define MAX_FILE_NAME           256     /* ������� ������� ��� ����� ���� ������� */


int glob_clock;

//domi gia ta block
typedef struct {

	char filename[MAX_FILE_NAME];
	char data[BF_BLOCK_SIZE];
	BOOLEAN free;
	BOOLEAN pinned[MAXOPENFILES];
	int blocknum;
	BOOLEAN dirty;
	int pincount;
	int lastUsed;

	}block;


block Blocks[BF_BUFFER_SIZE];


#endif
