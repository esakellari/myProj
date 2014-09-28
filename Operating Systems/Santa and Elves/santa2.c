#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <signal.h>


/*domi dwro*/
typedef struct gift{

	char cont[10];
	char presen[70];

	}gift;

/*domi kados*/
typedef struct bucket{

	struct gift *array;
	int stable, size,occ, num_of_elfs;
	int *elf_id;
	int temp_pres;
	
	sem_t mutex_b;	

	}bucket; 

/*domi sakos*/
typedef struct sack{

	struct gift *arr;
	sem_t mutex_s;
	int counter;
	
	}sack;

void free_rsc(int id)
{
	shmctl(id, IPC_RMID, NULL);

}

sem_t mutex;

/*synartisi pou dinetai ws orisma stin qsort kai i opoia kanei tin
 *sygrisi gia tis sumvoloseires-dwra
 */
int compare (struct gift *a, struct gift *b)
{
  return (  strcmp( a->presen, b->presen )) ;
}

/*synartisi pou metraei posa dwra periexei to arxeio */
int lines(char* input)
{
	int count;
	FILE *fp;
	char line[100];

	count  = 0;
	
	fp = fopen(input, "r");
	
	if( fp == NULL)
	{
		perror("error opening file");
		exit(-1);
	}
	
	while(!feof(fp))
	{
		fgets(line, 100, fp);
		count++;
	}
	
	fclose(fp);
	
	return count;
}


int main(int argc, char**argv)
{
	FILE* fp;	
	int buckets;
	int id1,id2,id3,id4,id5,id6,begin, end, dur, presents_max;
	int *pid;
	int* time_count;
	int* id;
	int elfs,counter,counter3, letters,pid2,status, ibuck,i,j,t,size1,b,p, empty_bucks,bmin,bmax, tmin, tmax, smin , smax, sleepb, sleept, sleeps;
	char c;
	int* count2;
	int* min;
	int *presents_counter;
	char continent[11];
	char input[20];
	char listEuGr[20];
	char listAfAm[20];
	char listAuAs[20];
	struct bucket* buck_arr;
	struct sack* sack_arr;

	srand(time(NULL));

	
	counter  = 0;
	/*metritis gia to posa dwra vazei kathe kswtiko*/
	counter3 = 0;

	/*parsing*/
	for( i =0; i <argc; i++)
	{
		if(strcmp(argv[i], "-m") == 0)
		buckets = atoi(argv[i+1]);
		
		if(strcmp(argv[i], "-n") ==0 )
		elfs = atoi(argv[i+1]);

		if(strcmp(argv[i], "-i") == 0 )
		sprintf(input, argv[i+1]);

		if(strcmp(argv[i] , "-d1") == 0)
		{
			bmin = atoi(argv[i+1]);
			bmax = atoi(argv[i+1]);
		}

		if(strcmp(argv[i] , "-d2") == 0 )
		{
			tmin = atoi(argv[i+1]);
			tmax = atoi(argv[i+2]);
		}
	
		if(strcmp(argv[i], "-d3") == 0)
		{
			smin = atoi(argv[i+1]);
			smax = atoi(argv[i+2]);
		}		

		if(strcmp(argv[i], "-L") == 0)
		{
			sprintf(listEuGr, argv[i+1]);
			sprintf(listAfAm, argv[i+2]);
			sprintf(listAuAs, argv[i+3]);
		}
		
	}

	/* orismata twn synartisewn sleep*/
	sleepb = rand()%bmin + bmax;
	sleept = rand()%tmin + tmax;
	sleeps = rand()%smin + smax;


	/* dimiourgia kai attach twn shared segments */
	id3 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);	
	
	 if(id3 == -1)
        {
                perror("shmget error");
                exit(-1);
        }


	id4 =  shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
	
	 if(id4 == -1)
        {
                perror("shmget error");
                exit(-1);
        }
	
	/*pinakas pou krataei ta dwra pou evale kathe kswtiko*/
	id5 = shmget(IPC_PRIVATE, elfs*sizeof(int), IPC_CREAT | 0666);
	
	 if(id5 == -1)
         {
                 perror("shmget error");
                 exit(-1);
         }

	id6 = shmget(IPC_PRIVATE, elfs*sizeof(int),IPC_CREAT | 0666);
	
	if(id6 == -1)
	{
		perror("shmget eroor");
		exit(-1);
	}

	
	count2 =(int*) shmat(id3, NULL, 0);
	min = (int*) shmat(id4, NULL, 0);
	
	/* pinakas pou krataei ta ids twn paidiwn-elf */ 
	pid = malloc(elfs*sizeof(int));
	id = malloc(elfs*sizeof(int));	

	/* shared segment buckets */
	id1 = shmget(IPC_PRIVATE,buckets*sizeof(struct bucket),IPC_CREAT |  0666);	

	if(id1 == -1)
	{
		perror("shmget error");
		exit(-1);
	}

	/* shared segment sacks */
	id2 = shmget(IPC_PRIVATE, 3*sizeof(struct sack), IPC_CREAT | 0666);

	if(id2 == -1)
	{
		perror("shgrt eroor");
		exit(-1);
	}		

	/*attach twn buckets*/
	buck_arr = shmat(id1, (void*)0, 0);

	if(buck_arr == NULL)
	{
		perror("shmat error");
		free_rsc(id1);
		exit(-1);
	}
	
	/* attach twn sacks */
	sack_arr = shmat(id2,(void*)0,0);

	if(sack_arr == NULL)
	{
		perror("shmat eroor");
		free_rsc(id2);
		exit(-1);
	}
	
	
	presents_counter  = shmat(id5, (void*)0,0);
	
	 if(presents_counter == NULL)
         {
                 perror("shmat eroor");
                 free_rsc(id5);
                 exit(-1);
         }

	time_count = shmat(id6, (void*)0,0);
	
	if(time_count == NULL)
	{
		perror("shmat error");
		free_rsc(id6);
		exit(-1);
	}
	for(i =0; i<buckets; i++)
	{
		buck_arr[i].size =0;
		buck_arr[i].occ = 0;
		buck_arr[i].num_of_elfs  =0;	
		buck_arr[i].temp_pres  =0;
		buck_arr[i].elf_id = malloc(elfs*sizeof(int));
	}

   	/*arxikopoihsh tou metriti twn sakwn pou deixnei
	 * posa dwra periexei o sakos
	 */
	for( i = 0; i < 3; i++)
	sack_arr[i].counter = 0;	

	/* vriskoume posa dwra max mporei na periexei enas kados*/
	presents_max  = lines(input);

	/*desmeyw dynamika ton pinaka twn dwrwn tou kathe kadou*/
	for( i = 0 ; i < buckets ; i++)
	buck_arr[i].array =  malloc(presents_max*sizeof(struct gift));

	/*desmeyw dynamika ton pinaka twn dwrwn toy kathe sakou*/
	for( i = 0; i < 3; i++)
	sack_arr[i].arr = malloc((presents_max/2)*sizeof(struct gift));
	
	/* diavasma tou arxeiou me ta dwra */	 
	fp = fopen(input, "r");
	

	while(!feof(fp))
	{
		letters  = 0;
		ibuck = rand()%(buckets);
		size1 = buck_arr[ibuck].size;
		fgets(buck_arr[ibuck].array[size1].presen, 100, fp);
		buck_arr[ibuck].size++;
 		buck_arr[ibuck].stable =  buck_arr[ibuck].size;
	}	

	fclose(fp);

	/* arxikopoihsh twn shmaforwn twn kadwn kai twn sakwn */
	for(i = 0; i< buckets; i++)
	sem_init(&(buck_arr[i].mutex_b), 1, 1);

	for( i = 0; i < 3; i++)
	sem_init(&(sack_arr[i].mutex_s), 1,1);
	
	sem_init(&mutex, 1,1);


	for(i =0 ; i< buckets; i++)
	printf("bucket No# %d contains %d gifts\n", i, buck_arr[i].size);
	
	 
	*count2 = 0;	
	empty_bucks  = 0;

	/* diergasies elf */
	for(i = 0;i <elfs; i++)
	{
		
		printf("elf No# %d\n",i);
		if((pid[i] = fork())== 0)
		{
		
		begin = time(NULL);		

		 id[i] = getpid();
		
	
	 	  for( j =0; j < buckets; j++)
		  {
			/*an vrikes diathesimo kado*/
			if( buck_arr[j].occ == 0)
			{ 
				printf("elf with id %d got bucket No# %d\n", id[i], j);
				
				
				/*ayksanetai o arithmos twn elfs pou douleyoun se ayton ton kado*/
				buck_arr[j].num_of_elfs++;
				
				/*o kados krataei ta ids twn kswtikwn pou douleyoun se ayton*/
				buck_arr[j].elf_id[0] = id[i];
			
				/* i timi min pou deixnei posa kswtika exei o kados me ta ligotera allazei */
				*min  = buck_arr[j].num_of_elfs;
	
				/* o sygekrimenos kados ginetai kateilhmenos*/
				buck_arr[j].occ = 1;
				break;
			}
			
		 }	
			/*an den yparxei kanenas kados diathesimos psakse ton kado me ta 
                         *ligotera kswtika
 			 */
	

		if(buck_arr[buckets-1].num_of_elfs != 0)
		{
			
		   	for( j = 0; j < buckets ; j++)
	 	   	{
		   		
				if(buck_arr[j].num_of_elfs == (*min) )
				{	
				 	printf("elf with id %d got bucket No# %d\n", id[i], j);
					
					sem_wait(&mutex);
					b  = buck_arr[j].num_of_elfs;
					buck_arr[j].elf_id[b]=  id[i];
					buck_arr[j].num_of_elfs++;

					sem_post(&mutex);
					/* an exoume ftasei ston teleytaio kado ayksanetai kata 1 i timi 
					 * tis metavlitis min
					 */
					if( j == (buckets-1))
					{
					sem_wait(&mutex);
					(*min)++;
					sem_post(&mutex);

					}
					break;
			
				}
		    

		   	}
		}
				
		
	  	 

		
			
		/* oso den exoun adeiasei oloi oi kadoi */	
		while(1)
		{
               /*psaxnoume na vroume se poion kado anhkei to kswtiko*/
			
			for( t = 0; t < buckets; t++)
			{
				for( j = 0; j < elfs;j++)
			   	{
					if(buck_arr[t].elf_id[j] == id[i])
					{
						b = t;
						break;
					}
			   	}	
			  	
			}
			
                   	/* oso den exei adeiasei o sykekrimenos kados */         
			while(1)
			{

						
				 j = buck_arr[b].temp_pres;
				
				/* CS: piasimo dwrou apo kado */
		  	 	sem_wait(&(buck_arr[b].mutex_b));
			
				printf("Elf No# %d works on bucket %d\n", i, b);
				usleep(sleepb);
			
				for( t = 0; t < 15; t++)
				{
					if(  buck_arr[b].array[j].presen[t] == ',')
					break;
				}
			
				/*vazoume tin hpeiro mono se ena string gia ti sygrisi */
				strncpy(continent, buck_arr[b].array[j].presen,t);
				continent[t] = '\0';
				
				/*ayksanoume ton metriti pou deixnei se poio dwro tou kadou eimaste */
				buck_arr[b].temp_pres++;
				

				sem_post(&(buck_arr[b].mutex_b));				

				usleep(sleept);

				/*an anhkei ston prwto sako*/
				if((strcmp(continent, "Europe")== 0) || ( strcmp(continent, "Greenland")== 0 ))
				{	
					/*CS : metafora dwrou apo kado se sako */
					sem_wait(&(sack_arr[0].mutex_s));

					printf("Elf No#%d puts a present in the first sack\n", i);
					usleep(sleeps);

				
					counter  = sack_arr[0].counter;
					strcpy(sack_arr[0].arr[counter].presen, buck_arr[b].array[j].presen);
				
					sack_arr[0].counter++;
					(*count2)++;
					buck_arr[b].size--;
					counter3++;
					
					sem_post(&(sack_arr[0].mutex_s));
					
				}
				
				else if((strcmp(continent, "Africa")== 0 ) || (strcmp(continent, "America")== 0 ))
				{
					/*CS: metafora dwrou apo kado se sako */
					sem_wait(&(sack_arr[1].mutex_s));
			
					printf("Elf No#%d puts a present in the second sack\n", i);
					usleep(sleeps);
			
					counter = sack_arr[1].counter;
					strcpy(sack_arr[1].arr[counter].presen, buck_arr[b].array[j].presen);
					sack_arr[1].counter++;
					(*count2)++;
					buck_arr[b].size--;
					counter3++;

					sem_post(&(sack_arr[1].mutex_s));
					
				}
				
				else if((strcmp(continent, "Asia")== 0 ) || (strcmp(continent, "Australia") == 0 ))
				{
					/*CS: metafora dwrou apo kado se sako */
					sem_wait(&(sack_arr[2].mutex_s));
					printf("Elf No#%d puts a present in the third sack\n", i);
					usleep(sleeps);
				
					counter = sack_arr[2].counter;
					strcpy(sack_arr[2].arr[counter].presen, buck_arr[b].array[j].presen);
					sack_arr[2].counter++;
					(*count2)++;
					buck_arr[b].size--;
			
					counter3++;					
					sem_post(&(sack_arr[2].mutex_s));
					
				}
				
			   	
				j++;
			
			
			/* an adeiase aytos o kados alla den exoun adeiasei oloi oi kadoi*/
			if(((buck_arr[b].size == 0) || (j >  buck_arr[b].stable)  ) && ((empty_bucks) < (buckets)))
			{
				
				/* ayksanetai o arithmos twn adeiwn kadwn kata 1 */
			        empty_bucks++;	
			
		        					
				/* svinoume to id aytou tou elf apo ton pinaka twn ids twn elf tou kadou*/
          			for( t = 0; t < buck_arr[b].num_of_elfs; t++)
				{
					if(buck_arr[b].elf_id[t] == id[i])
					buck_arr[b].elf_id[t] = 0;
		   		}					
				
				/*arnhtikos arithmos twn elfs pou douleyeoun ekei etsi wste stin
 				* sygrisi me to min se mellontiko elegxo na min paei kapoio elf se ayto ton kado
 				*/	
				 					
				sem_wait(&buck_arr[b].mutex_b);
				buck_arr[b].num_of_elfs = -1;
				sem_post(&buck_arr[b].mutex_b);
	
				/*psaxnoume na vroume ton kado me ta ligotera kswtika gia na  paei to elf */
        			for( t = 0; t < buckets ; t++)
  		    		{
					if(buck_arr[t].size > 0)
					{
						if(buck_arr[t].num_of_elfs == (*min) )
   			       			{
							printf("Elf No# %d with id %d got bucket No# %d\n", i, id[i], t);
							
							sem_wait(&(buck_arr[t].mutex_b));
                                 			p = buck_arr[t].num_of_elfs;
                                 			buck_arr[t].elf_id[p]=  id[i];
                              	 			buck_arr[t].num_of_elfs++;
							sem_post(&(buck_arr[t].mutex_b));
                                 	

                                 			break;
                       				}
					}
            			}	
                 		
        		}
			/*an exoun adeiasei oloi oi kadoi, termatizei to kswtiko*/
			if( empty_bucks >= buckets )
			{
				end = time(NULL);
				dur  = end - begin;

				printf("Terminating elf No#%d\n", i);	

				presents_counter[i] = counter3;
				time_count[i] = dur;

				return 0;
			}

			
		   }/* while(1)*/
			
        	}/* while(1) */	

       	}/* if((pid[i] = fork())== 0) */
	

	
	}/*  for(i = 0;i <elfs; i++)  */
		
	
         

	/*diergasia pou kanei taksinomisi*/
	if((pid2 =fork())== 0)
	{
		printf("Starting classification...\n");
		for( i = 0; i < 3; i++)
		{
			qsort(sack_arr[i].arr, sack_arr[i].counter,sizeof(struct gift), compare); 
		}
		printf("The presents have been classified...\n");
		
	} 
  	
	 for(i =0; i<elfs; i++)
         wait(&(pid[i]));

	wait(&pid2); 	
	
	printf(" \n%d presents in the sacks\n", *count2);

	/* grapsimo twn periexomenwn twn sakwn sta arxeia*/

	for( i = 0; i < 3; i++)
	printf("Sack No#%d contains %d presents\n", i, sack_arr[i].counter);

	printf("\nStatistics..\n");

	for( i = 0; i < elfs ; i++)
	{
		printf("Elf No#%d put %d presents in the sacks\n", i, presents_counter[i]);
		printf("Elf No#%d worked for %d time units\n",i, time_count[i]);
	}
	
	fp = fopen(listEuGr, "w");
	for(i = 0; i < sack_arr[0].counter; i++)
       	fprintf(fp, "%s ", sack_arr[0].arr[i].presen);
	fclose(fp);

	fp = fopen(listAfAm, "w");
	for(i = 0; i < sack_arr[1].counter; i++)
	fprintf(fp, "%s", sack_arr[1].arr[i].presen);
	fclose(fp);

	fp  = fopen(listAuAs, "w");
        for(i = 0; i < sack_arr[2].counter; i++)
        fprintf(fp, "%s", sack_arr[2].arr[i].presen);
	fclose(fp);

	for( i =0; i <buckets; i++)
	sem_destroy(&(buck_arr[i].mutex_b));


	for( i = 0; i < 3; i++)
	sem_destroy(&(sack_arr[i].mutex_s));

	sem_destroy(&mutex);	
	
	free_rsc(id2);
        free_rsc(id1);
	free_rsc(id3); 
	free_rsc(id5);
	free_rsc(id6);
	free_rsc(id4);

	return 0;
}
