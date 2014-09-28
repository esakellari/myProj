/* **********************************************
 *						*
 *						*
 * 	ΣΑΚΕΛΛΑΡΗ ΕΛΙΣΑΒΕΤ 111520600152		*
 *						*
 *						*
 * **********************************************/



#include "BMM_Lib.h"


/*Η ρουτίνα αυτή χρησιμοποιείται για να αρχικοποιήσετε τις όποιες καθολικές (global) εσωτερικές δομές δεδομένων 
 * αποφασίσετε ότι χρειάζεστε. Δεν έχει καμία παράμετρο εισόδου και δεν παράγει καμία έξοδο.
 *
 */

void BMM_Init()
{
	int i,j;

	//arxikopoihsh tou global rologiou
	glob_clock = 0;

	for(i = 0; i < BF_BUFFER_SIZE; i++)
	{
		Blocks[i].free = TRUE;	
		Blocks[i].blocknum = -1;
		Blocks[i].dirty = FALSE;
		Blocks[i].pincount = 0;

		Blocks[i].lastUsed = -1;

		memset(Blocks[i].data, 0, BF_BLOCK_SIZE);

				
		for(j = 0; j < MAXOPENFILES; j++)
		{
			Blocks[i].pinned[j] = FALSE;
		}
	}
}

/*Η ρουτίνα ψάχνει στην ενδιάμεση μνήμη να βρει block(s) που προέρχονται από το αρχείο με όνομα finfo.filename. 
 *Αν το συγκεκριμένο block που βρίσκει είναι καρφωμένο στη μνήμη από το finfo.fd, τότε πρέπει να επιστραφεί κάποιος 
 *σχετικός κωδικός σφάλματος. Αν δεν είναι καρφωμένο από το finfo.fd, τότε εξετάζει αν είναι καρφωμένο γενικά από άλλα ανοίγματα αρχείων. 
 *Στην περίπτωση που δεν είναι καρφωμένο γενικά και έχει αλλαχθεί απ' όταν φορτώθηκε στη μνήμη (πληροφορία για dirty block στη μνήμη είναι TRUE), 
 *τότε και μόνο γράφεται στο δίσκο. Αν το block είναι καρφωμένο από άλλα ανοίγματα αρχείων, η εκτέλεση του κώδικα συνεχίζει κανονικά, χωρίς να γραφτεί 
 *το block πίσω στο δίσκο.
 */
int BMM_Flush(fileInfo_t finfo)
{
	int i;

	for(i = 0; i <  BF_BUFFER_SIZE; i++)
	{
		//elegxw me vasi to onoma tou arxeiou pou krataei i thesi
		if(strcmp(Blocks[i].filename, finfo.filename) == 0)
		{
			if(Blocks[i].pinned[finfo.fd] == TRUE)
			{
				BF_Errno = BFE_BLOCKPINNEDBYFILE;
				return BF_Errno;
			}

			if((Blocks[i].pincount == 0) && (Blocks[i].dirty == TRUE))
			{
			 	if(HDF_WriteBlock(Blocks[i].blocknum+1,Blocks[i].data,finfo.fp)!= BFE_OK)
			 	return BF_Errno;
			}
		}
	}	
	return BFE_OK;
}

/*ayti i synartisi xrhsimopoieitai apo tis getblock kai alloc block gia na vriskei thesi ston pinaka twn block.
 *Arxika psaxnei gia to an to block yparxei idi, ston pinaka, meta an den yparxei koitaei an yparxei eleytheri thesi, 
 *kai telos, an den yparxei oute eleytheri thesi , kanei lru. Se oles tis periptwseis epistrefei tin thesi tou 
 *pinaka pou vrhke kai dinei timi stin metavliti cases, etsi wste i kalousa synartisi na kserei gia poia apo tis 3 
 *periptwseis prokeitai
 */
int Search(char* filename, int blocknum,int* cases)
{
	int i,j,pinc,un_block,min;
	min = 0;

	//elexgw an to sygekrimeno block yparxei idi ston pinaka
	for(i = 0; i <  BF_BUFFER_SIZE; i++)
	{
		//elegxw mono oses theseis tou pinaka einai free
		if(Blocks[i].free == FALSE)
		{
			//elegxw na dw an einai to idio arxeio k to idio block 
			if((strcmp(filename, Blocks[i].filename) == 0) && (Blocks[i].blocknum == blocknum   ))
			{
				*cases = 0;
				return i;
			}
		}
	}
	//to block den yparxei idi
	//psaxnw keni thesi ston pinaka twn block
	for(i = 0; i < BF_BUFFER_SIZE; i++)
	{
		if(Blocks[i].free == TRUE)
		{	
			*cases = 1;
			return i;
		}
	}
	//an den vrika xwro ston pinaka twn block kanw LRU gia na diwksw kapoio block
	for(i = 0; i < BF_BUFFER_SIZE; i++)
	{
		//psaxnw ta block pou den einai pinned
		if(Blocks[i].pincount == 0)
		{
			//an den einai to prwto block elegxw an to vrethike mikroteri timi apo to min
			if(min > Blocks[i].lastUsed)
			{
				min = Blocks[i].lastUsed;
				un_block = i;
			}
			//tin prwti fora, to min pairnei tin timi lastUsed tou prwtou block p vriskei
			if( min == 0)
			{
				min =  Blocks[i].lastUsed;
				un_block = i;
			}
		}
	}

	//an vrethike kapoio block gia diwksimo
	if(min != 0)
	{
		*cases = 2;	
		return un_block;
	}
	//an de vrethike block gia diwksimo 
	else 
	{
		*cases = 3;
		return 0;
	}
}
/*Η ρουτίνα αυτή διαβάζει στην ενδιάμεση μνήμη το μπλοκ με αναγνωριστικό blockNum του αρχείου που 
 * προσδιορίζεται από το finfo και μετά βάζει τον δείκτη *blockBuf να δείχνει σ' αυτό.
 */
int BMM_GetBlock(fileInfo_t finfo,int blockNum,char** blockBuf)
{
	int i,j, min,pinc,un_block,cases;
	fileInfo_t flru;	//domi pou xrhsimopoiw gia na krathsw ta stoixeia tou block pou diwxnw me lru
	min = 0;

	//kaleitai i synartisi pou vriskei thesi ston pinaka twn block
	i =  Search(finfo.filename, blockNum,&cases);

	//periptwsi pou to block yparxei idi
	if(cases == 0)
	{
		Blocks[i].lastUsed = glob_clock++;		//ayksanw to global roloi
		Blocks[i].pincount++;				//ayksanw ton metriti twn karfwmatwn
		Blocks[i].pinned[finfo.fd] = TRUE;
		return BFE_OK;
	}

	//periptwsi pou vriskoume adeia thesi
	if(cases == 1)
	{
		//diavazw to block apo to arxeio
		if(HDF_ReadBlock(blockNum+1,Blocks[i].data,finfo.fp) != BFE_OK)
				return BF_Errno;

		*blockBuf = Blocks[i].data;					//o deiktis blockbuf deixnei sta data
		strcpy(Blocks[i].filename, finfo.filename);			//krataw to onoma tou arxeiou
		Blocks[i].free = FALSE;						//i thesi den einai pia eleytheri
		Blocks[i].blocknum = blockNum;					
		Blocks[i].lastUsed = glob_clock++;				//ayksanw to global clock kai to clock tou block
		Blocks[i].pinned[finfo.fd] =  TRUE;	
		Blocks[i].pincount=1;						//afou vazw gia prwti fora to block pros to paron exei mono ena karfwma
		return BFE_OK;
	}
	//periptwsi pou kanw LRU
	if(cases == 2)
	{
		//an to block ou diwxnw exei allaksei
		if(Blocks[i].dirty == TRUE)
			{
				strcpy(flru.filename, Blocks[i].filename);

				//parinw ta stoixeia tou me tin antistoixi FMM synartisi
				if(FMM_GetFileInfoByName(&flru) !=  BFE_OK)
				return BF_Errno;
				//to grafw sto arxeio
				if(HDF_WriteBlock(Blocks[i].blocknum+1,Blocks[i].data,flru.fp )!= BFE_OK)
				return BF_Errno;
			}

			//diavazw to neo block
			if(HDF_ReadBlock(blockNum+1,Blocks[i].data,finfo.fp) != BFE_OK)
			return BF_Errno;

			//allazw katallila ta stoixeia opws kai tin periptwsi parapanw, opou eixa vrei adeia thesi
			*blockBuf = Blocks[i].data;
			strcpy(Blocks[i].filename, finfo.filename);
			Blocks[i].free = FALSE;
			Blocks[i].blocknum = blockNum;				
			Blocks[i].lastUsed = glob_clock++;
			Blocks[i].pinned[finfo.fd] = TRUE;
			Blocks[i].pincount=1;
			Blocks[i].dirty = FALSE;
			return BFE_OK;
	}
	//de vethike xwros
	if ( cases == 3 )
	{
		BF_Errno = BFE_NOBUF;
		return BF_Errno;
	}
}

/*Η ρουτίνα αυτή προσθέτει ένα νέο άδειο μπλοκ στην ενδιάμεση μνήμη για το αρχείο που προσδιορίζεται από το finfo. 
 * Το άδειο block αντιστοιχεί στο νοητό bock υπ' αριθμό blockNum και βάζει τον δείκτη *blockBuf να δείχνει 
 * σε αντίστοιχο χώρο στην ενδιάμεση μνήμη.
 */
int BMM_AllocBlock(fileInfo_t finfo,int blockNum,char** blockBuf)
{
	int i,cases;
	fileInfo_t flru;  //domi stin opoia vazw ta stoixeia tou block pou tha diwksw apo tin lru

	i =  Search(finfo.filename, blockNum,&cases);
	
	//periptwsi pou to block yparxei idi
	if(cases == 0)
	{
		BF_Errno = BFE_BLOCKINBUF;
		return BF_Errno;
	}

	//periptwsi pou vriskoume adeia thesi
	if(cases == 1)
	{
		//enhmerwnw katallila ta stoixeia tis thesis tou pinaka
		strcpy(Blocks[i].filename, finfo.filename);
		Blocks[i].free = FALSE;
		Blocks[i].blocknum = blockNum;
		Blocks[i].lastUsed = glob_clock++;
		Blocks[i].pinned[finfo.fd] =  TRUE;
		Blocks[i].pincount=1;
		memset(Blocks[i].data, 0, BF_BLOCK_SIZE);
		*blockBuf = Blocks[i].data;
		Blocks[i].dirty = TRUE;
		return BFE_OK;
	}

	//periptwsi pou kanw LRU
	if(cases == 2)
	{
		//an to block exei allaksei to grafw sto arxeio
		if(Blocks[i].dirty == TRUE)
		{
			strcpy(flru.filename, Blocks[i].filename);

			if(FMM_GetFileInfoByName(&flru) !=  BFE_OK)
			return BF_Errno;
	
			if(HDF_WriteBlock(Blocks[i].blocknum+1,Blocks[i].data,flru.fp )!= BFE_OK)
			return BF_Errno;
		}

		//kata ton idio tropo allazw ta stoixeia tis thesis tou pinaka katallila
		strcpy(Blocks[i].filename, finfo.filename);
		Blocks[i].free = FALSE;
		Blocks[i].blocknum = blockNum;				
		Blocks[i].lastUsed = glob_clock++;
		Blocks[i].pinned[finfo.fd] = TRUE;
		Blocks[i].dirty = TRUE;
		Blocks[i].pincount=1;
		memset(Blocks[i].data, 0, BF_BLOCK_SIZE);
		*blockBuf = Blocks[i].data;

		return BFE_OK;
	}
	
	//de vethike xwros
	if ( cases == 3)
	{
		BF_Errno = BFE_NOBUF;
		return BF_Errno;
	}
}


/*Η ρουτίνα αυτή "καθαρίζει" την ενδιάμεση μνήμη από τυχόν μπλοκ του αρχείου με όνομα fileName.
 */
int BMM_Clear(char* fileName)
{
	int i;

	for(i = 0; i < BF_BUFFER_SIZE; i++)
	{
		//apla psaxnw ta block pou antistoixoun sto sygekrimeno arxeio kai kanw ayti ti thesi free
		//allazontas ti metavliti
		if( strcmp( fileName, Blocks[i].filename) == 0)
		{
			Blocks[i].free =  TRUE;
		}	
	}

	return BFE_OK;
}

/*Η ρουτίνα αυτή ειδοποιεί ότι το μπλοκ με αριθμό blockNum του αρχείου που προσδιορίζεται από το finfo δεν χρειάζεται 
 * πια στην ενδιάμεση μνήμη και μπορεί να "ξεκαρφωθεί''. Η παράμετρος dirty είναι ίση με TRUE εάν το μπλοκ έχει αλλάξει 
 * στην ενδιάμεση μνήμη από τη στιγμή που "καρφώθηκε" εκεί (οπότε σε περίπτωση αντικατάστασής του πρέπει πρώτα 
 * να γραφτεί στη θέση του στο δίσκο), ειδ' αλλοιώς είναι ίση με FALSE. Η ρουτίνα επιστρέφει BFE_OK εάν επιτύχει 
 * ή κάποιον κωδικό σφάλματος εάν κάτι δεν λειτουργήσει όπως πρέπει.
 */
int BMM_UnpinBlock(fileInfo_t finfo,int blockNum,BOOLEAN dirty)
{
	int i;

	for ( i = 0; i < BF_BUFFER_SIZE; i++)
	{
		//psaxnw symfwna me to onoma tou arxeiou kai to noumero tou block
		if(( strcmp(Blocks[i].filename, finfo.filename)==0) && (Blocks[i].blocknum == blockNum))
		{
			//an to block einai idi ksekarfwmeno epistrefei kwdiko sfalmatos
			if(Blocks[i].pinned[finfo.fd] == FALSE)
			{
				BF_Errno = BFE_BLOCKUNFIXED;
				return BF_Errno;
			}

			else
			{
				//to apopanw epipedo leei an exei allaksei i oxi to block
				if(dirty == TRUE)
				{	
					Blocks[i].dirty =  TRUE;		//an exei allaksei i metavliti dirty ginetai true
					Blocks[i].pinned[finfo.fd] = FALSE;	//afairw ton fd tou anoigmatos apo ton pinaka anoigmatwn
					Blocks[i].pincount--;			//meiwnetai o metritis karfwmatwn
				}
				else 
				{
					Blocks[i].pinned[finfo.fd] = FALSE;  	//an den exei allaksei
					Blocks[i].pincount--;			//apla meiwnw ton metriri karfwmatwn
				}
				return BFE_OK;
			}
		}
	}

	//an de vrethei to block sti mnimi epistrefetai lathos
	BF_Errno = BFE_BLOCKNOTINBUF;
	return BF_Errno;


}

/*Η ρουτίνα αυτή επιστρέφει προς ανακύκλωση το μπλοκ με αριθμό blockNum του αρχείου που προσδιορίζεται από το finfo. 
 * To μπλοκ δεν μπορεί να είναι ``καρφωμένο'' στην ενδιάμεση μνήμη. Η ρουτίνα επιστρέφει BFE_OK εάν επιτύχει ή 
 * κάποιον κωδικό σφάλματος εάν κάτι δεν λειτουργήσει όπως πρέπει.
 */

int BMM_DisposeBlock(fileInfo_t finfo,int blockNum)
{
	int i;

	for(i = 0; i <  BF_BUFFER_SIZE; i++)
	{
		//psaxnw symfwna me to onoma tou arxeiou, ton arithmo tou block kai to an einai unpinned
		if(( strcmp(Blocks[i].filename, finfo.filename)==0) && (Blocks[i].blocknum == blockNum)&&(Blocks[i].pincount == 0 ) )
		{
			Blocks[i].free = TRUE;
			return BFE_OK;
		}
	}

	//an de vrethei to block sti mnimi epistrefetai lathos
	BF_Errno = BFE_BLOCKNOTINBUF;
	return BF_Errno;
}

