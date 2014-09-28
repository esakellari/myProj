#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

/*ayto to arxeio dexetai san eisodo ena arxeio me ena kommati tis eikonas pgm kai allazei tis
 * times twn pixel tis eikonas
 */

int main(int argc, char *argv[])
{	

	double f[3][3];
	int temp[3][3];
	int t, rr;
	float sum;
	FILE* filter;
	FILE* input;
	FILE* output;
	int i,j,k,m;
	int **pixels;


/*arithmos grammwn
 *  */
  		
	k = atoi(argv[1]);
/*arithmos sthlwn
 */
	m = atoi(argv[2]);

	pixels = malloc ( k*sizeof(int *));
	for ( i=0 ; i<k ; i++ )
		pixels[i] = malloc (m*sizeof(int));
	
	input = fopen( argv[3], "r+");
 
	filter =  fopen(argv[5], "r");

	if(filter == NULL)
	{	
		perror("smooth:could not open file");
		exit(-1);
	}
	else
	{
/*diavazoume to filtro apo to  arxeio kai to apothikeyoume ston pinaka f
 */
		for(i = 0; i < 3; i++)
		  for( j = 0;  j < 3 ; j++)
			fscanf(filter, "%lf", &f[i][j]);
	}





	if(input == NULL)
	{
		perror("smooth2:could not open file");
		exit(-1);
	}

	else
	{
		for( i = 0 ;  i < k;  i++)
			for(j = 0 ; j < m ; j ++)
				fscanf(input, "%d", &pixels[i][j]);
	}




	output =  fopen(argv[4], "w");

	if( output == NULL)
	{
		perror("could not open file");
		exit(-1);

	}
	

else{
//allagi tis timis twn pixels 
	for( i = 0 ; i < k ; i++)
      	   {for(j = 0 ; j < m ; j++)
	     	{
/*arxika tompotheteitai to kommati pou tha epeksergastoume, dld to pixel pou eimaste kai ta geitonika tou,
 * se enan pinaka,, o opoios allazei times analoga me to filtro.
 * O pinakas temp antistoixei sta stoixeia pou vriskontai gyrw apo to pixel pou theloume. An kapoio geitonika
 * pixel tou pixel pou theloume na allaksoume timi einai ektos pinaka, tote to antistoixo stoixeio tou pinaka temp
 * pairnei tin timi tou kentrikou pixel.
 */
			temp[1][1] = pixels[i][j];

			if(((i-1) < 0) || ((j-1)<0))
				temp[0][0] = pixels[i][j];	
			else
				temp[0][0] = pixels[i-1][j-1];
			
			if(((i-1) < 0))
				temp[0][1] = pixels[i][j];	
			else
				temp[0][1] = pixels[i-1][j];

			if(((i-1) < 0) || ((j+1)>=m))
				temp[0][2] = pixels[i][j];	
			else
				temp[0][2] = pixels[i-1][j+1];
			
			if((j-1)<0)
				temp[1][0] = pixels[i][j];	
			else
				temp[1][0] = pixels[i][j-1];
		

			if( (j+1) >= m )
				temp[1][2] = pixels[i][j];	
			else
				temp[1][2] = pixels[i][j+1];

			if(((i+1) >= k ) || ((j-1)<0))
				temp[2][0] = pixels[i][j];	
			else
				temp[2][0] = pixels[i+1][j-1];

			if((i+1) >= k)
				temp[2][1] = pixels[i][j];	
			else
				temp[2][1] = pixels[i+1][j];

			if(((i+1) >= k) || ((j+1)>= m))
				temp[2][2] = pixels[i][j];	
			else
				temp[2][2] = pixels[i+1][j+1];
			
			sum = 0;

/* molis exoun topothetithei oles oi times ston pinaka temp, aytos pollaplasiazetai me ton pinaka
 * ston opoio yparxei to filtro kai to apotelesma apothikeyetai se mia metavliti sum, pou einai  i
 * nea timi tou pixel pou htan pros epeksergasia.
 */	
			for(t = 0; t < 3; t++)
			{for(rr = 0; rr< 3 ; rr++)
				{
					sum = sum + temp[t][rr]*f[t][rr];
				
				   
				}
			
			}
		fprintf(output, "%d " , (int )sum);
					
		}
		 fprintf(output,"\n");	

	}
}

	fclose(input);
	remove(argv[3]);
	fclose(output);
	fclose(filter);


	kill(getppid(), SIGUSR1);

	 exit(0);

}

