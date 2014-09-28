#include <stdio.h>
#include <string.h>

#include "UT_DM_Defines.h"
#include "HF_Lib.h"

#include "SCM_Lib.h"

//-------------------------------------------------------------------------------------------
//
//---> SCM_create():
//		Ta orismata tis einai idia me tis sunartisis UT_create.
//		Epistrefei UTE_OK an o elegxos egine me epituxia eid'alliws kapoion kwdiko sfalmatos.
//


int checkAttrFormat(char *s)
{
	/* to s exei tin morfi 'i' i 'c30' ktlp*/
        int i, mult, number,len;

	// -> ean einai  'i'  i 'f' ola ok
        if ( s[1] == 'i' || s[1] == 'f')
			return 0;
		if ( s[1] != 'c') //-> ean den einai oute i oute f oute c tote error
			return 1;
		
		//-> an einai c tote elegxw ean exei dipla number 
        number = 0;

        for(i=strlen(s)-2, mult=1 ; i >= 2 ; i--, mult*=10) {        //-> Prepei kathe enas xaraktiras sto kommati meta to c na einai
                if(s[i] < '0'  ||  s[i] > '9')                  //   enas arithmos sto [0,9]. Ean oxi, epistrefei failure.
                        return 1;
                number += (s[i]-'0')*mult;                              //-> Ypologizei parallila ton arithmo pou uparxei sto kommati "XXX".
        }

        if(number > MAXNAME)                                           //-> Elegxei ean o arithmos pou upologistike einai megaluteros
                return 1;                                                               //   tou MAXNAME. Ean nai, epistrefei failure.
        return 0;
}



int SCM_create(int argc, char* argv[])
{
	int i, j, fd;

	if( argc < 4)		//-> Prepei ta orismata na einai toulaxiston 4 (create,onoma sxesis,onoma pediou,format pediou).
		return (UT_errno = UTE_SCM_INSUFFICIENTARGS);
    
	if( argv[argc] != NULL)		//-> Prepei to plithos twn orismatwn na einai oso dilwnei i argc.
		return (UT_errno = UTE_SCM_WRONGNUMOFARGS);
		
	if( strcmp(argv[0], "create"))		//-> Prepei to prwto orisma na einai to string "create".
		return (UT_errno = UTE_SCM_WRONGFIRSTARG);	
	
	if( strlen(argv[1]) > MAXNAME) 		//-> Prepei to onoma tis sxesis na min ksepernaei tous MAXNAME xaraktires.
		return (UT_errno = UTE_SCM_WRONGRELNAME);	

	//elegxos ean i sxesi/arxeio 'argv[1]' uparxei idi. Ean nai, epistrefei error

	if ( (fd = HF_OpenFile(argv[1]) ) >= 0) {
		if (HF_CloseFile(fd) != HFE_OK)
			return (UT_errno = UTE_HFERROR);	
		return (UT_errno = UTE_SCM_RELEXISTS);
	}

	for(i=2 ; i<argc ; i+=2) 
	{					//-> Gia kathe onoma pediou pou dothike san orisma,
		if( argv[i] == NULL)
			return( UT_errno = UTE_SCM_NULLNAME);
		
		if( argv[i+1] == NULL)
			return (UT_errno  = UTE_SCM_NULLFORMAT);
								//-> Elegxei kat'arxas ean exei dothei NULL adi gia onoma kai format pediou.
		if( strlen(argv[i]) > MAXNAME) 			//-> Elegxei ean to onoma pediou ksepernaei tous MAXNAME xaraktires.
			return (UT_errno = UTE_SCM_LONGNAME); 

		for(j=i-2 ; j >= 2 ; j-=2)					//-> Gia kathe pedio elegxei ean dothike allo pedio me idio onoma.
			if(! strcmp(argv[i], argv[j]))			//
				return (UT_errno = UTE_SCM_SAMENAME);

		if(checkAttrFormat(argv[i+1]))				//-> Elegxei meta apo kathe onoma pediou, exei dothei eguro format pediou.
			return (UT_errno = UTE_SCM_WRONGARGS);
	}
	return UTE_OK;
}


//-------------------------------------------------------------------------------------------
//
//---> SCM_buildindex():
//		Ta orismata tis einai idia me tis sunartisis UT_buildindex.
//		Epistrefei UTE_OK an o elegxos egine me epituxia eid'alliws kapoion kwdiko sfalmatos.
//

int SCM_buildIndex(int argc, char* argv[])
{
	int fd;
	
	if( argc != 3)		//-> Prepei to plithos twn orismatwn na einai 3 ("buildindex",onoma sxesis, onoma pediou).
		return (UT_errno = UTE_SCM_WRONGARGSBUILD); 
		
	if(argv[3] != NULL)				//-> Prepei to plithos twn orismatwn na einai oso dilwnei i argc.
		return (UT_errno = UTE_SCM_WRONGNUMOFARGS); 
		
	if(strcmp(argv[0], "buildindex"))		//-> Prepei to prwto orisma na einai to string "buildindex".
		return(UT_errno = UTE_SCM_WRONGFIRSTARG);
 	
	if(strlen(argv[1]) > MAXNAME)			//-> Prepei to onoma tis sxesis na min ksepernaei tous MAXNAME xaraktires.
		return (UT_errno = UTE_SCM_WRONGRELNAME);
	
	if ( (fd = HF_OpenFile(argv[1]) ) < 0)
		return (UT_errno = UTE_SCM_RELNOTEXISTS);
	
	if (HF_CloseFile(fd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
		
	if(strlen(argv[2]) > MAXNAME)			//-> Prepei to onoma tou pediou na min kseperanei tous MAXNAME xaraktires.
		return (UT_errno = UTE_SCM_LONGNAME);
	
	return UTE_OK;
}



/* SCM_destroy() Τα ορίσματά της είναι ίδια με της συνάρτησης UT_destroy. Επιστρέφει UTE_OK αν ο
 έλεγχος έγινε με επιτυχία ειδ' αλλοιώς κάποιων κωδικό σφάλματος. */

int SCM_destroy(int argc, char* argv[])
{
	int fd;
	
	if( argc != 2)		//-> Prepei to plithos twn orismatwn na einai 2 ("destroy",onoma sxesis).
		return (UT_errno = UTE_SCM_WRONGARGSDESTROY); 
		
	if(argv[2] != NULL)				//-> Prepei to plithos twn orismatwn na einai oso dilwnei i argc.
		return (UT_errno = UTE_SCM_WRONGNUMOFARGS); 
		
	if(strcmp(argv[0], "destroy"))		//-> Prepei to prwto orisma na einai to string "buildindex".
		return (UT_errno = UTE_SCM_WRONGFIRSTARG);
 	
	if(strlen(argv[1]) > MAXNAME)			//-> Prepei to onoma tis sxesis na min ksepernaei tous MAXNAME xaraktires.
		return (UT_errno = UTE_SCM_WRONGRELNAME);
	
											//-> Prepei to onoma tis sxesis na uparxei sti basi
	if ( (fd = HF_OpenFile(argv[1]) ) < 0)
		return (UT_errno = UTE_SCM_RELNOTEXISTS);
	
	if (HF_CloseFile(fd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	return UTE_OK;
}



/* SCM_print() Τα ορίσματά της είναι ίδια με της συνάρτησης UT_print. Επιστρέφει UTE_OK 
    αν ο έλεγχος έγινε με επιτυχία ειδ' αλλοιώς κάποιων κωδικό σφάλματος.*/ 

int SCM_print(char* relName)
{
	int fd;
	if (relName == NULL)
		return (UT_errno = UTE_SCM_NULLNAME);
	
	if(strlen(relName) > MAXNAME)			//-> Prepei to onoma tis sxesis na min ksepernaei tous MAXNAME xaraktires.
		return (UT_errno = UTE_SCM_WRONGRELNAME);
		
	if ( (fd = HF_OpenFile(relName) ) < 0)
		return (UT_errno = UTE_SCM_RELNOTEXISTS);
	
	if (HF_CloseFile(fd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
		
	return UTE_OK;
}


/*  SCM_insert() Τα ορίσματά της είναι ίδια με της συνάρτησης DM_insert. Επιστρέφει UTE_OK 
    αν ο έλεγχος έγινε με επιτυχία ειδ' αλλοιώς κάποιων κωδικό σφάλματος.*/

int SCM_insert(int argc, char* argv[])
{
	return UTE_OK;
}



/*  SCM_delete() Τα ορίσματά της είναι ίδια με της συνάρτησης DM_delete. Επιστρέφει UTE_OK 
	αν ο έλεγχος έγινε με επιτυχία ειδ' αλλοιώς κάποιων κωδικό σφάλματος. */

int SCM_delete(int argc, char* argv[])
{
	return UTE_OK;
}



/*  SCM_select() Τα ορίσματά της είναι ίδια με της συνάρτησης DM_select. Επιστρέφει UTE_OK 
	αν ο έλεγχος έγινε με επιτυχία ειδ' αλλοιώς κάποιων κωδικό σφάλματος. */

int SCM_select(int argc, char* argv[])
{
	return UTE_OK;
}

 
/*  SCM_join() Τα ορίσματά της είναι ίδια με της συνάρτησης DM_join. Επιστρέφει UTE_OK
 αν ο έλεγχος έγινε με επιτυχία ειδ' αλλοιώς κάποιων κωδικό σφάλματος.*/

int SCM_join(int argc, char* argv[])
{
	return UTE_OK;
}


