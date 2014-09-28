#include "bitmap.h"



/*******************************************************bitmap handling***************************************/

//epistrefei 1 an to bit pou elegxoume einai 1 , alliws 0

int  checkbit(char* block, int offset)
{

	char test = 1;
	char result;
	int byte;
	int bit;

	//vriskw to byte sto opoio vrisketai to bit  p thelw 
	byte = (offset/8);

	//vriskw to bit pou vrisketai to bit p thelw mesa sto byte
	bit = offset%8+1;

	//olisthisi tis metavlitis test kata oses theseis xreiazetai gia na ginei iipraksi
	test = test << (8 - bit);

	result = test & block[byte];

	if(result == 0)
	return 0;

	else return 1;
	
}

//synartisi pou thetei ena sygekrimeno bit iso me 1
void setbit(char* block, int offset)
{
	char test=1;
	int byte;
	int bit;

	//vriskw to byte sto opoio vrisketai to bit  p thelw 
	byte = (offset/8);

	//vriskw to bit pou vrisketai to bit p thelw mesa sto byte
	bit = offset%8+1;

	//olisthisi tis metavlitis test kata oses theseis xreiazetai gia na ginei iipraksi
	test = test << (8 -bit);

	block[byte] = block[byte] | test;
}


//thetei ena sygekrimeno bit iso me 0
void unsetbit(char* block, int offset)
{

	char test = 1;
	int byte;
	int bit;

	//vriskw to byte sto opoio vrisketai to bit  p thelw 
	byte = (offset/8);

	//vriskw to bit pou vrisketai to bit p thelw mesa sto byte
	bit = offset%8+1;

	//olisthisi tis metavlitis test kata oses theseis xreiazetai gia na ginei iipraksi
	test = test << (8 - bit);

	block[byte] = block[byte] & (~test);

}

