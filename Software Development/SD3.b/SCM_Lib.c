#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
		return (UT_errno = UTE_SCM_WRONGNAMELEN);	

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
		return (UT_errno = UTE_SCM_WRONGNAMELEN);
	
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
		return (UT_errno = UTE_SCM_WRONGNAMELEN);
	
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
		return (UT_errno = UTE_SCM_WRONGNAMELEN);
		
	if ( (fd = HF_OpenFile(relName) ) < 0)
		return (UT_errno = UTE_SCM_RELNOTEXISTS);
	
	if (HF_CloseFile(fd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);
		
	return UTE_OK;
}


/*  SCM_insert() Τα ορίσματά της είναι ίδια με της συνάρτησης DM_insert. Επιστρέφει UTE_OK 
    αν ο έλεγχος έγινε με επιτυχία ειδ' αλλοιώς κάποιων κωδικό σφάλματος.*/



int IsInteger(char *s)
{
	int i = 0;

	if((s[i] == '-' || s[i] == '+')  &&  s[i+1] != '\0')
		i ++;
	while(s[i] != '\0')
		if(!isdigit(s[i++]))
			return 0;
	return 1;
}

int IsFloat(char *s)
{
	int i, dot,digits;

	i = 0;
	dot = 0;
	digits = 0;

	if((s[i] == '-' || s[i] == '+')  &&  s[i+1] != '\0')
		i ++;
	while(s[i] != '\0') {
		if(!isdigit(s[i])) {
			if(!dot  &&  s[i] == '.')
				dot ++;
			else
				return 0;
		}
		else
			digits ++;
	}

	if(digits)
		return 1;
	else
		return 0;
}


int SCM2_insert(int argc, char* argv[])
{
	int i, j, fd, h, firstdot;
	relDesc rel;
	attrDesc *attrs;


    
	if( argv[argc] != NULL)		//-> Prepei to plithos twn orismatwn na einai oso dilwnei i argc.
		return (UT_errno = UTE_SCM_WRONGNUMOFARGS);
		
	if( strcmp(argv[0], "insert"))		//-> Prepei to prwto orisma na einai to string "insert".
		return (UT_errno = UTE_SCM_WRONGFIRSTARG);	
	
	if( strlen(argv[1]) > MAXNAME) 		//-> Prepei to onoma tis sxesis na min ksepernaei tous MAXNAME xaraktires.
		return (UT_errno = UTE_SCM_WRONGNAMELEN);	

	//elegxos ean i sxesi/arxeio 'argv[1]' uparxei idi. Ean oxi, epistrefei error

	if ( (fd = HF_OpenFile(argv[1]) ) < 0)
		return (UT_errno = UTE_SCM_RELNOTEXISTS);
	else if (HF_CloseFile(fd) != HFE_OK)
		return (UT_errno = UTE_HFERROR);

	if (CFM_Lookup(argv[1], &rel, &attrs) != UTE_OK)
		return UT_errno;
	
	if (rel.attrcnt != (argc - 2)/2)	/*gia kathe pedio exw 2 orismata ara an den dothikan gia ola ta attrcnt*/
		return (UT_errno = UTE_WRONGATTRNUMS);


	for(i=2 ; i<argc ; i+=2) 
	{							//-> Gia kathe onoma pediou pou dothike san orisma,
		if( argv[i] == NULL)
			return( UT_errno = UTE_SCM_NULLNAME);
								//-> Elegxei kat'arxas ean exei dothei NULL adi gia onoma kai format pediou.	
		if( argv[i+1] == NULL)
			return (UT_errno  = UTE_SCM_NULLVALUE);

		for (j = 0; j < rel.attrcnt; j++) {
			if ( strcmp(argv[i], attrs[j].attrname) == 0) {
				if (attrs[j].attrtype == 'i') {
					if(!IsInteger(argv[i+1]))
						return (UT_errno = UTE_WRONGINTVAL);
				}
				else if (attrs[j].attrtype == 'f') {
					if(!IsFloat(argv[i+1]))
						return (UT_errno = UTE_WRONGFLOATVAL);
				}
				else if (attrs[j].attrlength < strlen(argv[i+1]))
						return (UT_errno = UTE_WRONGCVALLEN);
				break;
			}
		}
		if (j == rel.attrcnt)
			return (UT_errno = UTE_ATTRNOTVALID);
	}

	return UTE_OK;
}



/*  SCM_delete() Τα ορίσματά της είναι ίδια με της συνάρτησης DM_delete. Επιστρέφει UTE_OK 
	αν ο έλεγχος έγινε με επιτυχία ειδ' αλλοιώς κάποιων κωδικό σφάλματος. */

int SCM2_delete(int argc, char* argv[])
{
	return UTE_OK;
}










//----------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//



/*  SCM_select() Τα ορίσματά της είναι ίδια με της συνάρτησης DM_select. Επιστρέφει UTE_OK 
	αν ο έλεγχος έγινε με επιτυχία ειδ' αλλοιώς κάποιων κωδικό σφάλματος. */

//O idios o parser elegxei ean yparxei zefksi 2 sxesewn sto pithano "where ...", opote ean klithei i
//'DM_select' apton parser, den yparxei periptwsi na yparxei zefksi sxesewn. Opote stin "target list"
//kai sto pithano "where condition", tha prepei yparxei upoxrewtika MONO ena onoma sxesis.
//Ean vrei parapanw apo ena diaforetiko onoma sxesis, einai lathos.

int SCM2_select(int argc, char *argv[])
{
	int i, j, i_start, i_end, fd, attrsnum;


	if(argv[argc] != NULL)								//-> Prepei to plithos twn orismatwn na einai oso dilwnei to argc.
		return (UT_errno = UTE_SCM_WRONGNUMOFARGS);
	if(strcmp(argv[0], "select"))						//-> Prepei to prwto orisma na einai to string "select".
		return (UT_errno = UTE_SCM_WRONGFIRSTARG);	

	//-> Ean i timi tou argc einai artios arithmos tote simainei oti den yparxei sta orismata to proairetiko orisma
	//	 "onoma sxesis apotelesmatos". Alliws ean einai perittos, tote yparxei.
	//	 Twra, ean yparxei to proairetiko auto orisma, tote to orisma "plithos orismatwn ta opoia provallontai"
	//	 einai to 3o orisma dld to argv[2]. Alliws einai to 2o orisma dld to argv[1].
	if(argc%2 == 0)
		i_start = 1;
	else {
		if((fd = HF_OpenFile(argv[1])) >= 0) {			//-> Elegxei ean to arxeio tou 'onomatos sxesis apotelesmatos' yparxei idi.
			if(HF_CloseFile(fd) != HFE_OK)				//	 Ean nai, kleinei to arxeio, kai epistrefei katallilo error.
				return (UT_errno = UTE_HFERROR);
			return (UT_errno = UTE_SCM_RELEXISTS);
		}
		i_start = 2;
	}

	for(i=0 ; argv[i_start][i] != '\0' ; i++)			//-> Elegxei prwta ean oloi oi xaraktires tou orismatos adistoixoun se
		if(! isdigit(argv[i_start][i]))					//	 psifia, dioti me sketi atoi (pou ginetai epeita) den elegxetai
			return (UT_errno = UTE_SCM_WRONGINTVAL);	//	 auti i periptwsi. px, i 'atoi("123babis")' epistrefei '123'.
	if((attrsnum = atoi(argv[i_start])) == 0)			//-> Ean to plithos twn orismatwn pou provallontai einai miden,
		return (UT_errno = UTE_SCM_EMPTYTARGETLIST);	//	 epistrefei error.


	i_start ++;			//-> Pleon deixnei sto prwto onoma provallomenis sxesis.

	//-> Sto parakatw 'if' elegxei ean yparxoun ta 4 proairetika teleutaia orismata.
	//	 attrsnum*2 : to plithos twn orismatwn twn zevgwn 'onoma provallomenis sxeis - onoma provallomenou pediou'
	//	 +2			: ta 2 prwta orismata "select" kai to 'plithos orismatwn ta opoia provallontai'.
	//	 +4			: ta 4 teleutaia proairetika orismata.
	//	 To proairetiko orisma 'onoma sxesis apotelesmatos' den paizei rolo ston parakatw elegxo, to ean yparxei i oxi.
	if(attrsnum*2+6 <= argc) {					//-> Ean isxuei, tote yparxoun ta 4 teleutaia proairetika orismata.
		optionalFour = 1;
		i_end = i_start + attrsnum*2;			//-> Deixnei mexri kai to 'onoma sxesis epilogis'.
	}
	else {
		optionalFour = 0;
		i_end = i_start + attrsnum*2 - 2;		//-> Deixnei mexri kai to teleutaio 'onoma provallomenis sxesis'.
	}


	//-> Tha prepei ola ta onomata sxesewn se ola ta orismata 'onoma provallomenis sxesis' kai sto pithano orisma
	//	 'onoma sxesis epilogis' na einai idia. Ean yparksei estw kai ena diaforetiko onoma sxesis se sxesi me
	//	 ta upoloipa, tote epistrefei error.
	for(i=i_start ; i<=i_end ; i+=2)
		if(strcmp(argv[i_start], argv[i]))
			return (UT_errno = UTE_SCM_ERROR); //*********************************************

	if(CFM_Lookup(argv[i_start], &rel, &attrs) != UTE_OK)
		return UT_errno;

	//-> Gia kathe orisma 'onoma provallomenou pediou' i kai 'onoma pediou epilogis', elegxei to onoma tou pediou
	//	 autou, ontws yparxei san pedio tis sxeis, mesw twn pinakwn 'relCat' kai 'attrCat'.
	for(i=i_start+1 ; i<=i_end+1 ; i+=2) {
		for(j=0 ; j<rel.attrcnt ; j++)
			if(!strcmp(argv[i], attrs[j].attrname))
				break;
		if(j == rel.attrcnt)					//-> Ean den vrethike auto to onoma auto san onoma pediou tis sxesis, epistrefei error.
			return (UT_errno = UTE_SCM_ERROR); //*****************************************

		if(!optionalFour  ||  i < i_end+1)		//-> Gia ola ta orismata onomatwn pediou, ektos apto pithano 'onoma pediou epilogis'.
			for(j=i-2 ; j>=i_start+1 ; j-=2)	//	 elegxei ean vrei 2 fores to idio onoma pediou. Automatws, etsi ginetai elegxos
				if(!strcmp(argv[i], argv[j]))	//	 ean kai to 'plithos provallomenwn orismatwn' einai > apo rel.attrcnt
					return (UT_errno = UTE_SCM_ERROR);	//*******************************************
	}


	if(optionalFour) {							//-> Ean dothike kai sunthiki "where",
		if(	strcmp(argv[argc-2], "<")	&&		//	 elegxei ean o telestis pou dothike einai egguros.
			strcmp(argv[argc-2], "<=")	&&
			strcmp(argv[argc-2], ">")	&&
			strcmp(argv[argc-2], ">=")	&&
			strcmp(argv[argc-2], "=")	&&
			strcmp(argv[argc-2], "!="))
			return (UT_errno = UTE_SCM_ERROR);

		for(i=0 ; i<rel.attrcnt ; i++)						//-> Elegxei ean stin sunthiki tou "where" o typos tou pediou
			if(!strcmp(argv[argc-3], attrs[i].attrname)) {	//	 pou sugrinetai sumvadizei me ton typo tis timis tis sugrisis.
				if(attrs[i].attrtype == 'i'  &&  IsInteger(argv[argc-1])	//-> Ean einai integer.
					return (UT_errno = UTE_SCM_WRONGINTVAL);
				else if(attrs[i].attrtype == 'f'  &&  IsFloat(argv[argc-1])	//-> Ean einai float.
					return (UT_errno = UTE_SCM_WRONGFLOATVAL);
				//else if is string, then its ok anyway.
				break;
			}
	}

	return UTE_OK;
}






/*  SCM_join() Τα ορίσματά της είναι ίδια με της συνάρτησης DM_join. Επιστρέφει UTE_OK
 αν ο έλεγχος έγινε με επιτυχία ειδ' αλλοιώς κάποιων κωδικό σφάλματος.*/

int SCM2_join(int argc, char *argv[])
{
	int i, j, i_start, i_end, fd, attrsnum;


	if(argv[argc] != NULL)								//-> Prepei to plithos twn orismatwn na einai oso dilwnei to argc.
		return (UT_errno = UTE_SCM_WRONGNUMOFARGS);
	if(strcmp(argv[0], "join"))							//-> Prepei to prwto orisma na einai to string "select".
		return (UT_errno = UTE_SCM_WRONGFIRSTARG);

	//-> Ean i timi tou argc einai artios arithmos tote simainei oti den yparxei sta orismata to proairetiko orisma
	//	 "onoma sxesis apotelesmatos". Alliws ean einai perittos, tote yparxei.
	//	 Twra, ean yparxei to proairetiko auto orisma, tote to orisma "plithos orismatwn ta opoia provallontai"
	//	 einai to 3o orisma dld to argv[2]. Alliws einai to 2o orisma dld to argv[1].
	if(argc%2 == 0)
		i_start = 1;
	else {
		if((fd = HF_OpenFile(argv[1])) >= 0) {			//-> Elegxei ean to arxeio tou 'onomatos sxesis apotelesmatos' yparxei idi.
			if(HF_CloseFile(fd) != HFE_OK)				//	 Ean nai, kleinei to arxeio, kai epistrefei katallilo error.
				return (UT_errno = UTE_HFERROR);
			return (UT_errno = UTE_SCM_RELEXISTS);
		}
		i_start = 2;
	}

	for(i=0 ; argv[i_start][i] != '\0' ; i++)			//-> Elegxei prwta ean oloi oi xaraktires tou orismatos adistoixoun se
		if(! isdigit(argv[i_start][i]))					//	 psifia, dioti me sketi atoi (pou ginetai epeita) den elegxetai
			return (UT_errno = UTE_SCM_WRONGINTVAL);	//	 auti i periptwsi. px, i 'atoi("123babis")' epistrefei '123'.

	i_start ++;											//-> Pleon deixnei sto prwto onoma provallomenis sxesis.




	if((fd = HF_OpenFile(argv[argc-5])) < 0)					//-> Elegxei ean yparxei i prwti sxesi tis sunthikis "where".
		return (UT_errno = UTE_SCM_RELNOTEXISTS);				//	 Ean oxi, epistrefei error.
	else {
		if(HF_CloseFile(fd) != HFE_OK)
			return (UT_errno = UTE_HFERROR);
		if(CFM_Lookup(argv[argc-5], &rel1, &attrs1) != UTE_OK)	//-> Diavazei apton 'attrCat' tis plirofories gia ta pedia tis sxesis.
			return UT_errno;

		for(i=0 ; i<rel1.attrcnt ; i++)							//-> Elegxei ean to pedio tis sxesis einai eguro (uparkto).
			if(!strcmp(argv[argc-4], attrs1[i].attrname))
				break;
		if(i == rel1.attrcnt)
/*******/	return (UT_errno = UTE_SCM_ERROR);
	}

	if(strcmp(argv[argc-5], argv[argc-2]) {							//-> Ean i 2i sxesi tis sunthikis einai diaforetiki apo tin 1i,
		if((fd = HF_OpenFile(argv[argc-2])) < 0)					//-> Elegxei ean yparxei i prwti sxesi tis sunthikis "where".
			return (UT_errno = UTE_SCM_RELNOTEXISTS);				//	 Ean oxi, epistrefei error.
		else {
			if(HF_CloseFile(fd) != HFE_OK)
				return (UT_errno = UTE_HFERROR);
			if(CFM_Lookup(argv[argc-2], &rel2, &attrs2) != UTE_OK)	//-> Diavazei apton 'attrCat' tis plirofories gia ta pedia tis sxesis.
				return UT_errno;

			for(j=0 ; j<rel2.attrcnt ; j++)							//-> Elegxei ean to pedio tis sxesis einai eguro (uparkto).
			if(!strcmp(argv[argc-1], attrs2[j].attrname))
				break;
			if(j == rel2.attrcnt)
/*******/		return (UT_errno = UTE_SCM_ERROR);
		}
	}
	else {															//-> Ean i 2i sxesi tis sunthikis einai idia me tin 1i,
		for(j=0 ; j<rel1.attrcnt ; j++)								//-> Elegxei ean to pedio tis sxesis einai eguro (uparkto).
			if(!strcmp(argv[argc-1], attrs1[j].attrname))
				break;
		if(j == rel1.attrcnt)
/*******/	return (UT_errno = UTE_SCM_ERROR);
	}


	if(attrs[i].attrtype != attrs[j].attrtype)						//-> Tha prepei ta duo pedia twn sxesewn tis sunthikis "where"
/*******/	return (UT_errno = UTE_SCM_ERROR);						//	 na einai idiou tupou.





	for(i=i_start ; i<=argc-7 ; i+=2) {
		if(strcmp(argv[i], argv[argc-5]) && strcmp(argv[i],argv[argc-2]))	//-> Prepei oi sxeseis twn provallomenwn pediwn
/*********/	return (UT_errno = UTE_SCM_ERROR);								//	 na vriskontai stin sunthiki "where".

		if(!strcmp(argv[i], argv[argc-5])) {								//-> I provallomeni sxesi einai i idia me tin prwti
			for(j=0 ; j<rel1.attrcnt ; j++)									//	 sxesi pou sugrinetai sti sunthiki "where". //->
				if(!strcmp(argv[i+1], attrs1[j].attrname))					//	 Elegxei ean to pedio tis provallomenis sxesis 
					break;													//	 einai eguro (uparkto) pedio tis sxesis.
			if(j == rel1.attrcnt)
/*********/		return (UT_errno = UTE_SCM_ERROR);
		}
		else if(!strcmp(argv[i], argv[argc-2])) {							//-> I provallomeni sxesi einai i idia me ti defteri
			for(j=0 ; j<rel2.attrcnt ; j++)									//	 sxesi pou sugrinetai sti sunthiki "where". //->
				if(!strcmp(argv[i+1], attrs2[j].attrname))					//	 Elegxei ean to pedio tis provallomenis sxesis
					break;													//	 einai eguro (uparkto) pedio tis sxesis.
			if(j == rel2.attrcnt)
/*********/		return (UT_errno = UTE_SCM_ERROR);
		}
		else	//I provallomeni sxesi den vrisketai stin sunthiki "where".
/*********/		return (UT_errno = UTE_SCM_ERROR);

		for(j=i-2 ; j>=i_start ; j-=2)											//-> Prepei o ekastote sunduasmos provallomenis
			if(!strcmp(argv[i], argv[j])  &&  !strcmp(argv[i+1], argv[j+1]))	//	 sxesis-pediou na min exei ksanayparksei stin
/*********/		return (UT_errno = UTE_SCM_ERROR);								//	 target list.
	}



	if(	strcmp(argv[argc-3], "<")	&&							//-> Elegxei ean o telestis pou dothike einai egguros.
		strcmp(argv[argc-3], "<=")	&&
		strcmp(argv[argc-3], ">")	&&
		strcmp(argv[argc-3], ">=")	&&
		strcmp(argv[argc-3], "=")	&&
		strcmp(argv[argc-3], "!="))
		return (UT_errno = UTE_SCM_ERROR);


	return UTE_OK;
}




















