
/* 
 * File:   HF_Defines.h
 * Author: marialena
 *
 * Created on November 25, 2009, 7:07 PM
 */

#ifndef _HF_DEFINES_H
#define	_HF_DEFINES_H

#include "BF_Defines.h"


#define HFE_OK           0
#define HFE_BFERROR     -1   /* Error of BF level */
#define HFE_EOF         -2   /* End of file */
#define HFE_INVALIDREC  -3   /* Invalid record */
#define HFE_STFULL      -4   /* Scan table full */
#define HFE_INVALIDOP   -5   /* Invalid scan operator */
#define HFE_ATTRTYPE    -6	 /* Invalid attrType */
#define HFE_RECSIZE	   -7	 /* Negative RecordSize */
#define HFE_NOMEM 	   -8
#define HFE_HASOPENSCANS -9

#define HF_MAXBLOCKS    8*BF_BLOCK_SIZE-1
#define HF_HEADPOSITIONS    8*BF_BLOCK_SIZE

int HF_errno;

#endif	/* _HF_DEFINES_H */

