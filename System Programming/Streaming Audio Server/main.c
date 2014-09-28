#include "functions.h"
#include "thread_source.h"

int main(int argc, char *argv[])
{

	struct sockaddr_in saddr;
    	int sock_listen, sock,loom =0,portnum,prwto;    
    	FILE *sock_fp;         
    	FILE *pipe_fp;           
    	char dirname[BUFSIZ],c,*header[20],*pch,*pch2,anazitoumeno[256],* new_args[4];   
    	char command[BUFSIZ],temp[256],temp2[512],host[256],mystring[LEN],buffer[BUFSIZ],blabla[LEN],blabla2[LEN];
    	int one = 1,songsearch,i,status,stop;
    	pid_t	cpid;
    	
	FILE * pFile;
    	FILE *fp;
        pthread_t t;

	
	if (argc!=5) /* ama o xrhsths den dosei ta apaitoumena orismata*/
  	{
      	   	printf("Wrong number of arguments, program cannot run\n");
                exit(1);
   	}

	else /* ama exei dwsei ta aparaithta orismata */
	{
      		
     		c = 1;
      		while (c<argc) /* apothikeusi twn orismatwn se metablites */
      		{
           	 	strcpy (temp,argv[c]);

			//apothikeyetai i port
             		if (strcmp(temp,"-p")==0)
             		{
                  		c++;
                  		portnum = atoi (argv[c]);  
                  		c++;
             		}
			//apothikeyetai o fakelos ston opoio vriskontai ta tragoudia 
             		else  if   (strcmp(temp,"-d")==0)                
             		{
                  	
				c++;
                  		strcpy(music_folder,argv[c]); 
                  		c++;
             		}
             	
                  }
        }     
        

      
          strcpy(music_folder,eidikos_xeirismos(music_folder, temp));
         /* puts (music_folder);*/

		//dimiorgeitai mia diergasia pou psaxnei gia ton fakelo ston opoio vriskontai ta tragoudia 
		//pou theloume, kai ta periexomena apothikeyontai sto fakelo songslisting_temp
          switch( cpid = fork() )
	{

			case 0:
                             new_args[0]=NULL;
		             new_args[0]=(char *)malloc(strlen("find")+1);
		             strcpy(new_args[0],"find");

                             new_args[1]=NULL;
		             new_args[1]=(char *)malloc(strlen(music_folder)+1);
		             strcpy(new_args[1],music_folder);

                             new_args[2]=NULL;

			     fp=fopen("songslisting_temp","w+"); 

                             dup2(fileno(fp),STDOUT_FILENO);

                             execvp(new_args[0],new_args);
			     perror("exec failure");
		  	     exit(1);

                       default:
                            cpid= waitpid(cpid,&status,0);
                           
        }
           
        fp = fopen ("songslisting_temp","r");
        pFile = fopen ("songslisting","w+");

	
        strcpy(temp,enter_lat_nl(music_folder,temp));

	//antigrafontai ta periexomena tou songslisting_temp sto songslisting
        while (fgets (mystring , LEN , fp) != NULL)
        {
              if (strcmp(mystring,temp)!=0)
              {

		//psaxnei ta arxeia me kataliksi .m3u, .mp3, klp kai ta avazei sto neo arxeio 
                i = exei_teleia(mystring);
              	if (i==1)
               	fprintf(pFile,"%s",mystring);
             }

        }

        fclose(pFile);
	fclose(fp);

        if( remove( "songslisting_temp" ) != 0 )
           perror( "Error deleting file" );


    


    	//dimiourgia kai syndesi tou socket
    	if ((sock_listen = socket(PF_INET, SOCK_STREAM, 0)) == -1)    
        oops("socket");
	
    	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	saddr.sin_port = htons(portnum);    
    	saddr.sin_family = AF_INET;    
    
	if (bind(sock_listen, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)
        oops("bind");

    	
    	if (listen(sock_listen, 1) != 0)
        oops("listen");

    	puts("Server Running and awaiting connections");
    
	while (1) {
			int c;
        		
			if((sock = accept(sock_listen, NULL, NULL)) < 0)   
            		oops("accept");
        		
			
        		if ((sock_fp = fdopen(sock, "rb+")) == NULL)
            		oops("fdopen reading"); 
    
		    	//dimiourgia tou thread gia na mporoun polloi xrhstes taytoxronoa na akoune tragoudia
			pthread_create(&t, NULL, main_funct, sock_fp);
		}
 
}

