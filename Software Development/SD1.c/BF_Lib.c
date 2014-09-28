/************************************************
 *						*
 *						*
 *	ΣΑΚΕΛΛΑΡΗ ΕΛΙΣΑΒΕΤ 1115200600152	*
 *						*
 *						*
 ***********************************************/


#include "BF_Lib.h"
#include "BMM_Lib.h"
#include "FMM_Lib.h"
#include "HDF_Lib.h"

/*Αρχικοποιεί τις εσωτερικές δομές που χρησιμοποιεί η μονάδα διαχείρισης αρχείων. Αυτές περιλαμβάνουν 
 *περιγραφείς / αναγνωριστικά αρχείων (του λειτουργικού συστήματος ή του FMM), ονόματα αρχείων κ.λπ.
 */
void BF_Init( void )
{
	FMM_Init();
	BMM_Init();
}

/*Η ρουτίνα αυτή δημιουργεί ένα αρχείο με όνομα fileName. Το αρχείο δεν πρέπει να υπάρχει ήδη.
 *Η ρουτίνα επιστρέφει BFE_OK εάν το καινούργιο αρχείο δημιουργηθεί με επιτυχία, ειδ' αλλοιώς κάποιον κωδικό σφάλματος.
 */
int BF_CreateFile(char *fileName)
{
	char header[BF_BLOCK_SIZE];
	FILE* fp;

	//dimiourgw to header kai to gemizw me mhdenika
	memset(header, 0, BF_BLOCK_SIZE);

	//elegxw an yparxei idi to arxeio, an yparxei epistrefei error
	if( HDF_ExistsFile(fileName) == TRUE )
	{	
		BF_Errno  = BFE_FILEEXISTS;
		return BF_Errno;  
	}

	//dimiourgia tou arxeiou kai grapsimo tou header stin arxi
	if(HDF_CreateFile(fileName) != BFE_OK)
	return BF_Errno;

	if(HDF_OpenFile(fileName,&fp)!= BFE_OK)
	return BF_Errno;

	if(HDF_WriteBlock(0,header,fp)!= BFE_OK)
	return BF_Errno;

	if(HDF_CloseFile(fp) != BFE_OK)
	return BF_Errno;

	return BFE_OK;

}


/*Η ρουτίνα αυτή καταστρέφει το αρχείο fileName, σβήνοντας όλα τα δεδομένα του. 
 *Το αρχείο πρέπει να υπάρχει ήδη αλλά να μην είναι ανοιχτό. Η ρουτίνα επιστρέφει BFE_OK εάν 
 *επιτύχει, ειδ' αλλοιώς κάποιον κωδικό σφάλματος.
 */
int BF_DestroyFile(char *fileName)
{

	//elegxw an den yparxei to arxeio, tote epistrefei error
	if(HDF_ExistsFile ( fileName ) == FALSE)
	{
		BF_Errno = BFE_FILENOTEXISTS;
		return BF_Errno;	
	}	
	
	//an einai anoixto epistrefei error
	if( FMM_IsOpen(fileName)== TRUE )
	{
		BF_Errno = BFE_CANNOTDESTROYFILE;
		return BF_Errno;
	}

	//adeiazw tin endiamesi mnimi apo block tou arxeiou aytou
	if( BMM_Clear(fileName)!= BFE_OK)
	{
		BF_Errno = BFE_FILEHASFIXEDBLOCKS;
		return BF_Errno;
	}

	if(HDF_DestroyFile(fileName)!= BFE_OK)
	return BF_Errno;
	
	return BFE_OK;

}

/*Η ρουτίνα αυτή ανοίγει το αρχείο fileName. Εάν το αρχείο ανοιχτεί κανονικά, η ρουτίνα επιστρέφει έναν μικρό μη αρνητικό ακέραιο,
 *ο οποίος χρησιμοποιείται για να αναγνωρίζεται το αρχείο (όπως περιγράφουμε παρακάτω). Ειδ' αλλοιώς επιστρέφει κάποιον κωδικό σφάλματος.
 */
int BF_OpenFile (char *fileName)
{
	int fd;

	//an den yparxei to arxeio epistrefei lathos
	if(HDF_ExistsFile(fileName) == FALSE)
	{
		BF_Errno = BFE_FILENOTEXISTS;
		return BF_Errno;
	}

	//anoigma tou arxeiou kai epistrofi tou file descriptor
	if((fd=FMM_Open(fileName))<0)
	return BF_Errno;
	
	else return fd;

}

/*Η ρουτίνα αυτή κλείνει το αρχείο με αναγνωριστικό αριθμό fileDesc. Επίσης σβύνει την όλη καταχώρηση που αντιστοιχεί στο αρχείο αυτό στον 
 *πίνακα ανοιχτών αρχείων. Επιστρέφει BFE_OK εάν το αρχείο κλείσει επιτυχώς, ειδ' αλλοιώς κάποιον κωδικό σφάλματος. Ένα από τα σφάλματα 
 *αντιστοιχεί σε απόπειρα κλεισίματος αρχείου ενώ κάποιο μπλοκ του είναι ``καρφωμένο'' στην ενδιάμεση μνήμη.
 */
int BF_CloseFile (int fileDesc)
{

	fileInfo_t finfo;

	finfo.fd = fileDesc;

	//elegxw an einai valid o filedescriptor pou dothike
	if ( FMM_IsValid ( fileDesc ) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}

	//gemizw me tis plirofories tou arxeioy to finfo
	if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
	return BF_Errno;

	//grafei ta blocks tou arxeiou prin to kleisei
	if(BMM_Flush(finfo)!= BFE_OK)
	return BF_Errno;

	if(FMM_Close(fileDesc)!= BFE_OK)
	return BF_Errno;
	
	return BFE_OK;

}

/*Η ρουτίνα αυτή διαβάζει στην ενδιάμεση μνήμη το πρώτο έγγυρο μπλοκ του αρχείου με αναγνωριστικό αριθμό fileDesc και μετά 
 *βάζει τον δείκτη *blockBuf να δείχνει σ' αυτό. Επίσης αναθέτει στο *blockNum τον αριθμό του μπλοκ που διαβάστηκε. 
 *Προσέξτε ότι αυτό δεν είναι απαραίτητα το μπλοκ υπ' αριθμόν 1 (αν υποθέσουμε ότι η μέτρηση των μπλοκ ξεκινά από εκεί), 
 *καθότι το τελευταίο (και ίσως και άλλα) μπορεί να έχει επιστραφεί από το αρχείο στο σύστημα για ανακύκλωση. Το μπλοκ που διαβάστηκε
 *πρέπει να ``καρφωθεί'' στη μνήμη και να παραμείνει σ' αυτήν την κατάσταση μέχρι να ``ξεκαρφωθεί'' μέσω της ρουτίνας BF_UnpinBlock(), 
 *η οποία περιγράφεται παρακάτω. Η ρουτίνα επιστρέφει BFE_OK εάν επιτύχει, BFE_EOF εάν έχουμε φτάσει στο τέλος του αρχείου (δηλαδή, εάν το αρχείο 
 *δεν έχει κανένα μπλοκ), ή κάποιον κωδικό σφάλματος εάν κάτι δεν λειτουργήσει όπως πρέπει.
 */
int BF_GetFirstBlock(int fileDesc,int *blockNum ,char **blockBuf )
{
	fileInfo_t finfo;

	//elegxos an to fd einai valid
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}

	//epistrefei sto blocknum to prwto egyro block
	if((*blockNum = FMM_HEAD_GetFirstBlock(fileDesc))<0)
	{
		BF_Errno = BFE_EOF;
		return BF_Errno;
	}

	else 
	{
		finfo.fd = fileDesc;

		//gemizw me plirofories ti domi finfo kai vazw to block sti mnimi
		if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
		return BF_Errno;

		if(BMM_GetBlock(finfo,*blockNum,blockBuf)!= BFE_OK)
		return BF_Errno;

	}
	
	return BFE_OK;
}

/*Η ρουτίνα αυτή διαβάζει στην ενδιάμεση μνήμη το πρώτο έγγυρο μπλοκ του αρχείου με αναγνωριστικό αριθμό 
 *fileDesc μετά από το μπλοκ με αριθμό *blockNum και μετά βάζει τον δείκτη *blockBuf να δείχνει σ' αυτό. Επίσης, στο τέλος της διαδικασίας,
 *ενημερώνει το *blockNum ώστε να περιέχει τον αριθμό του νέου μπλοκ που διαβάστηκε. Προσέξτε και πάλι ότι αυτό δεν είναι απαραίτητα 
 *το αριθμητικά αμέσως επόμενο του *blockNum, καθότι το τελευταίο (και ίσως και άλλα) μπορεί να έχει επιστραφεί από το αρχείο στο σύστημα 
 *για ανακύκλωση. Το μπλοκ που διαβάστηκε πρέπει να ``καρφωθεί'' στη μνήμη και να παραμείνει σ' αυτήν την κατάσταση μέχρι να ``ξεκαρφωθεί'' 
 *μέσω της ρουτίνας BF_UnpinBlock(), η οποία περιγράφεται παρακάτω. Η ρουτίνα επιστρέφει BFE_OK εάν επιτύχει, BFE_EOF εάν έχουμε φτάσει στο
 *τέλος του αρχείου (δηλαδή δεν υπάρχει επόμενο μπλοκ), BFE_INVALIDBLOCK εάν ο αρχικά δοσμένος αριθμός μπλοκ (*blockNum) δεν αναφέρεται σε 
 *έγγυρο μπλοκ, ή κάποιον άλλον κωδικό σφάλματος εάν κάτι άλλο δεν λειτουργήσει όπως πρέπει. 
 */
int BF_GetNextBlock(int fileDesc ,int *blockNum ,char **blockBuf )
{
	fileInfo_t finfo;

	//elegxos an to fd einai valid
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}

	//elegxw an to block einai egyro 
	if(FMM_HEAD_IsValid(fileDesc,*blockNum) == FALSE)
	{
		BF_Errno = BFE_INVALIDBLOCK;
		return BF_Errno;
	}

	//epistrefei to prwto egyro block amesws meta to blocknum kai to vazei sto blocknum
	if((*blockNum = FMM_HEAD_GetNextBlock(fileDesc,*blockNum))< 0)
	return BF_Errno;

	else
	{
		finfo.fd = fileDesc;

		//gemizw me plirofories gia to arxeio to finfo kai vazw to block sti mnimi
		if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
		return BF_Errno;

		if(BMM_GetBlock(finfo,*blockNum,blockBuf)!= BFE_OK)
		return BF_Errno;
	}

	return BFE_OK;
}


/*Η ρουτίνα αυτή διαβάζει στην ενδιάμεση μνήμη το μπλοκ με αριθμό blockNum του αρχείου με αναγνωριστικό αριθμό
 *fileDesc και μετά βάζει τον δείκτη *blockBuf να δείχνει σ' αυτό. Ο αριθμός blockNum πρέπει να αναφέρεται σε 
 *έγγυρο μπλοκ. Το μπλοκ που διαβάστηκε πρέπει να ``καρφωθεί'' στη μνήμη και να παραμείνει σ' αυτήν την κατάσταση
 *μέχρι να ``ξεκαρφωθεί'' μέσω της ρουτίνας BF_UnpinBlock(), η οποία περιγράφεται παρακάτω. Η ρουτίνα επιστρέφει
 * BFE_OK εάν επιτύχει, BFE_INVALIDBLOCK εάν ο αρχικά δοσμένος αριθμός μπλοκ (blockNum) δεν αναφέρεται σε έγγυρο 
 *μπλοκ, ή κάποιον άλλον κωδικό σφάλματος εάν κάτι άλλο δεν λειτουργήσει όπως πρέπει. 
 */
int BF_GetThisBlock(int fileDesc,int blockNum,char **blockBuf)
{
	fileInfo_t finfo;

	//elegxw an to fd einai egyro
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}

	//elegxw an to block blocknum einai egyro
	if(FMM_HEAD_IsValid(fileDesc,blockNum) == FALSE)
	{
		BF_Errno = BFE_INVALIDBLOCK;
		return BF_Errno;

	}

	//gemizw me plirofories ti domi kai vazw to block sti mnimi
	finfo.fd = fileDesc;

	if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
	return BF_Errno;
	
	if(BMM_GetBlock(finfo,blockNum,blockBuf)!= BFE_OK)
	return BF_Errno;

	return BFE_OK;

}

/*Η ρουτίνα αυτή προσθέτει ένα νέο άδειο μπλοκ στο αρχείο με αναγνωριστικό αριθμό fileDesc. 
 *Αναθέτει τον αριθμό του νέου μπλοκ στο *blockNum και βάζει τον δείκτη *blockBuf να δείχνει
 *σε αντίστοιχο χώρο στην ενδιάμεση μνήμη. Το νέο μπλοκ μπορεί να είναι πρωτοεμφανιζόμενο ή 
 *να προέρχεται από ανακύκλωση. Επίσης το νέο μπλοκ πρέπει να ``καρφωθεί'' στη μνήμη και να 
 *παραμείνει σ' αυτήν την κατάσταση μέχρι να ``ξεκαρφωθεί'' μέσω της ρουτίνας BF_UnpinBlock(),
 *η οποία περιγράφεται παρακάτω. Η ρουτίνα επιστρέφει BFE_OK εάν επιτύχει ή κάποιον κωδικό 
 *σφάλματος εάν κάτι δεν λειτουργήσει όπως πρέπει.
 */
int BF_AllocBlock(int fileDesc,int *blockNum ,char **blockBuf)
{

	fileInfo_t finfo;
	
	//elegxos an to fd einai egyro
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}
	
	//epistrefei to prwto mi egyro block
	if((*blockNum=FMM_HEAD_GetFirstEmpty(fileDesc))<0)
	return BF_Errno;

	//gemizw me plirofories ti domi
	finfo.fd = fileDesc;
	
	if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
	return BF_Errno;

	//prosthetw to block sto arxeio
	if(BMM_AllocBlock(finfo,*blockNum,blockBuf) != BFE_OK)
	return BF_Errno;

	//kanw to antistoixo bit tou header 1
	if(FMM_HEAD_Set(fileDesc,*blockNum,TRUE) != BFE_OK)
	return BF_Errno;

	return BFE_OK;
}


/*Η ρουτίνα αυτή επιστρέφει προς ανακύκλωση το μπλοκ με αριθμό blockNum του αρχείου με αναγνωριστικό αριθμό fileDesc. 
 *To μπλοκ δεν μπορεί να είναι ``καρφωμένο'' στην ενδιάμεση μνήμη. Η ρουτίνα επιστρέφει BFE_OK εάν επιτύχει ή κάποιον
 *κωδικό σφάλματος εάν κάτι δεν λειτουργήσει όπως πρέπει.
 */
int BF_DisposeBlock(int fileDesc,int blockNum)
{
	 fileInfo_t finfo;

	//elegxos an to fd einai egyro	
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}

	//an to block den einai egyro, epistrefei lathos
	if(FMM_HEAD_IsValid(fileDesc,blockNum) == FALSE)
	{
		BF_Errno = BFE_INVALIDBLOCK;
		return BF_Errno;
	}

	//gemizw me plirofories gia to arxeio to finfo
	finfo.fd = fileDesc;

	if(FMM_GetFileInfoByFD(&finfo)!= BFE_OK)
	return BF_Errno;

	//diwxnw t block apo ti mnimi
	if(BMM_DisposeBlock(finfo,blockNum)!= BFE_OK)
	return BF_Errno;
	
	//thetei to antistoixo bit tou header se 0
	if(FMM_HEAD_Set(fileDesc,blockNum,FALSE)!= BFE_OK)
	return BF_Errno;
	
	return BFE_OK;

}

/*Η ρουτίνα αυτή ειδοποιεί ότι το μπλοκ με αριθμό blockNum του αρχείου με αναγνωριστικό αριθμό fileDesc δεν χρειάζεται πια στην
 *ενδιάμεση μνήμη και μπορεί να ``ξεκαρφωθεί''. Η παράμετρος dirty είναι ίση με TRUE=1 εάν το μπλοκ έχει αλλάξει στην ενδιάμεση μνήμη από τη στιγμή που 
 *``καρφώθηκε'' εκεί (οπότε σε περίπτωση αντικατάστασής του πρέπει πρώτα να γραφτεί στη θέση του στο δίσκο), ειδ' αλλοιώς είναι ίση με FALSE=0. 
 *Η ρουτίνα επιστρέφει BFE_OK εάν επιτύχει ή κάποιον κωδικό σφάλματος εάν κάτι δεν λειτουργήσει όπως πρέπει. 
 */
int BF_UnpinBlock(int fileDesc,int blockNum,int dirty)
{
	fileInfo_t finfo;
         //metavliti pou elegxei an to block einai dirty i oxi
	BOOLEAN dirt;

	//elegxos an to fd einai valid
	if (FMM_IsValid(fileDesc) == FALSE )
	{
		BF_Errno = BFE_FD;
		return BF_Errno;
	}
	
	//elegxos an to block einai egyro
	if(FMM_HEAD_IsValid ( fileDesc, blockNum ) == FALSE)
	{
		BF_Errno = BFE_INVALIDBLOCK;
		return BF_Errno;
	}

	//an den exei ginei allagi sto block thetw ti metavliti dirt =  false
	if(dirty == 0)
	dirt = FALSE;

	//an exei ginei allagi ti thetei true
	else dirt = TRUE;

	//gemizei me plirofories to finfo kai ginetai unpin to block
	finfo.fd = fileDesc;

	if ( FMM_GetFileInfoByFD ( &finfo ) != BFE_OK )	
	return BF_Errno;

	if(BMM_UnpinBlock(finfo,blockNum,dirt) != BFE_OK)
	return BF_Errno;

	return BFE_OK;

}

/*Η ρουτίνα τυπώνει το κείμενο που δείχνει η παράμετρος errString, και μετά τυπώνει το μήνυμα που αντιστοιχεί 
 *στο τελευταίο σφάλμα που προέκυψε από οποιαδήποτε από τις ρουτίνες. Για τον σκοπό αυτό, η ρουτίνα αυτή 
 *χρησιμοποιεί μία καθολική (global) μεταβλητή BF_errno η οποία αποθηκεύει πάντα τον κωδικό του πλέον πρόσφατου
 *σφάλματος. Ο κωδικός αυτός σφάλματος πρέπει πάντα να ενημερώνεται σωστά σε όλες τις άλλες ρουτίνες.
 *Η ρουτίνα αυτή δεν έχει δική της τιμή επιστροφής.
 */
void BF_PrintError(char *errString )
{
	printf("%s\n",errString);

	 if(BF_Errno == BFE_NOMEM )
	puts("No memory available");              /* Δεν υπάρχει διαθέσιμη μνήμη */

	if (BF_Errno==BFE_CANNOTOPENFILE)
	puts("Cannot open file");                 /* Αδύνατο το άνοιγμα του αρχείου */

	if( BF_Errno==BFE_CANNOTCLOSEFILE)
	puts("Cannot Close file");                /* Αδύνατο το κλείσιμο του αρχείου */

	if (BF_Errno == BFE_CANNOTCREATEFILE)
	puts("Cannot create file");              /* Αδύνατη η δημιουργία του αρχείου */

	if(BF_Errno== BFE_INCOMPLETEREAD)
	puts("Incomplete read");                 /* Ημιτελές διάβασμα */

	if (BF_Errno == BFE_INCOMPLETEWRITE)
	puts("Incomplete write");                /* Ημιτελές γράψιμο */

	if(BF_Errno == BFE_FILEEXISTS)
	puts("File already exists");              /* Το αρχείο υπάρχει ήδη */

	if(BF_Errno == BFE_NOBUF)
	puts("No space in buffer available");     /* Έλλειψη χώρου στην ενδιάμεση μνήμη */

	if(BF_Errno==BFE_FILEOPEN)
	puts("File already open");                /* Το αρχείο είναι ήδη ανοιχτό */

	if(BF_Errno==BFE_FD)
	puts("Wrong fd");                          /* Λανθασμένο αναγνωριστικό αρχείου */

	if(BF_Errno == BFE_FILENOTEXISTS)
	puts("file does not exist");               /* Το αρχείο δεν υπάρχει */

	if(BF_Errno == BFE_FTABFULL)
	puts("Tab is full");                        /* Λίστα ανοιχτών αρχείων πλήρης */

	if(BF_Errno == BFE_HEADOVERFLOW)
	puts("Head overflow");                      /* Λανθασμένος αριθμός  */

	if(BF_Errno== BFE_BLOCKFIXED)
	puts("Block already fixed");                /* Μπλοκ ήδη "καρφωμένο" στη μνήμη */

	if(BF_Errno == BFE_BLOCKUNFIXED)
	puts("Block already unfixed");              /* Μπλοκ ήδη ξεκαρφωμένο */

	if(BF_Errno==BFE_EOF)
	puts("End of file");                        /* Τέλος αρχείου */

	if(BF_Errno== BFE_FILEHASFIXEDBLOCKS)
	puts("the file has fixed blocks");          /* Το αρχείο έχει καρφωμένα block */

	if(BF_Errno==BFE_BLOCKFREE)
	puts("Block already free");	                /* Μπλοκ ήδη διαθέσιμο προς ανακύκλωση */

	if(BF_Errno == BFE_BLOCKINBUF)
	puts("Block already in buffer");            /* Μπλοκ ήδη στη μνήμη */

	if(BF_Errno == BFE_BLOCKNOTINBUF);
	puts("Block not in buffer");                /* Μπλοκ όχι στη μνήμη */

	if(BF_Errno == BFE_INVALIDBLOCK)
	puts("invalid block code");                 /* Λανθασμένος κωδικός block */

	if(BF_Errno == BFE_CANNOTDESTROYFILE)
	puts("Cannot destroy file");                /* Αδύνατη η καταστροφή του αρχείου */


}
