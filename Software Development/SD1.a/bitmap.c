#include "bitmap.h"



/*******************************************************bitmap handling***************************************/

//epistrefei 1 an to bit pou elegxoume einai 1 , alliws 0

int  checkbit(char* block, int offset)
{

	char test = 1;
	char result;
	int byte;
	int bit;

	byte = (offset/8);

	bit = offset%8+1;

	
//	printf("BIT TO BE CHECKED:%d OF BLOCK NO %d\n", bit,byte);


	test = test << (8 - bit);

//	printf("TEST = %d\n", test);

	result = test & block[byte];

	if(result == 0)
	return 0;

	else return 1;
	
}

void setbit(char* block, int offset)
{
	char test=1;
	int byte;
	int bit;

	byte = (offset/8);

	bit = offset%8+1;

	

//	printf("BIT TO BE SET:%d OF BLOCK NO %d\n", bit,byte);

	test = test << (8 -bit);

//	 printf("TEST = %d\n", test);


	block[byte] = block[byte] | test;

	
}

void unsetbit(char* block, int offset)
{

	char test = 1;
	int byte;
	int bit;

	byte = (offset/8);

	bit = offset%8+1;

	test = test << (8 - bit);

	block[byte] = block[byte] & (~test);


}

