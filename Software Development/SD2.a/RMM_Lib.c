#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RMM_Lib.h"
#include "RecBitmap.h"



// ------------------------------------------------------------------------------------------------------------------------
//
// >>> RMM_InsertRec ()
// 	I routina eisagei tin eggrafi pou deixnei o deiktis 'record' sto anoixto arxeio me anagnwristiko arithmo
// 	'fileDesc'. To megethos tis eggrafis einai 'recordSize'.
// 	I routina epistrefei enan mi arnitiko akeraio ws to anagnwristiko eggrafis 'recordNum' to opoio exei anatethei sti
// 	neo-eiselthousa eggrafi, an i eisagwgi einai epituxis, eid'alliws enan kwdiko sfalmatos.
//

int RMM_InsertRec(int fileDesc, char *record, int recordSize)
{
	int blockNum, ret, max_records, recordNum;
	char *blockBuf, *HF_HeadBuf;


	if ((HF_HeadBuf = malloc(BF_BLOCK_SIZE)) == NULL ) {
		BF_Errno = BFE_NOMEM;
		return (HF_errno = HFE_BFERROR);
	}
	if ((blockBuf = malloc(BF_BLOCK_SIZE)) == NULL ) {
		BF_Errno = BFE_NOMEM;
		return (HF_errno = HFE_BFERROR);
	}

	if(BF_GetThisBlock(fileDesc, 0, &HF_HeadBuf) != BFE_OK)			//-> Fernei to HF_HEADER stin mnimi.
		return (HF_errno = HFE_BFERROR);				//

	if ((blockNum = getFirstZeroBitPos(HF_HeadBuf,BF_BLOCK_SIZE)) == -1)	//-> Psaxnei to HF_HEADER gia to 1o mi mideniko. Ean ola
		return (HF_errno = HFE_EOF);					//   einai 1 epistrefei 'EOF'. I epistrefomeni timi '0'
	blockNum ++;								//   anaferetai sto 1o block.
	
	if ((ret = BF_GetThisBlock(fileDesc, blockNum, &blockBuf)) == BFE_INVALIDBLOCK) {	//-> Epixeirei na ferei sti mnimi to block sto
		if (BF_AllocBlock(fileDesc, &blockNum, &blockBuf) != BFE_OK)			//   opoio tha eisagagei tin eggrafi. Ean itan
			return (HF_errno = HFE_BFERROR);					//   mi eguro block, to dimiourgei kanontas alloc.
	}											//
	else if (ret != BFE_OK )
		return (HF_errno = HFE_BFERROR);

	max_records = BF_BLOCK_SIZE*8 / (recordSize*8 + 1);			//-> Ypologizei to megisto plithos eggrafwn pou xwraei
										//   to kathe block.
	for(recordNum=0 ; recordNum < max_records ; recordNum++)		//-> Psaxnei sto anestrammeno bitmap tou block gia na
		if (!isBitActive(blockBuf, BF_BLOCK_SIZE*8-1 -recordNum))	//   vrei tin prwti adeia thesi eggrafis sto block,
			break;							//   i opoia kai einai i 'recordNum'. Sigoura tha vrei
										//   mia adeia thesi eggrafis, mias kai to block epilexthike
										//   ef'oson den itan plirws gemato apo eggrafes.
										//
	memcpy(blockBuf + recordSize*recordNum, record, recordSize);		//-> Adigrafei tin eggrafi sto block, stin thesi
										//   'recordNum'.
	setBit(blockBuf, BF_BLOCK_SIZE*8-1 -recordNum);				//-> Enimerwnei to bitmap tou block gia to gemisma tis
										//   eggrafis.
	if ((ret = FullOrEmpty(blockBuf, max_records, BF_BLOCK_SIZE, 1)))	//-> Ean to block gemise me eggrafes, tote enimerwnei
		setBit(HF_HeadBuf, blockNum-1);					//   to HF_HEADER gia to pleon plirws gemato block.
										//   Ean to to block einai gemato i 'ret' exei timi '1'
										//   alliws '0'.
	if (BF_UnpinBlock(fileDesc, blockNum, 1) != BFE_OK)			//-> Kanei unpin to block pou eisigage tin eggrafi me
		return (HF_errno = HFE_BFERROR);				//   dirty=1.
	if (BF_UnpinBlock(fileDesc, 0, ret) != BFE_OK)				//-> Kanei unpin to HF_HEADER me dirty iso me tin timi
		return (HF_errno = HFE_BFERROR);				//   tis 'ret' pou epistrafike parapanw.

	return ((blockNum-1)*max_records + recordNum);
}



// ------------------------------------------------------------------------------------------------------------------------
//
// >>> RMM_DeleteRec ()
// 	I routina diagrafei tin eggrafi me anagnwristiko arithmo 'recId', apo to anoixto arxeio me anagnwristiko 'fileDesc'.
// 	To megethos tis eggrafis einai 'recordSize'.
// 	I routina epistrefei 'HFE_OK' an i diagrafi egine epituxws, diaforetika kapoion antistoixo kwdiko sfalmatos.
//

int RMM_DeleteRec(int fileDesc, int recId, int recordSize)
{
	int full, max_records, blockNum, recordNum;
	char *HF_HeadBuf, *blockBuf;


	if (( HF_HeadBuf = malloc(BF_BLOCK_SIZE)) == NULL ) {
		BF_Errno = BFE_NOMEM;
		return (HF_errno = HFE_BFERROR);
	}  
	if (( blockBuf = malloc ( BF_BLOCK_SIZE )) == NULL ) {
		BF_Errno = BFE_NOMEM;
		return (HF_errno = HFE_BFERROR);
	}

	if(BF_GetThisBlock(fileDesc, 0, &HF_HeadBuf) != BFE_OK)		//-> Fernei to HF_HEADER sti mnimi.
		return (HF_errno = HFE_BFERROR);			//

	max_records = BF_BLOCK_SIZE*8 / (recordSize*8 + 1);		//-> Ypologizei to megisto plithos eggrafwn pou
									//   xwraei to kathe block.
	blockNum = recId/max_records + 1;				//-> Vriskei poio einai to block sto opoio vrisketai
									//   i eggrafi pou zitithike na diagrapsei.
	if(BF_GetThisBlock(fileDesc, blockNum, &blockBuf) != BFE_OK)	//-> Fernei stin mnimi to block sto opoio vrisketai
		return (HF_errno = HFE_BFERROR);			//   i eggrafi pou zitithike na diagrapsei.

	recordNum = recId % max_records;				//-> Vriskei ti thesi tis eggrafis mesa sto block.
	if(!isBitActive(blockBuf, BF_BLOCK_SIZE*8-1 -recordNum))	//-> Elegxei ean i eggrafi pou zitithike na diagrafei
	{	if (BF_UnpinBlock(fileDesc, blockNum, 0) != BFE_OK)	//   ontws uparxei mesa sto block.
			return (HF_errno = HFE_BFERROR);
		return (HF_errno = HFE_INVALIDREC);
	}

	resetBit(blockBuf, BF_BLOCK_SIZE*8-1 -recordNum);		//-> "Diagrafei" ti zitoumeni eggrafi sto block,
									//   midenizontas tin thesi tis sto bitmap tou block.
	if(( full = isBitActive(HF_HeadBuf, blockNum-1 )))		//-> Elegxei to HF_HEADER gia na dei ean to block prin
		resetBit(HF_HeadBuf, blockNum-1);			//   tin diagrafi tis eggrafis, itan plirws gemato.
									//   Ean nai, enimerwnei to HF_HEADER gia tin nea
									//   katastasi tou block.
	if(BF_UnpinBlock(fileDesc, blockNum, 1) != BFE_OK)		//-> Kanei unpin to block.
		return (HF_errno = HFE_BFERROR);			//
	if(FullOrEmpty(blockBuf, max_records, BF_BLOCK_SIZE, 0))	//-> Elegxei, ean meta apo tin diagrafi tis eggrafis
		if(BF_DisposeBlock(fileDesc, blockNum) != BFE_OK)	//   pou molis egine, to block den periexei kammia
			return (HF_errno = HFE_BFERROR);		//   eggrafi. Ean nai, kanei dispose to adeio block.

	if(BF_UnpinBlock(fileDesc, 0, full) != BFE_OK)			//-> Kanei unpin to HF_HEADER me dirty iso me 'full',
		return (HF_errno = HFE_BFERROR);			//   i opoia exei timi '0' i '1' analoga me to ean
									//   to block itan prin diagrafei oxi plirws gemato
									//   i plirws gemato, antistoixa.
	return HFE_OK;	
}



// ------------------------------------------------------------------------------------------------------------------------
//
// >>> RMM_GetFirstRec ()
// 	I routina auti diavazei stin endiamesi mnimi 'record' tin eggrafi pou antistoixei stin prwti diathesimi kai
// 	proerxetai apo to anoixto arxeio me anagnwristiko arithmo 'fileDesc'. Kathe eggrafi sto arxeio exei megethos
// 	'recordSize'.
// 	Epistrefei enan mi arnitiko akeraio arithmo, to anagnwristiko tis eggrafis, an i eggrafi anaktithike epituxws
// 	i ton antistoixo kwdiko sfalmatos.
//

int RMM_GetFirstRec(int fileDesc, char *record, int recordSize)
{
	int max_records, blockNum, i;
	char *blockBuf;

	if ((blockBuf = malloc(BF_BLOCK_SIZE)) == NULL) {
		BF_Errno = BFE_NOMEM;
		return (HF_errno = HFE_BFERROR);
	}

	max_records = BF_BLOCK_SIZE*8 / (recordSize*8 + 1);			//-> Ypologizei to megisto plithos eggrafwn
										//   pou xwraei to kathe block.
	blockNum = 0;								//-> Tha psaksei na vrei to 1o eguro block
										//   meta to 'block 0' dld meta to HF_HEADER.
	if(BF_GetNextBlock(fileDesc, &blockNum, &blockBuf) != BFE_OK )		//-> Fernei to prwto block pou vrei me estw mia
		return (HF_errno = HFE_BFERROR);				//   eggrafi.

	for(i=0 ; i < max_records ; i++)					//-> Gia kathe thesi eggrafis sto block, koitaei
		if(isBitActive(blockBuf, BF_BLOCK_SIZE*8-1 -i)) {		//   ean uparxei eggrafi.
			memcpy(record, blockBuf+i*recordSize, recordSize);	//-> Ean nai, adigrafei tin eggrafi stin 'record'.
			break;							//
		}

	if (BF_UnpinBlock(fileDesc, blockNum, 0 ) != BFE_OK )			//-> Kanei unpin to block pou efere sti mnimi.
		return (HF_errno = HFE_BFERROR);

	return ((blockNum-1)*max_records + i);
}



// ------------------------------------------------------------------------------------------------------------------------
//
// >>> RMM_GetNextRec ()
// 	I routina auti diavazei apo to anoixto arxeio me anagnwristiko arithmo 'fileDesc' stin endiamesi mnimi 'record'
// 	tin prwti eguri eggrafi pou uparxei meta apo autin me anagnwristiko arithmo 'recId'. To megethos kathe eggrafis
// 	tou arxeiou dinetai apo to 'recordSize'. Gia tin eggrafi 'recId' den ginetai kanenas elegxos egurotitas.
// 	Epistrefei enan mi arnitiko akeraio arithmo, to anagnwristiko tis eggrafis, an i epomeni eggrafi anaktithike
// 	epituxws i antistoixo kwdiko sfalmatos.
//

int RMM_GetNextRec(int fileDesc, int recId, char *record, int recordSize)
{
	int ret, i, max_records, blockNum, startingRec;
	char *blockBuf;


	if (( blockBuf = malloc(BF_BLOCK_SIZE)) == NULL) {
		BF_Errno = BFE_NOMEM;
		return (HF_errno = HFE_BFERROR);
	}

	max_records = BF_BLOCK_SIZE*8 / (recordSize*8 + 1);			//-> Ypologizei to megisto plithos eggrafwn
										//   pou xwraei to kathe block.
	blockNum = recId / max_records + 1;					//-> Ypologizei poio einai to block sto opoio
										//   vrisketai i 'recId'-iosti eggrafi.
	startingRec = recId % max_records + 1;					//-> Vriskei ti thesi tis epomenis eggrafis mesa
										//   sto block. Ean ksepernaei to 'max_records'
										//   den einai provlima.
	if (BF_GetThisBlock(fileDesc, blockNum, &blockBuf) != BFE_OK )		//-> Fernei stin mnimi to block sto opoio vrisketai
		return (HF_errno = HFE_BFERROR);				//   i eggrafi me anagnwristiko 'recId'.

	do {									//-> Gia to block pou exei erthei sti mnimi,
		for(i = startingRec ; i < max_records; i++)				//-> Gia kathe thesi eggrafis tou block
			if (isBitActive(blockBuf, BF_BLOCK_SIZE*8-1 -i)) {		//   ksekinontas apo tin 'startingRec',
											//   elegxei ean uparxei eggrafi.
				memcpy(record, blockBuf+i*recordSize, recordSize);	//-> Ean nai, adigrafei tin eggrafi sto
											//   'record'.
				if (BF_UnpinBlock(fileDesc, blockNum, 0) != BFE_OK)	//-> Kanei unpin to block pou exei ferei
					return (HF_errno = HFE_BFERROR);		//   sti mnimi.
				return ((blockNum-1)*max_records + i);
			}

		if (BF_UnpinBlock(fileDesc, blockNum, 0) != BFE_OK)			//-> Kanei unpin to block pou exei ferei
			return (HF_errno = HFE_BFERROR);				//   sti mnimi.
		startingRec = 0;							//-> Thetei tin arxiki thesi anazitisis
											//   eggrafis enos block se '0' (1i thesi).
	} while ((ret = BF_GetNextBlock(fileDesc, &blockNum , &blockBuf)) == BFE_OK );	//-> Den exei vrethei akomi i zitoumeni "epomeni
											//   eggrafi", ki etsi psaxnei sto epomeno block.
	if (ret == BFE_EOF)	return (HF_errno = HFE_EOF);
	else			return (HF_errno = HFE_BFERROR);
}



// ------------------------------------------------------------------------------------------------------------------------
//
// >>> RMM_GetThisRec ()
//	I routina diavaei apo to anoixto arxeio me anagnwristiko arithmo 'fileDesc' tin eggrafi me anagnwristiko 'recId'.
//	I eggrafi apothikevetai stin endiamesi mnimi 'record'. To megethos kathe eggrafis tou arxeiou einai 'recordSize'.
//	Epistrefei 'HFE_OK' an i eggrafi anaktithike epituxws i antistoixo kwdiko sfalmatos.
//

int RMM_GetThisRec(int fileDesc, int recId, char *record, int recordSize)
{
	int max_records, blockNum, recordNum;
	char *blockBuf;


	if (( blockBuf = malloc(BF_BLOCK_SIZE)) == NULL) {
		BF_Errno = BFE_NOMEM;
		return (HF_errno = HFE_BFERROR);
	}

	max_records = (BF_BLOCK_SIZE*8) / (recordSize*8 + 1);		//-> Ypologizei to megisto plithos eggrafwn
									//   pou xwraei to kathe block.
	blockNum = recId / max_records + 1;				//-> Vriskei poio einai to block sto opio vrisketai
									//   i eggrafi pou zitithike na diavasei.
	recordNum = recId % max_records;				//-> Vriskei ti thesi tis eggrafis mesa sto block.

	if (BF_GetThisBlock(fileDesc, blockNum, &blockBuf) != BFE_OK ) 	//-> Fernei sti mnimi to block sto opoio
		return (HF_errno = HFE_BFERROR);			//   vrisketai i eggrafi pou zitithike.

	if (!isBitActive(blockBuf, BF_BLOCK_SIZE*8-1 -recordNum)) {	//-> Elegxei ean uparxei i eggrafi pou zitithike.
		if (BF_UnpinBlock(fileDesc, blockNum, 0) != BFE_OK)	//-> Kanei unpin to block pou efere sti mnimi.
			return (HF_errno = HFE_BFERROR);		//
		return (HF_errno = HFE_INVALIDREC);
	}


	memcpy(record, blockBuf+recordNum*recordSize, recordSize);	//-> Adigrafei tin eggrafi sto 'record'.
	if(BF_UnpinBlock(fileDesc, blockNum, 0) != BFE_OK )		//-> Kanei unpin to block pou efere proigoumenws
		return (HF_errno = HFE_INVALIDREC);			//   sti mnimi.

	return HFE_OK;
}



