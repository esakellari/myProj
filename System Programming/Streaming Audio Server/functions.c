#include "functions.h"

//synartisi pou anixneyei teleia mesa sto string
char *song_search_halding(char *s , char *mytoken)
{
	int i,j=0,stop=0;
         

       i = 16;

      while ( (i<strlen(s)) && (stop==0))
      {
 	    if (s[i]!= '.')
	    {
		mytoken[j]=s[i];
                i++;
                j++;
	    }
	    else 
		stop=1;
	} 

       /* mytoken[j]=' ';
        i++;*/
	mytoken[j]='\0';

	return(mytoken);
}

//synartisi pou prospernaei ta kena se ena string
char *song_streaming_halding(char *s , char *mytoken)
{
	int i,j=0,stop=0;
         

       i = 5;
      while ( (i<strlen(s)) && (stop==0))
      {
 	    if (s[i]!= ' ')
	    {
		mytoken[j]=s[i];
                i++;
                j++;
	    }
	    else 
		stop=1;
	} 

       /* mytoken[j]=' ';
        i++;*/
	mytoken[j]='\0';

	return(mytoken);
}

//synartisi pou xrhsimopoieitai gia na vrei to host
char *get_host(char *s , char *mytoken)
{
	int i,j=0,stop=0;
         

       i = 6;
      while ( (i<strlen(s)) && (stop==0))
      {
 	    if (s[i]!= '\r')
	    {
		mytoken[j]=s[i];
                i++;
                j++;
	    }
	    else 
		stop=1;
	} 

       /* mytoken[j]=' ';
        i++;*/
	mytoken[j]='\0';

	return(mytoken);
}


char *eidikos_xeirismos(char *s , char *mytoken)
{

	int i=0,j=0,stop=0;
        
	//apothikeyoume ta stoixeia tou s sto mytoken 
	for(i=0; i<strlen(s); i++)
    	 mytoken[i]=s[i];
 
	//an to proteleytaio stoixeio einai to "/" , vazoume sto teleytaio stoixeio "/"
        if (mytoken[i-1]!='/')
         {
              mytoken[i]='/';
	       i++;
         }

	mytoken[i]='.';
        i++;
        mytoken[i]='\0';

	return(mytoken);
}

//synartisi pou afairei tin allagi grammis apo to telos enos string
char *strip_from_nl(char *s , char *mytoken)
{
	int i=0,j=0,stop=0;
         


     for(i=0; i<strlen(s)-1; i++)
      {
		mytoken[i]=s[i];
	} 

        i= strlen(s)-1;
        if (s[i]=='\n')
        {
	   mytoken[i]='\0';
          
        }
        else
         {
           mytoken[i] = s[i];
           i++;
           mytoken[i] = s[i];
	  
        }
          return(mytoken);

	
}

//synartisi pou prosthetei sto telos tin allagi grammis
char *enter_lat_nl(char *s , char *mytoken)
{

	int	i=0,j=0,stop=0,pos1=0,pos2=0;
         
     	for(i=0; i<strlen(s); i++)
      	mytoken[i]=s[i];
	 
        
	 mytoken[i]='\n';
         i++;
         mytoken[i]='\0';
          
   	return(mytoken);
}

//synartisi pou diwxnei to teleytaio keno
char  *strip_from_last_keno(char *s , char *mytoken)
{
	int i=0,j=0,stop=0,pos1=0,pos2=0;
         
	for(i=0; i<strlen(s)-1; i++)
      	mytoken[i]=s[i];
	 

        i= strlen(s)-1;

        if (s[i]==' ')
        	mytoken[i]='\0';
          
   	else
        {
       		mytoken[i] = s[i];
           	i++;
           	mytoken[i] = s[i];
	}
         
	 return(mytoken);

	
}

//synartisi pou vriskei diafores anamesa se dyo string
void diafores(char *s , char *mytoken)
{
	int i=0,j=0,stop=0,pos1=0,pos2=0;
         


     	for(i=0; i<strlen(s)+1; i++)
      	{
 	    if (s[i]!= mytoken[i])
		printf("i diaora einai edw\n");    
	     
	} 

        

	
}

//ayti i synartisi xrhsimopoieitai gia na elegksoume ama exei to string kataliksi .m3u,.mp3 klp..
int exei_teleia(char *s)
{
	int i=0,j=0,brika=0;
         
	for(i=0; i<strlen(s)-1; i++)
      	{
 	    if (s[i]== '.')
		brika=1;
	    else if (s[i]== '/') 
		brika=0;
	}
 
      return brika;
}

//vriskei to monopati gia ena fakelo
char * get_folder_path (char *s,char * mytoken)
{
	int i=0,j=0,stop=0,pos1=0;
         
	for(i=0; i<strlen(s); i++)
      	{
 	    if ((s[i]=='/')&&(i<strlen(s)-2))
		    pos1=i;	    
	} 
	
	for(i=0; i<=pos1; i++)
      	{
 	     mytoken[i]= s[i];
	} 
	
        mytoken[i]='\0';

	return(mytoken);
}

//synartisi pou xreiazetai sto xeirismo tis periptwsis pou anoigoume me media player
char * add_nes (char *s,char * mytoken,int x)
{
	int i=0,j=0,stop=0,pos1=0,pos2=0;
         
	for(i=0; i<strlen(s); i++)
      	{
 	   if ((i==x)&&(( s[1+1]!= '.')||(s[i+2]!='/')))
	   {
         	mytoken[j]= s[i];
                j++;
                mytoken[j]= '.';
                j++;
                mytoken[j]= '/';
                j++;
           }
           else
           {      
		mytoken[j]=s[i];
                j++;
           }
        }
               
	
        mytoken[j]='\0';

	return(mytoken);
}
