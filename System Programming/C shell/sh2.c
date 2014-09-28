#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fnmatch.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define READ 0
#define WRITE 1
#define APPEND 1
#define NOT_APPEND 0

void history();

struct arg{

	char *argument[100];

}arg;

//ayti i domi xreiazetai gia na krataei to history
struct hist{

	char his[10];
	int c;
	char inp[256];
}hist;

typedef struct arg arg2;
typedef struct hist hist2;

pid_t pid_to_kill;


//synartisi pou xeirizetai ta signals pou dino0ntai sto shell
void sig_handle(int signo)
{
	switch (signo)
	{
		case SIGINT:
		
			puts("KILLED CHILD");
			printf("pid killed = %d\n", pid_to_kill);
			kill(pid_to_kill,SIGKILL);

			break;

		case SIGTSTP:
			setpgid(pid_to_kill,pid_to_kill);
			kill(pid_to_kill,SIGSTOP);	
			break;

		case SIGCHLD: 
			break;
	}	
}




//kleinw ta pipes pou prepei na kleisoun epeidi de ta xreiazomai pleon
void closeUselessPipe(int fd[][2],int prog,int count)
{	
	int i;

	if (prog==0)
	{
		close(fd[0][0]);
		for(i = 1; i< count; i++)
		{
			close(fd[i][0]);
			close(fd[i][1]);
		}

	}
	else if( prog == count)
	{
		close(fd[count-1][1]);
	
		for(i=0 ; i < count-1 ; i++)
		{
			close(fd[i][0]);
			close(fd[i][1]);
		}

	}
	else
	{
		close(fd[prog-1][1]);
		close(fd[prog][0]);
		for( i=0 ; i < prog-1;i++)
		{
			close(fd[i][0]);
			close(fd[i][1]);
		}
		for(i=prog+1; i < count ;i++)
		{
			close(fd[i][0]);
			close(fd[i][1]);
		}
	}
}


char* argument(char* arg, int *pos, char* argum)
{
	int i;


	i = *pos;

	while((i < strlen(arg)) && (arg[i] != ' '))
	{
		argum[i-(*pos)]=arg[i];
		i++;
	}
	argum[i- (*pos)]='\0';

	*pos = i+1;

	return argum;

}

//ayti i synartisi diaxwrizei ta arguments pou dinontai sti grammi entolwn kai ta ta vazei sto args
//me ti voitheia tis synartisi argument i opoia kanei to diaxwrismo
int param( char* s, char** args)
{

	int i, pos;
	char  argum[100];

	i =0;
	pos = 0;

	strcpy(argum, argument(s,&i,argum));
	args[pos]=NULL;
        args[pos] = malloc(strlen(argum)+1);
        strcpy(args[pos], argum);
	
        pos++;

	while(i <= strlen(s))
	{
		 strcpy(argum, argument(s,&i,argum));
	
		args[pos]=NULL;
		args[pos] = malloc(strlen(argum)+1);
		strcpy(args[pos], argum);
		
		if (i>strlen(s)) 
		break;
		
		pos++;
		
	}

	args[pos]= NULL;
	return pos;
	

}

//synartisi pou psaxnei ta arxeia otan dinontai wildchars
void wild(char *s,char** args)
{
	DIR* dir;
	struct dirent* entry;
	int ret,status;
	pid_t pid;
	char* patt;


	patt = malloc(strlen(s)+1);
	strcpy(patt,s);
	
	  ret = FNM_NOMATCH;
	

	if ((dir = opendir(".")) == NULL)
    	perror("opendir() error");
	
	else
	{	
		   
		while(((entry = readdir(dir))!=NULL))
		{
			//psaxnw ta arxeia pou vriskontai mesa sto dir 
			ret = fnmatch(patt, entry->d_name, FNM_PERIOD|FNM_PATHNAME);
			//printf ("ret = %d , %s %s\n", ret,patt,entry->d_name );
		
			//molis vrw to arxeio pou psaxnw kanw fork	 		
			if( ret == 0 )
			{
				strcpy(args[1],entry->d_name);
				//printf("found:%s\n", entry->d_name);

				args[2] = NULL;

				signal(SIGINT,SIG_IGN);

				if((pid = fork()) == -1)
				{
					perror("FORK");
					exit(1);
				}	
	
				if(pid != 0)
				{
					signal(SIGTSTP,SIG_IGN);
					signal(SIGINT,SIG_IGN);

					pid_to_kill = pid;
					 signal(SIGINT,sig_handle);
		
					
					if(wait(&status)!= pid)
					{
						perror("wait");
						exit(1);
					}
				}

				else
				{

					signal(SIGTSTP,SIG_IGN);

					execvp(args[0], args);
	
					perror("exec failure");
       					 exit(-1);

				}
			}
		
			else if ( ret == FNM_NOMATCH )
                               continue;

			 else
                                printf ("ret = %d , error file=%s\n", ret,entry->d_name );

		}

		closedir(dir);

	}

	

	
}


//synartisi gia anakateythinsi kai append
void redirection2(char **args,int c,int append)
{
	int i,j,count, flag;
	char* arg[c];
	char str[10],file[30];
	pid_t pid;
	int fd[2],fp,status;
	FILE* ff;
		
	//vriskw ta arxeia apo ta opoia tha ginei anakateythinsi kai ta fylaw sto args	
	if(append == NOT_APPEND)
	{
		for(i =0; i < c; i++)
		{
			if((strcmp(args[i],">"))==0)
			break;
		}
	}

	//an kanoume append fylaw ta arxeia ta opoia tha kanoume append sto args
	else if(append == APPEND)
	{
		for( i = 0; i < c; i++)
		{
			if((strcmp(args[i],">>"))==0)
			 break;

		}
	}
	
	//vriskw ta orismata pou tha pane sto exec
	for(j=0; j < i; j++)
	{

		if((strcmp(args[j], "<"))==0)
		{
			j++;
			flag =1;
		}

		if(flag ==1)
		{
			strcpy(str,args[j]);
			arg[j-1]=malloc(strlen(str)+1);
			strcpy(arg[j-1],args[j]);
		}
	
		else
		{
			strcpy(str,args[j]);
			arg[j] = malloc(strlen(str)+1);
			strcpy(arg[j],args[j]);
		}
	}

	if(flag == 1)
	arg[i-1] = NULL;
	
	else
	arg[i] = NULL;
	
	
	//dimiourgw to pipe me to opoio tha epikoinwnisoun ta programmata 
	if(pipe(fd) == -1)
	{
		perror("PIPE ERROR");
		exit(1);
	}

	signal(SIGINT,SIG_IGN);

	if((pid = fork()) == -1)
	{
		perror("FORK");
		exit(1);
	}

	if(pid != 0)
	{
		//agnoei ta ctrl-z kai ctrl-c
		signal(SIGTSTP,SIG_IGN);
		signal(SIGINT,SIG_IGN);

		pid_to_kill = pid;
		signal(SIGINT,sig_handle);

		
		if(wait(&status)!= pid)
		{
			perror("wait");
			exit(1);
		}
	
	}

	 else
	{

	//agnoei to ctrl-Z
	signal(SIGTSTP,SIG_IGN);
	
	//an exoume anakateythinsi to stdout ginetai dup me to arxeio sto opoio thelw na grapsw
	if(append == NOT_APPEND)
	{
		if((fp = open(args[i+1], O_WRONLY, S_IRUSR | S_IWUSR | S_IXUSR, 819) )<0)
		perror("OPEN FILE");

		if( dup2(fp, STDOUT_FILENO) < 0)
		perror("DUP");

		if(close(fp)<0)
		  perror("CLOSE FILE");


	}

	//to idio kai gia to append
	if(append == APPEND)
	{

		if((ff = fopen(args[i+1],"a") )==NULL)
		 perror("OPEN FILE");

		if(dup2(fileno(ff),1)<0)
		perror("DUP");

		if(fclose(ff)<0)
		perror("CLOSE");

	}
	
	


	execvp(arg[0], arg);
	
	perror("exec failure");
        exit(-1);

	}

	

}

//synartisi pou ylopoiei ta pipes
void pipes2(char **s, int count)
{
	int fp,i,k,c,j,fd[10][2],t,m,status,flag;
	int read[10], write[10];
	char str[10], *args[10];
	 arg2 array[10];
	pid_t pid[10];
	FILE* ff;

	c = 0;
	t = 0;

	
	for(i = 0; i < count ; i++)
	{
		//koitaw poses fores tha kanw pipe
		if((strcmp(s[i],"|"))==0)
		c++;
 		  
		//elegxw an yparxei anakateythinsi
		if((strcmp(s[i], ">"))== 0)
		{
			//count = count -1;
			flag =1;
		}

		//elegxw an tha ginei append 
		if((strcmp(s[i],">>"))==0)
		{
			flag = 2;
		}

		
	}
	
	//krataw ta orismata pou tha dinw stis exec kathe fora
	for(j = 0; j <= c ; j++)
	{
		
		m = 0;

	for(i = t; i < count; i++)
	{
		
		if((strcmp(s[i], "|"))==0)
		break;

		if((strcmp(s[i], ">"))==0)
		i++;
		
		if((strcmp(s[i], ">>")) == 0)
		i++;

		strcpy(str,s[i]);
		
		array[j].argument[m] = malloc(strlen(str)+1);
		
		strcpy(array[j].argument[m], s[i]);	
  
	
		m++;
	}
	
	t = i+1;

	array[j].argument[m] = NULL;

	}
 
	//dimiourgw ta pipes pou xreiazomai
	for(j =0; j <=c; j++)
	{
	
		if(pipe(fd[j]) == -1)
         	{
                 perror("PIPE ERROR");
                 exit(1);
        	 }

	}

	for(i =0;i <=c; i++)
	{
		signal(SIGINT,SIG_IGN);
		
		if((pid[i] = fork()) == -1)
         	{
               	 	  perror("FORK");
                	 exit(1);
         	}

		if(pid[i] == 0)
          	{
      
			signal(SIGTSTP,SIG_IGN);

			//an einai to prwto programma tote ayto mono grafei se pipe
			//kai diavazei apo to stdin
			if(i==0)
			{
				if(close(fd[i][READ])<0)
				perror("CLOSE1");
			}


			else if(i !=0) 
			{
				if(dup2(fd[i-1][READ],STDIN_FILENO)<0)
				perror("DUP1");

				if(close(fd[i-1][READ])<0)
				perror("CLOSE*");
			}

			//an eimaste sto teleytaio programma tote ayto mono diavazei apo to pipe kai
			//grafei sto stdout i se arxeio an yparxei anakateythinsi
			if(i == c)
			{
				//redirection
				if(flag == 1)
				{
					if((fp = open(s[count-1], O_WRONLY, S_IRUSR | S_IWUSR | S_IXUSR, 819) )<0)
					perror("OPEN FILE");

					if( dup2(fp, STDOUT_FILENO) < 0)
					perror("DUP");

					if(close(fp)<0)
		 			 perror("CLOSE FILE");

				}
			
				//append
				else if( flag == 2)
				{	
					if((ff = fopen(s[count-1],"a") )==NULL)
					 perror("OPEN FILE");

					if(dup2(fileno(ff),1)<0)
					perror("DUP");

					if(fclose(ff)<0)
					perror("CLOSE");


				}
	
				else
				{
					if(close(fd[i][WRITE])<0)
					perror("CLOSE2");
				}
			}

			else
			{
				if(dup2(fd[i][WRITE],STDOUT_FILENO)<0)
				perror("DUp2");


				if(close(fd[i][WRITE])<0)
				perror("CLOSE3");
			}

			closeUselessPipe(fd,i,c);

			
      		         execvp(array[i].argument[0], array[i].argument);
	        	 perror("exec failure");
	         	
		}

		else
		{
			//agnoei ta ctrl-Z kai ctrl-C
			signal(SIGTSTP,SIG_IGN);
			signal(SIGINT,SIG_IGN);

			pid_to_kill = pid[i];
			signal(SIGINT,sig_handle);

			
		}

		
	
	}

	for(i =0; i <=c ;i++)
	{
		 if(close(fd[i][WRITE])<0)
		perror("CLOSE3");

		 if(close(fd[i][READ])<0)
		 perror("CLOSE1");

	}

	
	for(i = 0; i <= c ; i++)
		wait(&pid[i]);
		
	
}

//ekteleseis programmatwn sto backround
void backround(char** s,int count)
{
	int fp,i,k,c,j,fd[10][2],t,m,status,flag;
	char str[10], *args[10];
	arg2 array[10];
	pid_t pid[10],pid1;

	c = 0;
	t = 0;

	
	for(i = 0; i < count ; i++)
	{
		if((strcmp(s[i],"&"))==0)
		c++;
 		  

		if((strcmp(s[i], ">"))== 0)
		{
		
			count = count -1;
			flag =1;
		}
	}
	
	//krataw ta orismata pou tha perasw stin exec
	for(j = 0; j < c ; j++)
	{
		
		m = 0;

		for(i = t; i < count; i++)
		{
		
		if((strcmp(s[i], "&"))==0)
		break;

		if((strcmp(s[i], ">"))==0)
		i++;
	
		strcpy(str,s[i]);
		
		array[j].argument[m] = malloc(strlen(str)+1);
		
		strcpy(array[j].argument[m], s[i]);	
  
	
		m++;
		}	
	
		t = i+1;

		array[j].argument[m] = NULL;

	}
 
		
	//ginontai fork kai exec gia ola ta programmata pou theloume
	for(j = 0; j < c ;j++)
 	{			
				
		signal(SIGINT,SIG_IGN);

		if((pid[j] = fork()) == -1)
         	{
               	 	  perror("FORK");
                	 exit(1);
         	}

       		if(pid[j] == 0)
          	{
			signal(SIGTSTP,SIG_IGN);
			signal(SIGINT,SIG_IGN);

			 execvp(array[j].argument[0], array[j].argument);
	        	 perror("exec failure");
			 exit(-1);

		}		
		
 	}
	
	return;

}

//synartisi pou dimiourgei ena alias
void cr_alias(char **s, int count)
{
	int i,j,c,t;
	char alias[50];
	char com[100][100];
	FILE* fp;


	strcpy(alias, s[1]);
	
	c = 0;

	for(i = 0; i < 100;i++)
	strcpy(com[i],"0");

	for(i = 2; i <count;i++)
	{
		strcpy(com[c], s[i]);
		c++;
	}	

	//an to arxeio gia ta alias den yparxei to dimiourgw	
	if((fp = fopen(".alias","r")) == NULL)
	{
		fp = fopen(".alias", "wb+");
		t=0;

		if(fp == NULL)
		perror("FOPEN");
	}

	else
	fp = fopen(".alias", "rb+");
	

	if(t == 0)
	{
		t++;
		fseek(fp,0,SEEK_SET);
		fwrite(&t,sizeof(int),1,fp);
	}
	
	//ayksanw ton metriti pou deixnei posa aliases exw sto arxeio
	else
	{	
		fseek(fp,0,SEEK_SET);
		fread(&t,sizeof(int),1,fp);
		t++;
		fseek(fp,0,SEEK_SET);
		fwrite(&t,sizeof(int),1,fp);
	}

	fseek(fp,0,SEEK_END);
	
	//grafw sto arxeio to alias
	fwrite(alias,1, sizeof(alias),fp);
	fwrite(com,1,sizeof(com),fp);
	
	fclose(fp);

}

//synartisi pou kaleitai otan dinetai mia apli entoli
void simple_cmd(char** args,int count)
{
	int i,status,t,j;
	pid_t pid;
	char com[100][100],alias[50], *arg[100];;
	FILE* fp;

	if((fp = fopen(".alias","r"))== NULL)
	printf("%s:COMMAND DOES NOT EXIST IN ALIASES\n",args[0]);

	//psaxnw na dw an yparxei mesa sto arxeio me ta aliases i entoli 
	//pou dothike, an dld einai alias
	else
	{

		fseek(fp,0,SEEK_SET);		
		fread(&t,sizeof(int),1,fp);
		

		for(i= 0 ; i < t ; i++)

		{	
			fread(alias, sizeof(alias),1,fp);

			//vrhka to alias pou epsaxna
			if((strcmp(alias,args[0]))==0)
			{	
				//diavazw ta orismata piu tha dwthoun stin exec
				fread(com,sizeof(com),1,fp);

				for(j  = 0; j < 100;j++)
				{
					if((strcmp(com[j],"0"))==0)
					break;

					arg[j] = malloc(strlen(com[j])+1);
					strcpy(arg[j],com[j]);
				}

				arg[j]= NULL;

				fclose(fp);

				signal(SIGINT,SIG_IGN);			

				if((pid = fork()) == -1)
				{
					perror("FORK");
					exit(1);
				}

				if(pid == 0)
				{
					signal(SIGTSTP,SIG_IGN);

			
					execvp(arg[0],arg);
					perror("---EXEC");
					exit(-1);
				}
				
				else
				{
					signal(SIGTSTP,SIG_IGN);
					signal(SIGINT,SIG_IGN);

					pid_to_kill = pid;
					signal(SIGINT,sig_handle);

					if(wait(&status)!= pid)
					{
						perror("WAIT");
						exit(1);
					}
				}

				return;
				
			}

			fseek(fp,sizeof(com),SEEK_CUR);
		}

		  fclose(fp);
		

	}

	if((strcmp(args[0],"cd"))==0)
	{
		 if (chdir(args[1]) != 0)
    		perror("CHDIR FAILED");
			
		return;
	}

	signal(SIGINT,SIG_IGN);

	if((pid = fork()) == -1)
	{
		perror("FORK");
		exit(1);
	}

	args[count] = NULL;

	if(pid == 0)
	{
		signal(SIGTSTP,SIG_IGN);

		
		execvp(args[0],args);
		perror("---EXEC");
		exit(-1);
	}
	
	else
	{
		signal(SIGTSTP,SIG_IGN);
		signal(SIGINT,SIG_IGN);

		pid_to_kill = pid;
		signal(SIGINT,sig_handle);
		
	
		if(wait(&status)!= pid)
		{
			perror("WAIT");
			exit(1);
		}
	}

}

//synartisi pou katastrefei ena alias
void  destroy_alias(char* args)
{
	FILE* fp;
	int i,t,j;
	char alias[50],com[100][100];


	if((fp = fopen(".alias","rb+"))== NULL)
	perror("FOPEN");
	
	else
	{
		//diavazw to metriti pou vrisketai stin arxi tou arxeiou kai deixnei posa aliases exw
		fseek(fp,0,SEEK_SET);		
		fread(&t,sizeof(int),1,fp);
		
		//psaxw ta aliases gia na vrw ayto pou thelw
		for(i= 0 ; i < t ; i++)

		{	
			fread(alias, sizeof(alias),1,fp);

			if((strcmp(alias,args))==0)
			{	//puts("FOUND ALIAS TO DESTROY");
				

				for(j = 0; j<(strlen(args)+1); j++)
				alias[j] = '0';
	
				fseek(fp, -sizeof(alias), SEEK_CUR);

				fwrite(alias,1,sizeof(alias),fp);

				fclose(fp);
			
				return;
				
			}

			fseek(fp,sizeof(com),SEEK_CUR);
		}

		  fclose(fp);
		

	}

}


void analyze_input(char *s, int length)
{
	char* result;
	int i,j, flag,count;
	char in[256];
	char* args[10];

	count = param(s, args);

	if((strcmp(args[0],"history")==0))
	{
		history();
	}

	for(i = 0; i < count; i++)
	{
		//PIPES
		if((strcmp(args[i], "|")) == 0)
		{	
		
			pipes2(args,count);
			return;
		}
	}

	for(i = 0; i < count; i++)
	{
		//REDIRECTION
		if((strcmp(args[i], ">"))==0)
		{
			redirection2(args,count,NOT_APPEND);
			return;
		}
	}

	for(i = 0; i < count; i++)
	{
		//APPENDING
		if((strcmp(args[i], ">>")) == 0)
                {
			redirection2(args,count,APPEND);
			return;
                }
	}

	for(i = 0; i < count; i++)
	{
		//WILDCHARS
		if(i == 1)
		{	
			for(j = 0;  j<strlen(args[1]);j++)
			{
				if((args[1][j] == '*') || (args[1][j] == '?'))
				{
					wild(args[1],args);
					return;		
				}
			}
		}
	}

		//CREATE ALIAS
		if((strcmp(args[0], "createalias"))==0)
		{
			cr_alias(args,count);
			return;
		}

		if((strcmp(args[0],"destroyalias"))==0)
		{
			destroy_alias(args[1]);
			return;
		}
		
	for(i = 0; i < count; i++)
	{
		//BACKROUND
		if((strcmp(args[i],"&"))==0)
		{
			puts("BACKROUND");

			backround(args,count);
			return;
		}
	}
		//SIMPLE COMMAND
		simple_cmd(args,count);
		return;
}





//ayti i synartisi kaleitai otan pliktrologoume tin entoli "history"
//kai deixnei tis entoles pou einai apothykeymens sto arxeio .history
//gia na epileksei o xristis poia thelei na ektelesei
void history()
{
	FILE* fp;
	int count,i;
	hist2 str;	


	if((fp=fopen(".history","rb"))==NULL)
	perror("FOPEN");

	else
	fread(&count,sizeof(int),1,fp);

	printf("Choose the command you want to execute:\n");
	for(i=0;i < count; i++)
	{
		fread(&str,1,sizeof(struct hist),fp);
		printf("%s %d %s\n",str.his,str.c,str.inp);
		
	}

	scanf("%d",&count);

	fseek(fp,sizeof(int),SEEK_SET);

	fseek(fp,count*sizeof(struct hist),SEEK_CUR);

	fread(&str,1,sizeof(struct hist),fp);
	
	fclose(fp);

	analyze_input(str.inp,strlen(str.inp));
}
//synartisi pou diavazei olokliri tin entoli pou dinetai sto shell
int mygetline(char *s, int k)
{
	int c,i;

	for (i=0; i < k-1 && (c=getchar())!=EOF && c!='\n'; i++)
		s[i]=c;

	if (c=='\n')
 	{
		s[i]=' ';
		++i;
	}

	s[i]='\0';
	return (i);
}



int main()
{
	char input[256];
	int t;
	hist2 hi;
	FILE* fp;

	while(1)
	{
		printf("in-rssh-now:> ");

		fflush(stdout);

		if(mygetline(input, 256)!=0)
		{
			if(strcmp(input, "exit ")==0)
			exit(1);

			//apothikeyw kathe entoli pou dinetai sto arxeio .history
			else 
			{
				if((fp = fopen(".history","r")) == NULL)
				{
					fp = fopen(".history", "wb+");
					t = 0;
		
					if(fp == NULL)
					perror("FOPEN");
				}

				else
				{	
					fp = fopen(".history", "rb+");
					t=1;
				}
	
				//an dimiourgeitai twra to arxeio .history o metritis exei tin timi 0
				if(t==0)
				{
					fwrite(&t,sizeof(int),1,fp);
					strcpy(hi.his,"history");
					hi.c=t;
					strcpy(hi.inp,input);

					fwrite(&hi,sizeof(struct hist),1,fp);
					t++;
					fseek(fp,0,SEEK_SET);
					fwrite(&t,sizeof(int),1,fp);
				}		

				else if( t!= 0)
				{
					fread(&t,sizeof(int),1,fp);
					fseek(fp,t*sizeof(struct hist),SEEK_CUR);
						
					strcpy(hi.his,"history");
					hi.c=t;
					strcpy(hi.inp,input);
				
					fwrite(&hi,sizeof(struct hist),1,fp);
					t++;
					fseek(fp,0,SEEK_SET);
					fwrite(&t,sizeof(int),1,fp);
				}


				fclose(fp);

				analyze_input(input, strlen(input));	
			}
		}
	}


}


