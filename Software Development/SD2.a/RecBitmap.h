#ifndef _BIT_
#define _BIT_


int FullOrEmpty(char *buf, int maxrecords, int blocksize, int val);
int getFirstZeroBitPos(char *buf, int blocksize);

void setBit(char *buf, int bitId);
void resetBit(char *buf, int bitId);
int isBitActive(char *buf,int bitId);


#endif
