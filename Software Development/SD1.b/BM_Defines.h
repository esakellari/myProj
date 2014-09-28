#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "BF_Defines.h"

#ifndef _BM_
#define _BM_

#define BFE_OK                        0        /* OK */
#define BFE_NOMEM                    -1        /* Δεν υπάρχει διαθέσιμη μνήμη */
#define BFE_CANNOTOPENFILE           -2       /* Αδύνατο το άνοιγμα του αρχείου */
#define BFE_CANNOTCLOSEFILE          -3      /* Αδύνατο το κλείσιμο του αρχείου */
#define BFE_CANNOTCREATEFILE         -4      /* Αδύνατη η δημιουργία του αρχείου */
#define BFE_INCOMPLETEREAD           -5      /* Ημιτελές διάβασμα */
#define BFE_INCOMPLETEWRITE          -6      /* Ημιτελές γράψιμο */
#define BFE_FILEEXISTS               -7      /* Το αρχείο υπάρχει ήδη */
#define BFE_NOBUF                    -8       /* Έλλειψη χώρου στην ενδιάμεση μνήμη */
#define BFE_FILEOPEN                 -10     /* Το αρχείο είναι ήδη ανοιχτό */
#define BFE_FD                       -11     /* Λανθασμένο αναγνωριστικό αρχείου */
#define BFE_FILENOTEXISTS            -12    /* Το αρχείο δεν υπάρχει */
#define BFE_FTABFULL                 -13    /* Λίστα ανοιχτών αρχείων πλήρης */
#define BFE_HEADOVERFLOW             -14    /* Λανθασμένος αριθμός  */
#define BFE_BLOCKFIXED               -15    /* Μπλοκ ήδη "καρφωμένο" στη μνήμη */
#define BFE_BLOCKUNFIXED             -16    /* Μπλοκ ήδη ξεκαρφωμένο */
#define BFE_EOF                      -17    /* Τέλος αρχείου */
#define BFE_FILEHASFIXEDBLOCKS       -18    /* Το αρχείο έχει καρφωμένα block */
#define BFE_BLOCKFREE                -19   /* Μπλοκ ήδη διαθέσιμο προς ανακύκλωση */
#define BFE_BLOCKINBUF               -20   /* Μπλοκ ήδη στη μνήμη */
#define BFE_BLOCKNOTINBUF            -21   /* Μπλοκ όχι στη μνήμη */
#define BFE_INVALIDBLOCK             -22   /* Λανθασμένος κωδικός block */
#define BFE_CANNOTDESTROYFILE        -23   /* Αδύνατη η καταστροφή του αρχείου */
#define BFE_BLOCKSTILLPINNED	  -24

#define BF_BUFFER_SIZE          20       /* Μέγεθος της ενδιάμεσης μνήμης σε block */
#define MAXOPENFILES            25        /* Μέγιστο πλήθος ανοιχτών αρχείων */
#define BF_BLOCK_SIZE           1024    /* Μέγεθος ενός block σε bytes */
#define MAX_BLOCKS              8192     /* Μέγιστο πλήθος από διαθέσιμα block ανά αρχείο */
#define MAX_FILE_NAME           256     /* Μέγιστο μέγεθος για όνομα ενός αρχείου */


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
