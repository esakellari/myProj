#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>


/*global string pou krataw to onoma tou directory pou vazw sto baya, stin periptwsi pou dinoume path*/
char ini[20];

char** path3;

/*domi header stin arxi tou archive file me tis 
 * plifororries gia to arxeio
 * */
struct header{
		char* files[20];
		int num_files;
		long int dict;

		};
/* domi pou krataei tis metaplirofories gia kathe arxeio pou eisagetai*/
struct metadata{

		char name[20];
		char path[60];
		char root[20];
		char rel[30];
		long int size;
		long int tell;
		int uid;	
		int gid;
		int modes;
		int is_dir;
		/*metavliti pou leei an ayto to arxeio einai sybiesmeno*/
		int compress;
		

		};

/*synartisi ektypwsis lathwn analoga me to lathos pou exei prokypsei*/
void print_error( int errcode)
{

	switch(errcode)
	{
		case 0:
			perror("open for create failed");
			exit(-1);
		case 1:
			perror("open for reading failed");
			exit(-1);
		case 2:
			perror("open for writing failed");
			exit(-1);
		case 3:
			perror("closing failed");
			exit(-1);
		case 4:
			perror("writing failed");
			exit(-1);
		case 5:
			perror("readin failed");
			exit(-1);
		case 6:
			perror("making dir failed");
			exit(-1);
		case 7:
			perror("stat failed");
			exit(-1);
		case 8:
			perror("realloc failed");
			exit(-1);
		case 9 :
			perror("malloc failed");
			exit(-1);
		case 10:
			perror("cannot open directory");
			exit(-1);
		case 11:
			perror("cannot change dir!");
			exit(-1);
		case 12:
			perror("error getcwd");
			exit(-1);
	}
	
	
}
/*synartisi pou elegxei an ena arxeio yparxei idi*/
int dir_exists(const char * filename)
{
    DIR * dir;

    if ((dir = opendir(filename)) != NULL)
    {
        closedir(dir);
        return 1;
    }
    return 0;
}


/*synartisi pou elegxei an ena arxeio yparxei idi*/
int file_exists(const char * filename)
{
    FILE * file;

    if (file = fopen(filename, "rb"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}




/*an to string pou dinetai einai path, i synartisi epistrefei 1, an den einai,
 *epistrefei 0
 */
int it_is_a_path( const char* str)
{
	int i ;


	for ( i = 0; i < strlen(str); i++)
	{
		if( str[i] == '/')
		return 1;
		
	}

	return 0;


}

/*ayti i synartisi metaferei to leksiko otan prokeitai na eisagoume neo arxeio*/
void trans_dict(int size, int files, const char* baya, long tell)
{

	struct metadata **meta;
	int i;
	FILE* fp;
	

	meta = malloc( files*sizeof(struct metadata));
	
	if(meta == NULL)
	print_error(9);

	for( i = 0 ; i < files; i++)
	{
		meta[i] = malloc(sizeof(struct metadata));

		if( meta[i] == NULL)
		print_error(9);
	}

	if(( fp = fopen(baya, "rb+")) == NULL)
	print_error(2);

	/*phgainw sto shmeio pou einai to leksiko*/
	fseek(fp, tell, SEEK_SET);

	/*diavazw olokliro to leksiko kai vazw kathe ena metadata structure se mia thesi
	tou pinaka*/
	for( i = 0; i < files; i++)
	fread(meta[i], sizeof(struct metadata), 1, fp);

	fseek(fp, tell + size, SEEK_SET);
	
	/*ksanagrafw to leksiko sti nea tou thesi*/
	for( i = 0; i < files; i++)
	fwrite(meta[i], sizeof(struct metadata), 1, fp);

	fclose(fp); 

	for( i = 0; i < files ; i ++)
	free(meta[i]);

	free(meta);


}

/*i metavliti comp deixnei an to arxeio pou tha eisaxthei tha ypostei sympiesi i oxi. An comp == 1 to arxeio sybiezetai,
an comp == 0 to arxeio de sybiezetai*/  
void insert_from_dir(const char* file, const char* entry, int size, const char* cwd,int flag, int comp)
{
	char cwd2[1024];
	char cwd3[512];
	char entry2[30];
	char buf[size];
	FILE* fp;
	FILE* fd;
	struct header* head;
	struct metadata* meta;
	struct stat info;
	char *args[10];
	int pid, i,status;
	
	head = malloc(sizeof(struct header));
	meta = malloc(sizeof(struct metadata));


	if(getcwd(cwd2, sizeof(cwd2)) == NULL)
        print_error(12);

	
	/*an exei dothei san orisma to "-j" tote kanoume sybiesi*/
	if(comp == 1 )
	{
		printf("compress\n");

		for( i = 0; i < 10; i ++)
		args[i] = malloc( 32);
	
		strcpy(args[1], entry);
		args[2] =  NULL;
	
		/*apo to directory pou vriskomai vazw se enan buffer to arxeio pou thelw gia
		na to metaferw sto directory pou vrisketai to ektelesimo tou compress kai to archive baya*/
		fd = fopen(entry, "rb+");
		
		fread(buf,1, size, fd);

		fclose(fd);

		/*phgainw sto directory pou vrisketai to baya archive*/
		chdir(cwd);
	
		/*dimiourgw ekei to antistoixo arxeio pou thelw gia na ginei compress*/
		fd = fopen(entry, "wb+");
		
		fwrite(buf, 1, size, fd);
		
		fclose(fd);
			
		//printf("size befor comp = %d\n", size);		
	
		/*diadikasia tis sybiesis*/
		if( (pid  = fork()) == 0)
		{
			execvp("comp",args); 
				
			perror("exec failure");
		}
		
		pid = wait(&status);
		
		/*to neo onoma tou arxeiou exei tin kataliksi .Z*/
		sprintf(entry2, "%s.Z", entry);
		
		/*svinw to proswrino arxeio pou dimiourgisa gia na ginei compressed*/
		remove(entry);		

		/*pairnw tis plirofories gia to neo, sybiesmeno arxeio*/
		if(stat(entry2, &info)!= 0)
		print_error(7);

	
		if((fd  = fopen(entry2, "rb+")) == NULL)
		print_error(2);


		/*diavazw olokliro to arxeio se ena buffer giati tha allaksw directory gia na to grapsw sto file */
		fread(buf, info.st_size, 1, fd);

		fclose(fd);
		
		/*diagrafw to sybiesmeno arxeio*/
		remove(entry2);

		/*neo megethos*/
		size = info.st_size;

		//printf("size after comp = %d\n", size);
	
		/*apothikeyw ta stoixeia tou arxeiou*/
		strcpy(meta->name, entry2);
		meta->size = size;
		meta->uid = info.st_uid;
		meta->gid = info.st_gid;
		meta->modes = info.st_mode;
		meta->compress = comp;
		strcpy(meta->root, ini);

		/*phgainw sto fakelo pou vrisketai to archive-file*/
		if((chdir(cwd))!= 0)
		print_error(11);

		/*apothikeyw to path tou arxeiou*/
		strcpy(meta->path, cwd2);
		
		/*anoigw to archive baya*/
                if((fp =  fopen(file, "rb+")) == NULL)
                print_error(0);

              	/*diavazw to header apo to arxeio gia na kanw tis aparaithtes allages*/
		fseek(fp, 0, SEEK_SET);
		fread(head, sizeof(struct header), 1, fp);

		fclose(fp);	

		/*metaferw to dictionary gia na mpei to neo arxeio */
		trans_dict(size, head->num_files, file, head->dict);

		fp = fopen(file, "rb+");

		/*ayksanw ton arithmo twn arxeiwn pou vriskontai sto baya arxeio*/
	       	 head->num_files++;

		/*enhmerwnw to header sxetika me ti thesi tou leksikou*/
		//head->dict = head->dict + size;
		
		/*phgainw sto shmeio tou arxeiou baya pou prepei na mpei to neo arxeio */
		fseek(fp, head->dict-size, SEEK_SET);

		fwrite(buf, 1, size, fp);
	
		/*enhmerwnw to header sxetika me ti thesi tou leksikou*/
		fseek(fp, 0, SEEK_CUR);
		head->dict = ftell(fp);

		/*phgainw na dw apo pou ksekinaei to sygekrimeno arxeio sto baya */
		fseek(fp, -size, SEEK_CUR);

		/*enhmerwnw to metadata gia ti thesi tou arxeiou*/
		meta->tell = ftell(fp);

		/*phgainw na grapsw to metadata tou arxeiou sto telos tou leksikou*/
		fseek(fp, 0, SEEK_END);
		fwrite(meta, 1, sizeof(struct metadata), fp);

		/*grafw to header pali stin arxi tou arxeiou*/
		fseek(fp, 0, SEEK_SET);
		fwrite(head,1, sizeof(struct header), fp);
	
		fclose(fp);
	
		/*epistrefw sto dir pou vriskotan to arxeio pou eishxthh*/
		chdir(cwd2);
		

		for( i = 0; i < 10; i++)
		free(args[i]);

	}

	/*an de xreiazetai sybiesi*/
	else{
		
		if((fd  = fopen(entry, "rb+")) == NULL)
		print_error(2);


		/*diavazw olokliro to arxeio se ena buffer giati tha allaksw directory gia na to grapsw sto file */
		fread(buf, size, 1, fd);

		fclose(fd);

		/*pairnw tis plirofories gia to arxeio*/
		if(stat(entry, &info)!= 0)
		print_error(7);

		//printf("uid : %d gid : %d\n", (int) info.st_uid, (int) info.st_gid);
	
		/*enhmerwnw ta metadata tou arxeiou*/
		strcpy(meta->name, entry);
		meta->size = size;
		meta->uid = info.st_uid;
		meta->gid = info.st_gid;
		meta->modes = info.st_mode;
		meta->compress = comp;
		strcpy(meta->root, ini);

		/*phgainw sto fakelo pou vrisketai to archive-file*/
		if((chdir(cwd))!= 0)
		print_error(11);

		
		strcpy(meta->path, cwd2);
		printf("%s\n", cwd2);	
	
                if((fp =  fopen(file, "rb+")) == NULL)
                print_error(0);

           	/*diavazw to header apo to arxeio gia na kanw tis aparaithtes allages*/
		fseek(fp, 0, SEEK_SET);
		fread(head, sizeof(struct header), 1, fp);

		fclose(fp);	

		/*metaferw to dictionary gia na mpei to neo arxeio */
		trans_dict(size, head->num_files, file, head->dict);

		fp = fopen(file, "rb+");

		/*ayksanw ton arithmo twn arxeiwn pou vriskontai sto baya arxeio*/
        	head->num_files++;

		head->dict = head->dict + size;

		/*phgainw sto shmeio tou arxeiou baya pou prepei na mpei to neo arxeio */
		fseek(fp, head->dict-size, SEEK_SET);

		fwrite(buf, 1, size, fp);
	
		/*phgainw na dw apo pou ksekinaei to sygekrimeno arxeio sto baya */
		fseek(fp, -size, SEEK_CUR);
		meta->tell = ftell(fp);
	
		/*phgainw na grapsw to metadata tou arxeiou sto telos tou leksikou*/
		fseek(fp, 0, SEEK_END);
		fwrite(meta, 1, sizeof(struct metadata), fp);

		/*grafw to header stin arxi tou arxeiou*/
		fseek(fp, 0, SEEK_SET);
		fwrite(head,1, sizeof(struct header), fp);
	
		fclose(fp);
	
		/*epistrefw sto fakelo pou hmoun */
		//if(flag == 0)
		chdir(cwd2);
	
	}


} 

/*me ayti ti synartisi topothetw sta metadata ,structs apo  metadata gia directories*/
void put_dir(const char* baya, const char * dir, const char* cwd, const char* cwd2 )
{
	struct metadata * meta;
	struct header * head;
	struct stat info;
	FILE * fp;

	meta = malloc(sizeof(struct metadata));
	head = malloc(sizeof(struct header));

	/*pairnw tis plirofories gia to directory*/
        if(stat(dir, &info) != 0)
        print_error(7);

	/*phgainw sto dir pou vrisketai to archive file*/
	if( chdir(cwd)!= 0 )
	print_error(11);

	if(( fp = fopen(baya, "rb+")) == NULL)
	print_error(2);

	/*diavazw to header*/
	fseek(fp, 0, SEEK_SET);

	fread(head, 1, sizeof(struct header), fp  );

	/*enhmerwnw ta dedomena tou mertadata*/
	strcpy(meta->name, dir);
	strcpy(meta->path, cwd2);
	
	/*metaviti pou deixnei oti to sygekrimeno metadata anaferetai se dir*/
	meta->is_dir = 1;
	meta->uid = info.st_uid;
	meta->gid = info.st_gid;
	meta->modes = info.st_mode;

	head->num_files++;

	fseek(fp, 0, SEEK_END);
	
	/*grafw to metadata sto telos tou arxeiou*/
	fwrite(meta, 1, sizeof(struct metadata), fp);

	fseek(fp, 0, SEEK_SET);

	/*grafw kai to header stin arxi tou arxeiou */
	fwrite(head, 1, sizeof(struct header), fp);

	fclose(fp);

	if( chdir(cwd2)!= 0 )
	print_error(11);


	free(head);
	free(meta);

}

/*synartisi pou vazei anadromika ta periexomena enos directory sto baya arxeio */
void insert_dir_file(const char * dir_file, const char* filename, const char* cwd, int comp)
{
	DIR* dir;
	struct dirent* entry;
	struct stat info;
	char cwd2[512];

	
	/*anoigw to direcory kai mpainw mesa se ayto*/
	if( (dir = opendir(dir_file)) == NULL)
	print_error(10);

	if((chdir(dir_file)) != 0 )
	print_error(11);              


	else
	{	/*diavazw ena ena ta periexomena tou , kai analoga me to an einai directory i file, kalw tin
		synartisi anadromika, i eisagw to file sto baya antistoixa */
		while ((entry = readdir(dir)) != NULL)
		{
			if((strcmp(entry->d_name, "." )!=0 ) && (strcmp(entry->d_name, ".." )!=0 ))
			{
				if(stat(entry->d_name, &info) != 0)
				print_error(7);
			
				else
				{
					/*an einai directory kalw anadromika ti synartisi kai kanw to idio kai gia ayto to
 					 *directory 
					 */
					if((S_ISDIR(info.st_mode) != 0) )
					{
						/*krataw to monopati aytou tou fakelou*/
						if(getcwd(cwd2, sizeof(cwd2)) == NULL)
	                                        print_error(12);

						/*eisagw metadata gia to fakelo*/
						put_dir(filename , entry->d_name,cwd, cwd2 );

						insert_dir_file(entry->d_name, filename,cwd, comp);
					}
					else
					/*an den einai directory, to eisagw sto baya*/
					insert_from_dir(filename, entry->d_name,info.st_size, cwd, 0, comp);
					
						
					
				}
			}
		
		}
		/*kleinw to directory kai epistrefw sto directory pou vrisketai to baya*/
		closedir(dir);
		chdir("..");
	}
	
}

/*ayti i synartisi eisagei ena arxeio i directory pou exei dwthei me monopati*/
void insert_path(const char* pth,const char* filename,  const char* home, int comp )
{

	char* path;
	int j,i,count,t, count2,m;
	char** path2;
	DIR* dir;
	struct dirent* entry;
	struct stat info;
	char cwd[200];
	char cwd2[200];
	char* pch;

	count = 0;
	t  = 0;
	count2 = 0;
	m = 1;

	
	path =  malloc(strlen(pth));
	if(path == NULL)
	print_error(9);

	if((fopen(filename, "rb+")) == NULL)
	print_error(2);

	strcpy(path, pth);
	
	/*psaxnw na dw apo posa "stoixeia" apoteleitai to monopati*/
	for( i = 0; i < strlen(path);i++)
	{
		if( path[i] == '/')
		count++;
	}
	
	
	/*pinakas ston opoio kratame ta "stoixeia " tou path*/
	path2 =  malloc(count*sizeof(char *));
	
	if(path2 == NULL)
	print_error(9);

	/*vazoume ta stoixeia tou path ston pinaka path2*/
	for(j = 0; j < count; j++)
	{
		{
			for( i = m ; i <= strlen(path); i++)
			{
				t++;
				/*metavliti pou deixnei se poio stoixeio tou strin vriskomaste*/
				m++;
	
				if(path[i] == '/')
				break;
				
			}
		}

		path2[count2]= malloc(t);

		if(path2[count2] ==  NULL)
		print_error(9);

		if(count2 == 0)
		{
			for( i = 0; i < (t); i++)
			path2[count2][i] = path[m-t+i-1];
               }

		else
		{
			for( i =0; i < t; i++)
			path2[count2][i] = path[m-t+i];

		}
  
       		t = 0;		
		count2++;
		

		
	}
	

	/*psaxnoume na vroume apo pou ksekinaei to path*/
	do{	

		if((chdir(".."))!= 0 )
		perror("chdir failed");

		if(getcwd(cwd, sizeof(cwd)) == NULL)
		perror("getcwd failed");

	}while( (strcmp(cwd, path2[0]))!= 0  );


	/*antigrafw sto global string to onoma tou arxikou directory pou vazoume sto baya*/
	strcpy(ini, path2[count-1]);

	for(i = 1; i <count ; i++)
	{
		printf("%s\n", path2[i]);
	
		if(stat(path2[i], &info) != 0)
                 perror("stat failed");


		if((S_ISDIR(info.st_mode) != 0) && ( i != count -1) )
		{	
		 	if(getcwd(cwd2, sizeof(cwd2)) == NULL)
                 	perror("getcwd failed");

			put_dir(filename, path2[i], home, cwd2  );
		}

		if(i == count-1)
		{
			if(stat(path2[i], &info) != 0)
			perror("stat failed");

			else
			{
				if((S_ISDIR(info.st_mode) != 0) )
				{
					printf("%s is a directory\n", path2[i]);
					insert_dir_file(path2[i], filename, home, comp);
				}
	
				else
				{
					printf("%s is a file \n", path2[i]);
					insert_from_dir(filename, path2[i], info.st_size, home, 1, comp);
				}
			}
		}

		else
		{
			if((chdir(path2[i]))!= 0 )
			perror("chdir failed");
		}
	}


   	for(i = 0; i < count; i++)
   	free(path2[i]);
   
  	 free(path2);
  	 free(path);



}
void insert_file(const char * file_name, const char * filename2, int comp )
{
	FILE* fp;
	FILE* fd;
        struct metadata* meta;
	struct header* head;
	char *buffer;
	struct stat info;
	int i, pid,status;
	char *args[10];
	char file_name2[20];
	
	
	head = malloc(sizeof(struct header));
	meta = malloc(sizeof(struct metadata));
	
	/*an exei dothei san orisma to "-j" tote kanoume sybiesi*/
	if(comp == 1 )
	{
		printf("compress\n");

		for( i = 0; i < 10; i ++)
		args[i] = malloc( 32);
	
		strcpy(args[1], filename2);
		args[2] =  NULL;
	
		
		if( (pid  = fork()) == 0)
		{
			execvp("comp",args); 
				
			perror("exec failure");
		}
		
		pid = wait(&status);

	//	sprintf(file_name2, "%s.Z", filename2);
		sprintf(file_name2, "%s.Z", filename2);

		for( i = 0; i < 10; i++)
		free(args[i]);
	}
	
	else 
	strcpy(file_name2,filename2);
	

	

	/*anoigw to arxeio pou tha grapsw*/
	if(( fp  = fopen(file_name2, "rb+")) == NULL )
	print_error(1);
	


	/*anoigw to baya arxeio*/
	  if(( fd  = fopen(file_name,"rb+")) ==  NULL )
        {
		printf("EDWWWW\n");
		 printf("%s\n", file_name);
	
                 print_error(2);

         }

	fseek(fd, 0, SEEK_SET);
	fread(head, 1, sizeof(struct header), fd);
	
	//printf("files = %d\n",  head->num_files);

	 printf("%s\n", file_name2);
	

	if(stat(file_name2, &info) != 0)
	print_error(7);

	meta->size = info.st_size;
	meta->uid = info.st_uid;
	meta->gid = info.st_gid;
	meta->modes = info.st_mode;
	meta->compress = comp;
	//meta->path = NULL;

	strcpy(meta->name, file_name2);
	printf("file = %s\n", meta->name);

	/*metaferw to leksiko gia na mpei to neo arxeio*/
	
	trans_dict(meta->size, head->num_files, file_name, head->dict);

	buffer =  malloc(meta->size);

	fseek(fd, head->dict, SEEK_SET);

	/*grafw sto metadata to shmeio pou arxizei to arxeio*/
	//fseek(fd , 0, SEEK_CUR);
	meta->tell = ftell(fd);
	
	head->dict = head->dict + meta->size;

	/*grafw to arxeio sto baya arxeio*/
	fread( buffer, 1,  meta->size ,fp);
	
	fwrite( buffer, 1,  meta->size , fd);

	head->num_files++;
	
	/*grafw to metadata sto telos tou leksikou*/
	fseek(fd, 0, SEEK_END);
	fwrite(meta, 1, sizeof(struct metadata), fd);

	/*grafw to header*/	
	fseek(fd, 0, SEEK_SET);
	fwrite(head, 1, sizeof(struct header), fd);

	fclose(fd);
	fclose(fp);
	
	free(meta);	
	free(head);
	free(buffer);

}

/*ektypwnei ta periexomena tou arxeiou baya*/
void ierarchy( const char* baya)
{
	int i;
	struct metadata** meta;
	struct header* head;
	FILE* fp;

	head = malloc( sizeof(struct header));

	if((fp =  fopen(baya, "rb+")) == NULL )
	print_error(2);

	fseek(fp, 0, SEEK_SET);
	fread(head, 1, sizeof(struct header),fp);


	meta = malloc (head->num_files * sizeof(struct metadata));
	if( meta ==  NULL)
	print_error(9);

	for( i = 0; i < head->num_files; i++)
	{
		meta[i] =  malloc(sizeof(struct metadata));

		if( meta[i] == NULL)
		print_error(9);
	}

	/*pame sto simeio opou arxizei to leksiko*/
	fseek(fp, head->dict, SEEK_SET);
	
	printf("dict is at %d files = %d\n", head->dict,head->num_files);

	for( i = 0; i < head->num_files; i++)
	fread(meta[i], 1, sizeof(struct metadata), fp);

	printf("These files are included in the baya archive:\n");

	for( i = 0; i < head->num_files; i++)
	printf("%s/%s\n",  meta[i]->path, meta[i]->name);
		
	

	for( i  = 0; i < head->num_files; i++)
	free(meta[i]);

	free(meta);
	free(head);

	fclose(fp);

}


int  analyze_path( const char* path)
{
	int j,i,count,t, count2,m;
	char** path2;


	count = 0;
	t  = 0;
	count2 = 0;
	m = 1;

	
//	printf("%s\n", path);

	for( i = 0; i < strlen(path);i++)
	{
		if( path[i] == '/')
		count++;
	}
	
	//printf("count = %d\n", count);
	
	/*pinakas ston opoio kratame ta "stoixeia " tou path*/
	path2 =  malloc(count*sizeof(char *));
	
	if(path2 == NULL)
	perror("malloc failed");

	/*vazoume ta stoixeia tou path ston pinaka path2*/
	for(j = 0; j < count; j++)
	{
	
		for( i = m ; i <= strlen(path); i++)
			{
				t++;
				/*metavliti pou deixnei se poio stoixeio tou strin vriskomaste*/
				m++;
	
				//printf("i = %d\n", i);
			
				if(path[i] == '/')
				{
					break;
				}
			}
	
	
		/*desmeyw xwro gia kathe stoixeio tou monopatiou*/
		path2[count2]= malloc(t);

		if(path2[count2] ==  NULL)
		perror("malloc failed");


			for( i =0; i < t; i++)	
			{

				
				if(path[m-t+i] == '/')
				{
					path2[count2][i] = '\0';
					break;
				}

				path2[count2][i] = path[m-t+i];
			}
	
      

		t = 0;		
		count2++;
		

		
	}
	
	 

	for( i = 0; i < count; i++)
	{
		//printf("%s\n", path2[i]);
	
		if( (strcmp(path2[i], ini) )== 0 )
		break;
	}



	path3 = malloc((count - i)*sizeof(char *));

	if( path3 == NULL)
	print_error(9);

	for( j = 0; j < count -i ; j++)
	path3[j] = malloc(strlen(path2[i+j]));

	//for( j = 0; j < count-i; j++)
	//printf("%s ", path2[i+j]);

	//printf("\n");

	for( j = 0; j < count - i; j++)
	strcpy( path3[j], path2[i+j]);

	for( j = 0; j < count- i ; j++)
	printf("-----------%s\n", path3[j]);
	
	for( j = 0; j < count ; j++)
	free(path2[j]);

	free(path2);

	return (count-i);

}


/*synartisi pou apla allazei directory*/
void ex_dir( int i)
{
	
	DIR * dir;

	/*an to directory yparxei idi, mpainoume se ayto*/	
	if( dir_exists(path3[i]) == 1)
	{
		if( chdir(path3[i])!= 0 )
		print_error(11);
	}
	
	/*an den yparxei, to dimiourgw, kai mpainw se ayto*/
	else
	{
		if(mkdir(path3[i], S_IRWXU|S_IRGRP|S_IXGRP)!= 0)
		print_error(6);

		if( chdir(path3[i]) != 0 )
		print_error(11);
	}	
	


}



void extract( const char * baya)
{
	struct metadata ** meta;
	struct header * head;
	FILE * fp;
	FILE * new;
	char * buf;
	int i,count,j,pid,status;
	char cwd[200];
	char cwd2[200];
	char* args[10];
	char* new_name;
	struct stat info;

	
	head = malloc(sizeof(struct header));

	if(( fp = fopen(baya, "rb+")) == NULL )
	print_error(2);

	fseek(fp, 0, SEEK_SET);
	
	fread(head, 1, sizeof(struct header), fp);

	/*pinakas opou tha krataw ola ta metadata*/
	meta = malloc( head->num_files* sizeof(struct metadata));

	for( i = 0; i < head->num_files; i++)
	meta[i] = malloc(sizeof(struct metadata));

	 /*phgainw ekei ppou arxizei to leksiko */
         fseek(fp, head->dict, SEEK_SET);
 
	/*diavazw ta metadata ston pinaka*/
	for( i  = 0; i < head-> num_files; i++)
	fread(meta[i], 1, sizeof(struct metadata), fp);	

	
	for( i = 0; i < head->num_files; i++)
	{
		 //printf("%s\n", meta[i]->path);

		if(meta[i]->is_dir != 1)
		{
			/*an to arxeio exei dwthei apo path*/
			if( (it_is_a_path(meta[i]->path)) == 1)
			{

			//	printf("OLAOALALA %s\n", meta[i]->root);
				strcpy(ini, meta[i]->root);

				/*analyw to monopati kai krataw mono ta stoixeia tou monopatiou apo to arxiko directory 
				pou sybiezoume*/
				count  = analyze_path(meta[i]->path);
			
				/*krataw to directory sto opoio vriskomai, gia na epistrepsw meta*/
				getcwd(cwd, sizeof(cwd));
			
			
				/*phgainw ston katallilo katalogo opou tha topothetithei to arxeio*/
				for( j = 0; j < count ; j++)
				ex_dir(j);
			
				getcwd(cwd2, sizeof(cwd2));			

				/*an to arxeio exei sybiestei*/
				if(meta[i]->compress == 1)
				{
					/*phgainw sto dir pou vrisketai to archive baya*/
					chdir(cwd);
					
					/*diavazw to arxeio pou thelw apo to baya*/
					buf = malloc(meta[i]->size);

					fseek(fp, meta[i]->tell, SEEK_SET);

					fread(buf,1, meta[i]->size, fp);
			
					/*dimiourgw se ayto to dir to antistoixo sybiesmeno arxeio pou tha aposybiestei*/
					new = fopen(meta[i]->name, "wb");

					fwrite(buf,1,meta[i]->size, new);
		
					free(buf);
			
					fclose(new);

					for( j = 0; j < 10; j++)
					args[j] = malloc( 32);
	
					strcpy(args[1], "-d");
					strcpy(args[2], meta[i]->name);
					args[3] = NULL;

					/*aposybiesi*/
					if( (pid  = fork()) == 0)
					{
						execvp("comp",args); 
				
						perror("exec failure");
					}
		
					pid = wait(&status);
		
	
					new_name = malloc(strlen(meta[i]->name)-1);
					
					strncpy(new_name, meta[i]->name, strlen(meta[i]->name) -1);

					new_name[strlen(meta[i]->name)-2]= '\0';
				
						printf("new name = %s\n", new_name);
	
			
					if(stat(new_name, &info)!= 0)
					print_error(7);
		
							
					buf = malloc(info.st_size);

					new = fopen(new_name,"rb+");

					fread(buf,1, info.st_size, new);

					fclose(new);
				
					chdir(cwd2);

					new = fopen(new_name, "wb");
					
					fwrite(buf,1,info.st_size,new);
			
					fclose(new);

					free(buf);
		
					chdir(cwd);

				}

				else
				{
					 /*dimiourgw to buffer sto opoio tha mpei to arxeio*/
			                buf = malloc(meta[i]->size);
	
					/*phgainw sto dir pou vrisketai to archive baya*/
					chdir(cwd);

            				/*phgainw kai diavazw to arxeio */
			                fseek(fp, meta[i]->tell, SEEK_SET);

                 			fread(buf, 1, meta[i]->size, fp);

					/*phgainw sto dir opou tha dimiourgithei to arxeio*/
					chdir(cwd2);
					
					/*dimiourgw to neo arxeio*/
					new =  fopen(meta[i]->name, "wb");

					if( new == NULL)
					print_error(2);

					fwrite(buf, 1, meta[i]->size, new);
		
					free(buf);

					fclose(new);	

					/*epistrefw sto directory pou vriskomai*/
					if( chdir(cwd) != 0)
					print_error(11);

				}
			} /*if( (it_is_a_path(meta[i]->path)) == 1)*/

			else
			{
				if(meta[i]->compress == 1)
				{
					buf = malloc(meta[i]->size);
					
					fseek(fp, meta[i]->tell, SEEK_SET);

					fread(buf, 1, meta[i]->size, fp);

					new = fopen(meta[i]->name, "wb");

					fwrite(buf, 1, meta[i]->size, new);

					free(buf);

					fclose(new);					

					for( j = 0; j < 10; j++)
					args[j] = malloc( 32);
	
					strcpy(args[1], "-d");
					strcpy(args[2], meta[i]->name);
					args[3] = NULL;

					if( (pid  = fork()) == 0)
					{
						execvp("comp",args); 
				
						perror("exec failure");
					}
		
					pid = wait(&status);

					
						
				}
				
				else
				{
					new = fopen(meta[i]->name, "wb");

					if( new == NULL)
					print_error(2);

					fwrite(buf, 1, meta[i]->size, new);

					fclose(new);
				}
			}
		}

		//free(buf);
			
	}

	
	for( i = 0; i < head->num_files; i++)
	free(meta[i]);

	free(meta);

	fclose(fp);
	
	free(head);
	

}

void extract_spe(const char * baya, const char * file)
{
	struct metadata ** meta;
	struct header * head;
	FILE * fp;
	FILE * new;
	char * buf;
	int i,count,j,flag,status, pid;
	char* args[10];
	char* new_name;
	char cwd[200];
	char cwd2[200];
	struct stat info;

	head = malloc(sizeof(struct header));

	if(( fp = fopen(baya, "rb+")) == NULL )
	print_error(2);

	fseek(fp, 0, SEEK_SET);
	
	fread(head, 1, sizeof(struct header), fp);

	/*pinakas opou tha krataw ola ta metadata*/
	meta = malloc( head->num_files* sizeof(struct metadata));

	for( i = 0; i < head->num_files; i++)
	meta[i] = malloc(sizeof(struct metadata));

	 /*phgainw ekei ppou arxizei to leksiko */
         fseek(fp, head->dict, SEEK_SET);
 
	/*diavazw ta metadata ston pinaka*/
	for( i  = 0; i < head-> num_files; i++)
	fread(meta[i], 1, sizeof(struct metadata), fp);	

	
	for( i = 0; i < head->num_files; i++)
	{
		

		 //printf("%s\n", meta[i]->path);

		if( meta[i]->is_dir != 1)
		{
			/*an to arxeio exei dwthei apo path*/
			if( (it_is_a_path(meta[i]->path)) == 1)
			{
				if(strcmp(meta[i]->root, file) == 0)
				{
					strcpy(ini, meta[i]->root);
	
					count = analyze_path(meta[i]->path);
				
					/*krataw to directory sto opoio vriskomai, gia na epistrepsw meta*/
					getcwd(cwd, sizeof(cwd));
			
					/*phgainw ston katallilo katalogo opou tha topothetithei to arxeio*/
					for( j = 0; j < count ; j++)
					ex_dir(j);

					getcwd(cwd2, sizeof(cwd2));			

				/*an to arxeio exei sybiestei*/
				if(meta[i]->compress == 1)
				{
					/*phgainw sto dir pou vrisketai to archive baya*/
					chdir(cwd);
					
					/*diavazw to arxeio pou thelw apo to baya*/
					buf = malloc(meta[i]->size);

					fseek(fp, meta[i]->tell, SEEK_SET);

					fread(buf,1, meta[i]->size, fp);
			
					/*dimiourgw se ayto to dir to antistoixo sybiesmeno arxeio pou tha aposybiestei*/
					new = fopen(meta[i]->name, "wb");

					fwrite(buf,1,meta[i]->size, new);
		
					free(buf);
			
					fclose(new);

					for( j = 0; j < 10; j++)
					args[j] = malloc( 32);
	
					strcpy(args[1], "-d");
					strcpy(args[2], meta[i]->name);
					args[3] = NULL;

					/*aposybiesi*/
					if( (pid  = fork()) == 0)
					{
						execvp("comp",args); 
				
						perror("exec failure");
					}
		
					pid = wait(&status);
		
	
					new_name = malloc(strlen(meta[i]->name)-1);
					
					strncpy(new_name, meta[i]->name, strlen(meta[i]->name) -1);

					new_name[strlen(meta[i]->name)-2]= '\0';
				
						printf("new name = %s\n", new_name);
	
			
					if(stat(new_name, &info)!= 0)
					print_error(7);
		
							
					buf = malloc(info.st_size);

					new = fopen(new_name,"rb+");

					fread(buf,1, info.st_size, new);

					fclose(new);
				
					chdir(cwd2);

					new = fopen(new_name, "wb");
					
					fwrite(buf,1,info.st_size,new);
			
					fclose(new);

					free(buf);
		
					chdir(cwd);

				}

				else
				{
					 /*dimiourgw to buffer sto opoio tha mpei to arxeio*/
 	         			buf = malloc(meta[i]->size);

					/*phgainw kai diavazw to arxeio */
					fseek(fp, meta[i]->tell, SEEK_SET);

					fread(buf, 1, meta[i]->size, fp);	
				
					/*dimiourgw to neo arxeio*/
					new =  fopen(meta[i]->name, "wb");

					if( new == NULL)
					print_error(2);

					fwrite(buf, 1, meta[i]->size, new);
		
					free(buf);

					fclose(new);	

					/*epistrefw sto directory pou vriskomai*/
					if( chdir(cwd) != 0)
					print_error(11);
				}
			    }
			}

			else if(strcmp(meta[i]->name, file) == 0)

			{
				if(meta[i]->compress == 1)
				{
					buf = malloc(meta[i]->size);
					printf("OOLAAA\n");
					fseek(fp, meta[i]->tell, SEEK_SET);

					fread(buf, 1, meta[i]->size, fp);

					new = fopen(meta[i]->name, "wb");

					fwrite(buf, 1, meta[i]->size, new);

					free(buf);

					fclose(new);					

					for( j = 0; j < 10; j++)
					args[j] = malloc( 32);
	
					strcpy(args[1], "-d");
					strcpy(args[2], meta[i]->name);
					args[3] = NULL;

					if( (pid  = fork()) == 0)
					{
						execvp("comp",args); 
				
						perror("exec failure");
					}
		
					pid = wait(&status);

					flag = 0;	
						
				}

				else
				{
					buf = malloc(meta[i]->size);
					
					fseek(fp, meta[i]->tell, SEEK_SET);

					fread(buf, 1, meta[i]->size, fp);

					new = fopen(meta[i]->name, "wb");

					if( new == NULL)
					print_error(2);

					fwrite(buf, 1, meta[i]->size, new);
	
					free(buf);
	
					fclose(new);
					flag = 0;
				}
			}
		
		}

		//free(buf);
		
		/*an thelame na eksagoume mono ena arxeio stamatame to psaksimo*/
		if(flag == 0)
		break;
			
	}

	
	for( i = 0; i < head->num_files; i++)
	free(meta[i]);

	free(meta);

	fclose(fp);
	
	free(head);
}

void show_meta(const char* baya)
{

	struct metadata ** meta;
	struct header * head;
	FILE * fp;
	int i;

	head = malloc(sizeof(struct header));

	if(( fp = fopen(baya, "rb+")) == NULL )
	print_error(2);
	
	fseek(fp, 0, SEEK_SET);
	
	fread(head, 1, sizeof(struct header), fp);

	/*pinakas opou tha krataw ola ta metadata*/
	meta = malloc( head->num_files* sizeof(struct metadata));

	for( i = 0; i < head->num_files; i++)
	meta[i] = malloc(sizeof(struct metadata));

	 /*phgainw ekei ppou arxizei to leksiko */
         fseek(fp, head->dict, SEEK_SET);
 
	/*diavazw ta metadata ston pinaka*/
	for( i  = 0; i < head-> num_files; i++)
	fread(meta[i], 1, sizeof(struct metadata), fp);	

	for( i = 0; i < head-> num_files; i++)
	printf("name :  %s\t uid :  %d\t gid :  %d\t modes: %08o \n", meta[i]->name, meta[i]->uid, meta[i]->gid); 
	


	for( i = 0; i < head->num_files; i++)
	free(meta[i]);

	free(meta);

	fclose(fp);
	
	free(head);

}




int main (int argc, char* argv[])
{
	struct stat info;

	int  i,pid, count, arg;
	char filename[20], filename2[20];;
	FILE* fp;
	char *args[10];
	struct header* head;
	char cwd[100];
	char cwd2[100];
	char cwd3[200];
	const char* full;
	char *pch;
	int comp;

	head = malloc(sizeof(struct header));
	
	 if(getcwd(cwd, sizeof(cwd)) == NULL)
        print_error(12);

	
	if( (strcmp (argv[1], "-j")) == 0 )
	{
		printf("compress\n");

		for( i = 0; i < 10; i ++)
		args[i] = malloc( 32);
	
		strcpy(args[1], argv[3]);
		args[2] =  NULL;
	
		
		if( (pid  = fork()) == 0)
		{
			execvp("comp",args); 
				
			perror("exec failure");
		}
		

		sprintf(filename2, "%s.Z", argv[3]);
		strcpy(filename, argv[2]);
		
	}
		
	/*to arxeio baya den yparxei, prepei na dimiourgithei*/
	if(( strcmp(argv[1] , "-c")) == 0)
	{
		
		if((strcmp( argv[2], "-j")) == 0)
		{
			comp = 1;
			arg = 1;
		}

		else
		{
			comp = 0;
			arg = 0;
		}

		 if((fp =  fopen(argv[2+arg], "wb+")) == NULL)
                 {
                         print_error(6);

                 }
                
		 head->num_files  = 0;

                 /*efoson den yparxei akoma kapoio arxeio sto baya , i thesi tou dictionary
		  *einai thewrhtika amesws meta to header
  		  */
		 fseek(fp, sizeof(struct header), SEEK_SET);
		 head->dict = ftell(fp);

		//printf("dict begins at = %d\n", head->dict);
		
		 fseek(fp, 0, SEEK_SET);
                 fwrite(head, 1, sizeof(struct header), fp);

		fclose(fp);

	

		/*an dinetai path*/
		if( it_is_a_path(argv[3+arg]) == 1)
        	{	
			if(argv[3+arg][0] == '.')
			{
				if(argv[3+arg][1] == '.')
				{
					chdir("..");

					if(getcwd(cwd2, sizeof(cwd2)) == NULL)
					perror("getcwd failed");
			
					pch = strtok(argv[3+arg],"." );

					strcat(cwd2, pch);
		
					printf("%s\n", cwd2);

					chdir(cwd);

					insert_path(cwd2,argv[2+arg],cwd, comp );

				}
				else
				{

					printf("einai relative path\n");
			
					if(getcwd(cwd2, sizeof(cwd2)) == NULL)
					perror("getcwd failed");
			
					pch = strtok(argv[3+arg],"." );

					strcat(cwd2, pch);
	
					printf("%s\n", cwd2);

					insert_path(cwd2, argv[2+arg],cwd, comp );
				}
		
			}

			/*einai apolyto monopati*/
			else	
			insert_path(argv[3+arg],argv[2+arg],  cwd, comp);
		}

		/*an dinetai apla to directory*/
		else if( it_is_a_path(argv[3+arg]) == 0 )
		{

			if(stat(argv[3+arg], &info) != 0 )
         		print_error(7);

         		else
         		{
                 		if(S_ISDIR(info.st_mode) != 0)
                 		{
					if(getcwd(cwd, sizeof(cwd)) == NULL)
                          		print_error(12);

                         		printf("file is a directory\n");
					strcpy(ini, argv[3+arg]);
                         		insert_dir_file(argv[3+arg], argv[2+arg],cwd, comp);

                		}
         			else
					insert_file(argv[2+arg], argv[3+arg], comp);
			}
		}
	}


	/*to arxeio baya yparxei idi*/
	if(( strcmp(argv[1], "-a")) == 0)
	{

		printf("FILE EXISTS\n");

		if((strcmp( argv[2], "-j")) == 0)
		{
			comp = 1;
			arg = 1;
		}

		else
		{
			comp = 0;
			arg = 0;
		}



		if( it_is_a_path(argv[3+arg]) == 1)
	  	{
			if(argv[3+arg][0] == '.')
			{
				if(argv[3+arg][1] == '.')
				{
					chdir("..");

					if(getcwd(cwd2, sizeof(cwd2)) == NULL)
					perror("getcwd failed");
			
					pch = strtok(argv[3+arg],"." );

					strcat(cwd2, pch);
		
					printf("%s\n", cwd2);

					chdir(cwd);

					insert_path(cwd2 ,argv[2+arg],cwd, comp );

				}
				else
				{
					printf("einai relative path\n");
			
					if(getcwd(cwd2, sizeof(cwd2)) == NULL)
					perror("getcwd failed");
			
					pch = strtok(argv[3+arg],"." );

					strcat(cwd2, pch);
	
					printf("%s\n", cwd2);
					

					
					insert_path(cwd2,argv[2+arg],cwd, comp );
				}
			}
			else
			insert_path(argv[3+arg],argv[2+arg],  cwd, comp);
		}

		else if( it_is_a_path(argv[3+arg]) == 0 )
		{
			 if(stat(argv[3+arg], &info) != 0 )
                         print_error(7);

                        else
                         {
				 /*an prokeitai gia directory kalw tin analogi synartisi*/
				 if(S_ISDIR(info.st_mode) != 0)
                                 {
                                         if(getcwd(cwd, sizeof(cwd)) == NULL)
                                         print_error(12);

                                         printf("file is a directory\n");
					 strcpy(ini, argv[3+arg]);
                                         insert_dir_file(argv[3+arg], argv[2+arg],cwd, comp);

                                 }
                                 else
					/*an einai aplo file kalw alli synartisi*/
                                         insert_file(argv[2+arg], argv[3+arg], comp);
                         }

			
			
		}
	}

	if((strcmp(argv[1], "-p")) == 0)
	{
		ierarchy(argv[2]);	

	}

	/*eksagwgi arxeiwn */
	if((strcmp(argv[1], "-x")) == 0)
	{
		if( argv[3] == NULL)
		extract(argv[2]);

		else 
		extract_spe(argv[2], argv[3]);

	}

	if( strcmp(argv[1], "-m") == 0)
	{

		show_meta(argv[2]);

	}
	
	



	printf("OLE\n");


	return 0;
}
