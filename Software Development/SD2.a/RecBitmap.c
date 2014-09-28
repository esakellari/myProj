#include "RecBitmap.h"


// I Sunartisi upologizei ean se ola ta 'blocksize' bytes tou pinaka 'buf', ta bits tous
// exoun tin timi tis 'val' (0 or 1).
//
int FullOrEmpty(char *buf, int maxrecords, int blocksize, int val)
{
	int i;
	int limit = blocksize*8 - maxrecords;

	for (i=blocksize*8-1 ; i >= limit ; i--)
		if (isBitActive(buf, i) != val)
			return 0;
	return 1;
}


// I sunartisi epistrefei ti thesi tou prwtou midenikou bit twn blocksize bytes tou pinaka buf.
// Ean den vrei mideniko bit, epistrefei -1.
//
int getFirstZeroBitPos(char *buf, int blocksize)
{
	int i;
	int limit = blocksize*8 - 1;

	for (i=0 ; i<limit ; i++)			//-> Elegxei mexri 'MAX_BLOCKS-1' opws anaferetai parapanw.
		if (!isBitActive(buf, i))
			return i;
	return -1;
}


// I Sunartisi kanei set to 'bitId' bit tou pinaka buf se 1.
//
void setBit(char *buf, int bitId)
{
	*(buf+ bitId/8) |= (1 << (7 - bitId%8));	//set 2o *buf = 0010_1000 OR 0100_0000=0110_1000
}

// I Sunartisi kanei set to 'bitId' bit tou pinaka buf se 0.
//
void resetBit(char *buf, int bitId)
{	
	*(buf+ bitId/8) &= ( ~(1 << ( 7 - bitId%8)));	//unset 3o *buf = 0010_1000 AND 1101_1111=0000_1000
}

// I Sunartisi elegxei ean to 'bitId' bit tou pinaka buf einai active dld iso me 1.
//
int isBitActive(char *buf,int bitId)
{
	if ( (*(buf + bitId/8) >> (7 - bitId%8)) % 2 == 0)  
		return 0;
	return 1;

}

