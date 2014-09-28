#include "functions.h"
#include "thread_source.h"

//synartisi pou pairnei to thread kai kanei oli tin douleia

void *main_funct(FILE* sock_fp)
{
	struct sockaddr_in saddr;
	int sock_listen, sock,loom,portnum,prwto,one = 1,songsearch,i,status,stop;           
    	FILE *pipe_fp;          
	FILE * pFile;
        FILE *fp;
	char dirname[BUFSIZ],c,*header[20],*pch,*pch2,anazitoumeno[256],* new_args[4];    
    	char command[BUFSIZ],temp[256],temp2[512],host[256],mystring[LEN],buffer[BUFSIZ],blabla[LEN],blabla2[LEN];    

	char temp3[LEN];
       char temp4[LEN];
       char anazitoumeno2[LEN];
       char *pch3;
       char * pch4;

	long j,m;
      
       int k =0;
	
	
	printf("I am the newly created thread %ld\n", pthread_self());  
	
      loom =0;
     

	//pairnoume ayta pou exoun graftei sto socket kai ta apothikeyoume sto header
      do {
		  
        	fgets(dirname, BUFSIZ - 5, sock_fp);
          	puts(dirname);

         	header[loom]=NULL;
	  	header[loom]=(char *)malloc(strlen(dirname)+1);
	  	strcpy(header[loom],dirname);
         	loom++;

		

          } while ((dirname[0]!='\r')&& (dirname[1]!='\n'));
       
	//apomonwnetai to host
         strcpy(host,get_host(header[1] , host));
         printf("Host: %s\n",host);

	//an psaxnoume gia lista tragoudiwn
         pch = strstr (header[0],"GET /songsearch/");
        
	 if (pch!=NULL)
         {
		songsearch=1;
                puts("SONG SEARCH");
          
		//ksexwrizoume tin leksi pou psaxnoume	      
		strcpy(temp,song_search_halding(header[0] , temp));
              
		url_decode(temp, anazitoumeno, 256, 0);
		
		printf("String to be searched : %s\n",anazitoumeno);
                
		fp = fopen ("songslisting","r");
             
      		stop=0;

		//diavazontai ta periexomena tou arxeiou songlisting
		while ((fgets (mystring , LEN , fp) != NULL) && (stop==0))
		{
     	           strcpy(temp,strip_from_nl(mystring,temp));
		   pch = strstr (temp,anazitoumeno);
     	          
		   if (pch!=NULL)
                   {
     	              /*puts(temp);*/
                      stop=1;
                   }  
      		}

	        fclose(fp);
		
                if (stop==0)
                   fprintf(sock_fp, "HTTP/1.0 404 OK\r\n");
                else 
                  fprintf(sock_fp, "HTTP/1.0 200 OK\r\n");
               
		fprintf(sock_fp, "Server: musicstreamer v1.0\r\n"); 
               	fprintf(sock_fp, "Connection: close\r\n"); 
               	fprintf(sock_fp, "Content-Type: audio/x-mpegurl\r\n"); 
               	fprintf(sock_fp, "\r\n"); 

                fp = fopen ("songslisting","r");
		

		//psaxnoume se kathe mia grammi to anazitoumeno kai molis vrethei 
		//ginetai encode kai grafetai sto socket
		while (fgets (mystring , LEN , fp) != NULL)
		{
              	  	strcpy(temp,strip_from_nl(mystring,temp));
			pch = strstr (temp,anazitoumeno);
 
              		if (pch!=NULL)
                        {
                             url_encode(temp, temp2, 512, 0);
                 	     fprintf(sock_fp, "http://%s/%s\n",host,temp2); 
                           
                          
                         }
       		}

  	        fclose(fp);
              
                fclose(sock_fp);
                
                puts(" ");  
        
	}/* if (pch!=NULL)*/

	//an psaxnoume ena tragoudi kai oxi lista 
        else
        {
		

                puts("STREAMING");

                //vazoume sto temp3 to url tou tragoudiou
		strcpy(temp3,song_streaming_halding(header[0] , temp3));

              
              /*  memset (anazitoumeno2,'\0', LEN);*/

		//ginetai decode to url kai mpainei sto temp3
		url_decode(temp3, anazitoumeno2, 256, 0);

                puts(anazitoumeno2);
        
		//vriskoume to fakelo pou vrisketai akrivws 
                strcpy(temp4,get_folder_path (music_folder,temp4));
                puts(temp4);

		//vlepoume ama to tragoudi yparxei se ayton ton fakelo
                pch3 = strstr (anazitoumeno2,temp4);
                
		//to header tou media player
		pch4 = strstr (header[2],"NSPlayer");
        //        printf("to anazitoumeno einai %s me %d\n",anazitoumeno2,strlen(anazitoumeno2));
	
		//xeirismos tis periptwsis pou anoiggoume me media player 
                if ((pch3==NULL)&&(pch4!=NULL))
                {
                               strcat(temp4,anazitoumeno2);
                               strcpy(anazitoumeno2,temp4);
                }
              
		if (pch4!=NULL)
		strcpy(anazitoumeno2,add_nes (anazitoumeno2,temp4,strlen(music_folder)- 2));
			
		pFile = fopen ("songslisting","r");
		stop=0;

              	//diavazontai oi grammes apo to arxeio
		while ((fgets (mystring , LEN , pFile) != NULL) && (stop==0))
		{
     	        	//vgainei i allagi grammis apo o telos tis grammis(\n) kai to 
     	        	//aptelesma apothikeyetai sto temp3  
		 	strcpy(temp3,strip_from_nl(mystring,temp3));
                   	pch2=NULL;
			//psaxnoujme to anazhtoumeno sto temp3 kai to vazoume sto pch2
		   	pch2 = strstr (temp3,anazitoumeno2);
     	           
			if (pch2!=NULL)
              		stop=1;
		} 

		fclose(pFile);
                  
                pFile = NULL;

                if (stop==0)
                   fprintf(sock_fp, "HTTP/1.0 404 OK\r\n");
                
		else 
                   fprintf(sock_fp, "HTTP/1.0 200 OK\r\n"); 
	       
		fprintf(sock_fp, "Server: musicstreamer v1.0\r\n"); 
               	fprintf(sock_fp, "Connection: close\r\n"); 
               	fprintf(sock_fp, "Content-Type: audio/mpeg\r\n"); 
               	fprintf(sock_fp, "\r\n"); 
            
		//printf("stop =%d\n", stop);

               	if (stop==1)
		{
                      	//puts("mpika sto stop");
		
			FILE * pFile = fopen(anazitoumeno2,"rb");
 
                       
			if(pFile == NULL)
			perror("fopen:");
			
                	//adeiazoume ton buffer
		     	memset (buffer, '\0', BUFSIZE);
			
                      	fread (buffer,1, BUFSIZE, pFile );
                       	puts("Started reading and sending");
                        
			
			  i=0;

			//printf("will read and write %ld blocks\n", m);
                      	
			//diavazoume synexeia apo to arxeio kai grafoume sto socket 
			//mexri na teleiwsei i metafora 
			do
			{
		    		i = fwrite ( buffer, 1, BUFSIZE, sock_fp);
                              
				memset (buffer, '\0',BUFSIZE);
                                k=i;
                              
               		       i = fread ( buffer,1, BUFSIZE, pFile );
                            	
                              
			} while ((i==BUFSIZE)&&(k==BUFSIZE)); 

                	printf("Read and wrote %ld bytes\n", j);
 
		        puts("Finished sending (Etheir completed or stopped)");
	                 fclose(pFile);
                  
			fclose(sock_fp);
              }/*if (stop==1)*/

       }/*else*/           
      
             
     //pthread_exit(NULL);
     
}




