#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int main( int argc, char* argv[])
{
	int x,y,counter,i,j,t;
	int maxval;
	FILE* input;
	FILE* output;
	char fp[30], im[4];
	int **pixels;
	int k,lines_numb, lines_numb2;
	int **temp;

	counter  = 0;
/*arithmos stilwn
 */
	x = atoi(argv[1]);
/*arithmos grammwn
 */
	y = atoi(argv[2]);

	maxval = atoi(argv[3]);
	
	k = (argc - 5);

/*ypologizw poses grammes tha diavastoun apo ta outputs
 */
	
	if(k > y)
	{
		lines_numb = 1;
		lines_numb2 = 0;
	}
	
	else if(y > k )
	{


		if ((y%k) == 0)
		lines_numb = y/k;
		

		else if ((y%k) != 0)
		{
			lines_numb = y/k;
			lines_numb2 = lines_numb + (y%k);

		}
		


	}
	
/*desmeyw xwro gia enan pinaka ston opoio tha mpoun arxika ta meri apo ta outputs
 * o opoios meta tha perastei sto teliko arxeio
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
/*vazw ta stoixeia twn outputs se ena pinaka ton opoio meta tha ektypwsw sto eniaio output
 */
	
	
	for(i = 4; i <= (argc-2); i++)
	{
		strcpy(fp,argv[i]);
		
		input =  fopen(fp, "r");
		if(input == NULL)
		{
			perror("could not oppen file");
			exit(-1);
		}
		
/*periptwsi an to prwto arxeio output exei diaforetiko arithmo grammwn apo ta alla
 */
		 if (((y%k)!= 0) && ( i == 4 ))
		 {
			temp = malloc(lines_numb2*sizeof(int*));
			if(temp == NULL)
			{
				perror("could not allocate memory");
				exit(-1);

			}
			
			for(j = 0; j < lines_numb2; j++)
			{
				temp[j] = malloc(x*sizeof(int));
				if(temp[j] == NULL)
				{
					perror("could not allocate memory");
					exit(-1);

				}

			}

			for(j = 0; j<lines_numb2; j++)
				for( t = 0; t < x; t++)
					fscanf(input, "%d", &temp[j][t]);
			
			for(j = 0; j < lines_numb2; j++)
			{
				for(t = 0 ; t < x ; t++)
				{
					pixels[j][t] = temp[j][t];
				}
				counter++;

			}

			for(j = 0; j <lines_numb2; j++)
			free(temp[j]);
		
			free(temp);

		 }
		
/*periptwsi pou ola ta arxeia exoun idio arithmo grammwn
 */
		else
		{
			temp = malloc(lines_numb*sizeof(int*));
			if(temp == NULL)
			{
				perror("could not allocate memory");
				exit(-1);
			}
			for(j =0; j < lines_numb; j++)
			{
				temp[j] = malloc(x*sizeof(int));
				if(temp[j] == NULL)
				{
					perror("could not allocate memory");
					exit(-1);
				}
			}
			for(j =0; j < lines_numb; j++)
			   for(t = 0; t < x; t++)
				fscanf(input, "%d", &temp[j][t]);
			
			for(j = 0; j < lines_numb; j++)
			{
				for( t = 0; t < x; t++)
				{
					pixels[counter][t] = temp[j][t];
				}
				counter++;
			}
	
			for( j = 0; j < lines_numb; j++)
			free(temp[j]);
			
			free(temp);
		}
		fclose(input);
		if( remove(fp) != 0)
		perror("error removing file");
		
	}


/*metaferoume olokliro ton pinaka pixels sto teliko arxeio
 */
	strcpy(fp,argv[argc-1]);
	
	output = fopen(fp, "w");
	
	if(output == NULL)
	{
		perror("could not open file");
		exit(-1);
	} 

	strcpy(im, "P2");

	fprintf(output, "%s\n%d %d\n%d\n", im, x,y,maxval);
	
	for(i =0; i < y; i++)
	  {
		for( j =0 ; j < x; j++)
		{
			fprintf(output, "%d ", pixels[i][j]);
	
		}
		fprintf(output, "\n");
		
	}
	fclose(output);

	for(i = 0; i < y; i++)
	free(pixels[i]);
	
	free(pixels);
	

	return 0;
}
