#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int count;

void signalHandler(int sig)
{
	if(sig == SIGUSR1)
	{
		++count;
		signal(SIGUSR1, signalHandler);
	}


}

int main( int argc, char *argv[])
{
	FILE* pgm;
	FILE* input_file;
	int k ;
	int **pixels;
	int x,y;
	int maxval;
	char c;
	char input[20], output[20],xs[5], ys[5],max[5];
	int i,j, lines_numb, lines_numb2;
	int counter;
	int t;
	char * args[10];
	char **args2;
	int cpid, status;
	int *pids;

	counter  = 0;

	
/*arxeio pgm me tin eikona pou tha ginei smoothed
 */ 
	pgm = fopen(argv[3],"r"); 
	
	if( pgm == NULL)
	{
		perror("could ont open file");
		exit(-1);
	}

	do
	{
		c = fgetc(pgm);
	}
	while(c != '\n');

/* x= arithmos sthlwn
 */
	fscanf(pgm, "%d ", &x);

/* y = arithmos grammwn
 */
	fscanf(pgm, "%d", &y);
	
	
	fscanf(pgm, "%d", &maxval);

/*deseyetai dynamika o pinakas ston opoio tha topothetithei arxika i eikona pros
 * epeksergasia.
 */
	pixels = malloc(y*sizeof(int *)); 
	if( pixels == NULL)
	{
		perror("could not allocate memory");
		exit(-1);
	}
	
	for( i = 0; i < y; i++)
	{
		pixels[i] = malloc( x *sizeof(int));
		if(pixels[i] == NULL)
		{
			perror("could not allocate memory");
			exit(-1);

		}
	}


/*arithmos twn workers pou tha dimiourgithoun
 */
	k = atoi(argv[2]);

/* ypologizoume poses grammes tha periexei kathe arxeio pou tha epeksergastei o kathe worker
 */

	
	if(k >= y)
	{
		lines_numb = 1;
		lines_numb2 = 0;
	}
	
	else if(y > k )
	{

/*an o arithmos twn grammwn einai megalyteros apo ton arithmo twn workers kai diareitai akrivws
 * me ton arithmo twn workers, kathe worker pairnei lines_numb grammes
 */
		if ((y%k) == 0)
		lines_numb = y/k;
		
/*an o arithmos twn grammwn einai megalyteros apo ton arithmo twn workers alla i diairesi grammes/workers exei ypoloipo,
 * o prwtos worker tha parei (grammes/workers) grammes + to ypoloipo = lines_numb2
 */
		else if ((y%k) != 0)
		{
			lines_numb = y/k;
			lines_numb2 = lines_numb + (y%k);

		}
		


	}
	 

/*pernaw ola ta pixels tin eikonas se enan pinaka gia na ginei o diaxwrismos sta epimerous arxeia
 */
	for( i =0; i < y; i++)
	  for( j =0; j <x; j++) 
		fscanf(pgm, "%d", &pixels[i][j]);




/*diaxwrismos sta epimerous arxeia
 */	

/*an o arithmos twn workers einai megalyteros apo ton arithmo twn grammwn, tote kathe worker pairnei mono
 * mia grammi kai dimiourgountai tosoi workers oses einai oi grammes:
 */
	if(k > y)
	k = y;
	
	for(i =0; i < k; i++)
	{
		sprintf(input, "input_file%d.txt", i);
		input_file = fopen(input, "w");
		
		if(input_file == NULL)
		{
			perror("R:could not open file");
			exit(-1);
		}

/*an yparxei ypoloipo apo ti diairesi grammes/workers , o prwtos 
 * worker pairnei ton arithmo twn grammwn pou prepei+to ypoloipo
 */
		if (((y%k)!= 0) && ( i == 0 ) && (k<y))
		{
		
				for(t = counter; t < lines_numb2; t++)
			 	{ 
					for( j = 0; j < x ; j++)
					{
						fprintf(input_file, "%d ", pixels[t][j]); 
					}
					fprintf(input_file, "\n");
					counter++;
				}

		}
			
		else 
		{	
			
				for(t = 0; t < lines_numb; t++)
		  		{
					 for(j = 0; j < x; j++)
					{	 
						fprintf(input_file, "%d ", pixels[counter][j]);
					}
					fprintf(input_file, "\n");	
					counter++;
				}
		}
		
		fclose ( input_file );
	}

/*desmeyw xwro gia ta orismata twn workers
 */
	for ( i=0 ; i<10 ; i++ )
		args[i] = malloc ( 32 );
	
	pids = malloc(k*sizeof(int));
 
	signal(SIGUSR1, signalHandler);		


/*dimourgia diergasiwn Wi me xrisi fork() kai execvp
 */
	for(i = 0 ; i < k; i++)
	{

		sprintf(input, "input_file%d.txt", i);
		sprintf(output, "output_file%d.txt", i );

/*perniountai ta orismata tou programmatos gia to smoothing pou tha xrhsimopoihsoune
 * oi diergasies worker.
 */		

/*allazoun ta orismata pou pairnei kathe worker mew tis exec analoga 
 * me ton arithmo twn gramwnpoyu tha epeksergastei enas worker
 */
		if(((y%k)!= 0) && (i ==0))
		{
			sprintf(ys, "%d", lines_numb2);
			sprintf(xs, "%d", x);
		}
		
		else 
		{
			sprintf(ys, "%d", lines_numb);
			sprintf(xs, "%d", x);

		}
	
		strcpy ( args[0], argv[1] );
		strcpy ( args[1], ys );
		strcpy ( args[2], xs );
		strcpy ( args[3], input );
		strcpy ( args[4], output );
		strcpy ( args[5], argv[5] );
		args[6] = NULL;



		switch(pids[i] = fork())
		{
			case 0: execvp(args[0], args);
			
				perror("exec failure");
				exit(-1);
	
			case 1: perror("fork error");
				break;

 

		}  
			
	}
	
	for( i = 0; i <k; i++)
	wait(&pids[i]);

/*diergasia M
 */


/*dimiourgia tou pinaka twn orismatwn tis diergasias M
 */
	args2 = malloc ((k+6)*sizeof(char*));
	if(args2 == NULL)
	{
		perror("could not allocate memory");
		exit(-1);
	}

	for(i = 0; i < (k+6); i++)
	{
		 args2[i] = malloc(32);
		
		if(args2[i] == NULL)
		{
			perror("could not allocate memroy");
			exit(-1);
		}
	}

	sprintf(ys, "%d", y);
	sprintf(xs, "%d", x);
	sprintf(max, "%d", maxval);

	strcpy(args2[0], "M");
	strcpy(args2[1], xs);
	strcpy(args2[2], ys);
	strcpy(args2[3], max);
	strcpy(args2[k+4], argv[4]);

	for(i = 0; i<k;i++)
	{
		sprintf(output, "output_file%d.txt", i);
		strcpy(args2[4+i], output);
	}
	
	args2[k+5] = NULL;

	signal(SIGUSR1, signalHandler);

/*dimiourgia diergasias M
 */
	switch(cpid = fork())
	{
		case 0: execvp(args2[0],args2);
			
			perror("exec failure");
			exit(-1);
		case 1: perror("fork error");
			break;

	}
	cpid  = wait(&status);


	printf("i am father . %.2f%% of my children sent me a signal\n", (count/(double)(k+1))*100);
	for(i =0; i <10; i++)
		free(args[i]);

	for(i =0; i <(k+6); i++)
	free(args2[i]);

	free(args2);

/*diergasia eog gia tin emfanisi tis eikonas pgm
 */
 
 
        execl("/usr/bin/eog","eog",argv[4], (char *)NULL );
        perror("exec failure");
        exit(-1);

	

}
