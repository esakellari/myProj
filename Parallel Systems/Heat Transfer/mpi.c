#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <string.h>


#define MASTER  0	/* taskid of first process */
#define NONE   -1   /* indicates no neighbor */
#define TRUE	1
#define FALSE	0
#define ERROR_CODE -1

int is_square_number(int number);

struct Parms {
	float cx;
	float cy;
} parms = { 0.1f, 0.1f };

struct Neighbours_Buffers{

	int left_recv;
	int left_send;

	int right_recv;
	int right_send;

}neighbours_counter = { 0, 1, 2, 3 };

typedef struct {
	int above;
	int below;
	int left;
	int right;

}Neighbours_Ids;

typedef struct {
	int current;
	int next;

}States;

typedef struct {
	int nrows;
	int side_size;
	int sub_array_size;
	int topology_side_size;
	int grid_size;
} Info;

/* the number of the processes*/
int numtasks;

int  temp_count;

/*	A struct which holds the important information
	such as the grid size, the sub_array_size,
	the number of rows of the subgrid etc.
	*/
Info info;

/*  Arrays of the statuses and requests, used for the MPI_Waitall functions
	for each procedure. They are 4x2 arrays, each row corresponds
	to the exchange of data with the above, below, left and right neighbours.
	*/
MPI_Status status[4][2];
MPI_Request request[4][2];

/*	This function operates the update calculation only to the inner
	cells of the subgrids, since these cells do not make use
	of the ghost cells of the subgrid.
	*/
void update_inner(int nrows, float *u1, float *u2)
{
	int i, j, n;

	n = nrows + 2;
//#pragma omp parallel for
	for (i = 2; i <= n - 3; i++)
	{
		for (j = 2; j <= n - 3; j++)
		{
			u2[i*n + j] = u1[i*n + j]
				+ parms.cx * (u1[(i + 1)*n + j] + u1[(i - 1)*n + j] - 2.f * u1[i*n + j])
				+ parms.cy * (u1[i*n + j + 1] + u1[i*n + j - 1] - 2.f * u1[i*n + j]);
		}
	}
}

/* This function operates the update calculation to the outer
	cells of the subgrid, which make use of the ghost cells
	of the subgrid.
	*/
void update_outer(int nrows, float *u1, float *u2)
{
	int ix, iy, ny;

	ny = nrows;

//#pragma omp parallel
	{
		//calculating the upper row
//#pragma omp for
		for (iy = 1; iy <= nrows; iy++){
			ix = 1;

			*(u2 + ix*(ny + 2) + iy) = *(u1 + ix*(ny + 2) + iy) +
				parms.cx * (*(u1 + (ix + 1)*(ny + 2) + iy) +
				*(u1 + (ix - 1)*(ny + 2) + iy) -
				2.0f * *(u1 + ix*(ny + 2) + iy)) +
				parms.cy * (*(u1 + ix*(ny + 2) + iy + 1) +
				*(u1 + ix*(ny + 2) + iy - 1) -
				2.0f * *(u1 + ix*(ny + 2) + iy));
		}

		//calculting the down row
//#pragma omp for
		for (iy = 1; iy <= nrows; iy++){
			ix = nrows;

			*(u2 + ix*(ny + 2) + iy) = *(u1 + ix*(ny + 2) + iy) +
				parms.cx * (*(u1 + (ix + 1)*(ny + 2) + iy) +
				*(u1 + (ix - 1)*(ny + 2) + iy) -
				2.0f * *(u1 + ix*(ny + 2) + iy)) +
				parms.cy * (*(u1 + ix*(ny + 2) + iy + 1) +
				*(u1 + ix*(ny + 2) + iy - 1) -
				2.0f * *(u1 + ix*(ny + 2) + iy));
		}

		//calculating the left column
//#pragma omp for
		for (ix = 1; ix <= nrows; ix++){
			iy = 1;

			*(u2 + ix*(ny + 2) + iy) = *(u1 + ix*(ny + 2) + iy) +
				parms.cx * (*(u1 + (ix + 1)*(ny + 2) + iy) +
				*(u1 + (ix - 1)*(ny + 2) + iy) -
				2.0f * *(u1 + ix*(ny + 2) + iy)) +
				parms.cy * (*(u1 + ix*(ny + 2) + iy + 1) +
				*(u1 + ix*(ny + 2) + iy - 1) -
				2.0f * *(u1 + ix*(ny + 2) + iy));
		}

		//calculating the right column
//#pragma omp for
		for (ix = 1; ix <= nrows; ix++){
			iy = nrows;

			*(u2 + ix*(ny + 2) + iy) = *(u1 + ix*(ny + 2) + iy) +
				parms.cx * (*(u1 + (ix + 1)*(ny + 2) + iy) +
				*(u1 + (ix - 1)*(ny + 2) + iy) -
				2.0f * *(u1 + ix*(ny + 2) + iy)) +
				parms.cy * (*(u1 + ix*(ny + 2) + iy + 1) +
				*(u1 + ix*(ny + 2) + iy - 1) -
				2.0f * *(u1 + ix*(ny + 2) + iy));
		}
	}
}

void allocate_neighbour_rows(float *buffer_neighbours[4]){
	int i;

	for (i = 0; i < 4; i++){
		buffer_neighbours[i] = malloc(info.nrows*sizeof(float));
		if (buffer_neighbours[i] == NULL)
		{
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
	}
	
}

float* allocate_subgrid(){

	float *subgrid;

	subgrid = malloc(info.nrows*info.nrows*sizeof(float));
	if (subgrid == NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	return subgrid;
}


float* allocate_grid(){

	float *_grid;

	_grid = malloc(info.grid_size*sizeof(float));
	if (_grid == NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	else return _grid;

}

/* This function creates and initializes the two buffers
	of the procedure. The two buffers are of size (nrows+2)x(nrows+2),
	where nrows is the size of the subgrid of each procedure.
	Two more rows and columns (+2) are added to the subgrid, for the ghost cells.
	*/
float** states_buffer_grid_init(float *initial_subgrid_tmp){

	int i, j, b;
	float **states_buffer_grid;

	states_buffer_grid = malloc(2 * sizeof(float*));
	if (states_buffer_grid == NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	for (i = 0; i < 2; i++){
		states_buffer_grid[i] = malloc((info.nrows + 2)*(info.nrows + 2) * sizeof(float));
		if (states_buffer_grid[i] == NULL)
		{
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
	}

	/*  Initializing all the cells of the buffer with 0.0 value.
		This means that the ghost cells which are not replaced by the neighbour's
		data (this happens when the procedure does not have this specific neighbour,
		for example, if a procedure does not have a left neighbour, all the cells
		of the the left column of the subgrid states_buffer_grid[0] stay 0.0.
		*/
	for (b = 0; b <= 1; b++)
	for (i = 0; i < info.nrows + 2; i++)
	for (j = 0; j < info.nrows + 2; j++)
		states_buffer_grid[b][i*(info.nrows + 2) + j] = 0.0;

	/* Initially all the ghost cells have value 0.0,
		since we replace only the cells which correspond
		to the initial subgrid of this procedure, which
		is scattered from the initial grid.
		*/
	temp_count = 0;
	for (i = 1; i <= info.nrows; i++){
		for (j = 1; j <= info.nrows; j++){
			states_buffer_grid[0][i*(info.nrows + 2) + j] = initial_subgrid_tmp[temp_count];
			temp_count++;

		}
	}

	return states_buffer_grid;
}

/* Deallocating everything. */
void deallocate(float *initial_grid, float *initial_subgrid_tmp,
	float *final_grid, float *final_subgrid, float *buffer_neighbours[4]){

	int i;

	for (i = 0; i < 4; i++)
		free(buffer_neighbours[i]);

	free(initial_grid);
	free(final_grid);
	free(final_subgrid);
	free(initial_subgrid_tmp);
}

/*	This function transforms the initial grid to a form suitable
	to be passed to MPI_Scatter function.

	*/
float* transform_initial_grid(float *initial_grid){

	int offset_x, offset_y;
	int x, y;
	int temp_count;

	float *initial_grid_scatter;

	initial_grid_scatter = allocate_grid();

	temp_count = 0;
	for (offset_x = 0; offset_x < info.side_size; offset_x += info.nrows){
	for (offset_y = 0; offset_y < info.side_size; offset_y += info.nrows){
			for (x = offset_x; x < offset_x + info.nrows; x++){
			for (y = offset_y; y < offset_y + info.nrows; y++){
					initial_grid_scatter[temp_count] = initial_grid[x*info.side_size + y];
					temp_count++;
			}
			}
	}
	}

	return initial_grid_scatter;
}

/*	This function transforms the final grid which has gathered all
	the data from each procedure.
	*/
float* transform_final_grid(float *final_grid){

	int offset_x, offset_y;
	int x, y;
	int temp_count;

	float *final_grid_all;

	final_grid_all = allocate_grid();

	temp_count = 0;
	for (offset_x = 0; offset_x < info.side_size; offset_x += info.nrows){
	for (offset_y = 0; offset_y < info.side_size; offset_y += info.nrows){
			for (x = offset_x; x < offset_x + info.nrows; x++){
			for (y = offset_y; y < offset_y + info.nrows; y++){
					final_grid_all[x*info.side_size + y] = final_grid[temp_count];
					temp_count++;
			}
			}
	}
	}

	return final_grid_all;
}

/*	This function calculates the ids of the
	neighbours for each procedure.
	*/
void calculate_neighbours(Neighbours_Ids *neighbours_ids, int taskid){

	info.topology_side_size = info.side_size / ((int)sqrt(info.grid_size / numtasks));

	neighbours_ids->above = taskid - info.topology_side_size;
	if (neighbours_ids->above < 0)
		neighbours_ids->above = NONE;

	neighbours_ids->below = taskid + info.topology_side_size;
	if (neighbours_ids->below >= numtasks)
		neighbours_ids->below = NONE;

	neighbours_ids->left = taskid - 1;
	if ((taskid % info.topology_side_size == 0))
		neighbours_ids->left = NONE;

	neighbours_ids->right = taskid + 1;
	if ((taskid + 1) % info.topology_side_size == 0)
		neighbours_ids->right = NONE;
}

/*  This function calculates the state of
	the buffer for each step.
	it = step_number.
	For example, for the step = 0 the state.current
	has value 0 and state.next has value 1. For
	the step = 1 the state.current has value 1
	and state.next has value 0 etc.
	*/
void calculate_state(int it, States *states){

	if (it % 2 == 0){
		states->current = 0;
		states->next = 1;
	}
	else{
		states->current = 1;
		states->next = 0;
	}
}

/*  This function implements one of the basic parts
	of the process of the subgrid buffer for each procedure.
	It exchanges the data between this procedure and
	its neighbour procedure's subgrids.
	Firstly it checks if the procedure has above, below, left and right
	neighbour. Then it sends and receives the rows and columns to add
	to the ghost cells of the subgrid buffer. For the above and below
	neighbours(if they exist), it sends directly the top and down rows of the buffer.
	For the left and right neighbours(if they exist) it first stores
	the data of the column to a buffer and then it sends it to the neighbour.
	Respectively, it first receives into a buffer the left and right column
	of its neighbours, and then it restores the data to the ghost cells of the buffer.
	*/
void exchange_data(Neighbours_Ids neighbours_ids, States states, float **states_buffer_grid, float *buffer_neighbours[4]){

	int i;

	/* if I have a neighbour above */
	if (neighbours_ids.above != NONE){
		//send my top row to my above worker
		MPI_Isend(&states_buffer_grid[states.current][info.nrows + 3], info.nrows,
			MPI_FLOAT, neighbours_ids.above, 0, MPI_COMM_WORLD, &request[0][0]);

		//receive the down row of my above worker
		MPI_Irecv(&states_buffer_grid[states.current][1], info.nrows,
			MPI_FLOAT, neighbours_ids.above, MPI_ANY_TAG, MPI_COMM_WORLD, &request[0][1]);
	}
	/* if I have a neigbour below */
	if (neighbours_ids.below != NONE){
		//send my bottom row to my above worker
		MPI_Isend(&states_buffer_grid[states.current][info.nrows*(info.nrows + 2) + 1], info.nrows,
			MPI_FLOAT, neighbours_ids.below, 0, MPI_COMM_WORLD, &request[1][0]);

		//receive the top row of my below worker
		MPI_Irecv(&states_buffer_grid[states.current][(info.nrows + 1)*(info.nrows + 2) + 1], info.nrows,
			MPI_FLOAT, neighbours_ids.below, MPI_ANY_TAG, MPI_COMM_WORLD, &request[1][1]);
	}
	/* if I have a neighbour to the right.*/
	if (neighbours_ids.right != NONE){
		//send my right column to my right neighbour
		for (i = 0; i < info.nrows; i++)
			buffer_neighbours[neighbours_counter.right_send][i] = states_buffer_grid[states.current][(i + 1)*(info.nrows + 2) + info.nrows];

		MPI_Isend(buffer_neighbours[neighbours_counter.right_send], info.nrows,
			MPI_FLOAT, neighbours_ids.right, 0, MPI_COMM_WORLD, &request[2][0]);

		//receive the left column of my right worker
		MPI_Irecv(buffer_neighbours[neighbours_counter.right_recv], info.nrows,
			MPI_FLOAT, neighbours_ids.right, 0, MPI_COMM_WORLD, &request[2][1]);
	}
	/* if I have a neighbour to the left.*/
	if (neighbours_ids.left != NONE){
		//send my left column to my left neighbour
		for (i = 0; i < info.nrows; i++)
			buffer_neighbours[neighbours_counter.left_send][i] = states_buffer_grid[states.current][(i + 1)*(info.nrows + 2) + 1];

		MPI_Isend(buffer_neighbours[neighbours_counter.left_send], info.nrows,
			MPI_FLOAT, neighbours_ids.left, 0, MPI_COMM_WORLD, &request[3][0]);

		//receive the right column of my left worker
		MPI_Irecv(buffer_neighbours[neighbours_counter.left_recv], info.nrows,
			MPI_FLOAT, neighbours_ids.left, 0, MPI_COMM_WORLD, &request[3][1]);
	}
}

void waiting(Neighbours_Ids neighbours_ids){

	if (neighbours_ids.above != NONE)
		MPI_Waitall(2, request[0], status[0]);

	if (neighbours_ids.below != NONE)
		MPI_Waitall(2, request[1], status[1]);

	if (neighbours_ids.right != NONE)
		MPI_Waitall(2, request[2], status[2]);

	if (neighbours_ids.left != NONE)
		MPI_Waitall(2, request[3], status[3]);
}

void replace_columns(Neighbours_Ids neighbours_ids, States states, float **states_buffer_grid, float *buffer_neighbours[4]){

	int i;

	if (neighbours_ids.left != NONE){
		//filling the left column of the buffer
		for (i = 0; i < info.nrows; i++)
			states_buffer_grid[states.current][(i + 1)*(info.nrows + 2)] =
			buffer_neighbours[neighbours_counter.left_recv][i];
	}

	if (neighbours_ids.right != NONE){
		//filling the right column of the buffer
		for (i = 0; i < info.nrows; i++)
			states_buffer_grid[states.current][(i + 1)*(info.nrows + 2) + (info.nrows + 1)] =
			buffer_neighbours[neighbours_counter.right_recv][i];
	}
}

void fill_final_subgrid(States states, float *final_subgrid, float **states_buffer_grid){

	int i, j;

	/*filling with the data the final grid to send to the master procedure*/
	for (i = 0; i < info.nrows; i++)
	for (j = 0; j < info.nrows; j++)
		final_subgrid[i*info.nrows + j] = states_buffer_grid[states.next][(i + 1)*(info.nrows + 2) + j + 1];
}

/*	This function calculates the necessary
	values such as the size of the grid,
	the size of the subgrid for each procedure,
	the size of the side of each subgrid,
	and the size of the subgrid of each procedure
	plus the columns with the ghost cells.
	*/
void calculations(){

	info.grid_size = info.side_size*info.side_size;

	info.sub_array_size = info.grid_size / numtasks;
	info.nrows = (int)sqrt(info.sub_array_size);
}

int checks(){

	if (info.grid_size%numtasks != 0){
		fprintf(stderr, "The grid _size must be divisible with the number of the processes \n");
		return ERROR_CODE;
	}
	if (!is_square_number(numtasks)){
		fprintf(stderr, "The number of the processes must be square number \n");
		return ERROR_CODE;
	}
	return 0;
}

int is_square_number(int number){
	int t;
	t = (int)sqrt(number);
	return number == t*t;
}

void inidat(int nx, int ny, float *u) {
	int ix, iy;

	for (ix = 0; ix < nx; ix++)
	for (iy = 0; iy < ny; iy++)
		*(u + ix*ny + iy) = (float)(ix * (nx - ix - 1) * iy * (ny - iy - 1));
}

void print_grid(float *grid){
	
	int i,j;

	printf("\n\nGrid :\n");
		for (i = 0; i < info.side_size; i++){
			printf("\n");
		for (j = 0; j < info.side_size; j++){
			printf("%3.2f ", grid[i*info.side_size + j]);
		}
		}
}

int main(int argc, char *argv[])
{
	int taskid;
	int it;
	int steps;
	double time_start, time_end;
	int stats = FALSE;


	//the initial grid..
	float *initial_grid = NULL;
	//the temp buffer to break the initial grid..
	float *initial_subgrid = NULL;
	//the final grid of the master procedure, which gathers the sub-grids of each worker-procedure
	float *final_grid = NULL;
	//the final grid of each procedure which is sent to the master procedure
	float *final_subgrid = NULL;
	//the sub-grid of each worker-procedure
	float **states_buffer_grid;

	float *buffer_neighbours[4];

	/*	This struct holds the ids of the
		neighbours for each procedure.
		*/
	Neighbours_Ids neighbours_ids;

	/*  This struct holds the states of the buffer_grid of each procedure
		which is processed during the steps.
		*/
	States states;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

	srand((unsigned int)time(NULL));

	/* The variable side_size holds the size of
	the side of the grid and it is given as the
	first argument of the program.
	*/
	info.side_size = atoi(argv[1]);
	/* the number of the steps */
	steps = atoi(argv[2]);

	/* The master procedure does all the necessary
		checks of the arguments.
	*/
	if (taskid == MASTER){
		
		if (info.side_size <= 0) {
			fprintf(stderr, "The square grid's size must be a positive integer number\n");
			return -1;
		}
		if (steps <= 0) {
			fprintf(stderr, "The number of the steps must be a positive integer number\n");
			return -1;
		}
		/* Print statistics */
		if (!strcmp("-stats", argv[3]))
			stats = TRUE;
		else stats = FALSE;	
	}

	/*	Calculation of the necessary values */
	calculations();

	if (checks() == ERROR_CODE)
		return ERROR_CODE;

	/*	buffer_neighbours is an array which holds
	the right and left columns that are exchanged
	between the procedures.					*/
	allocate_neighbour_rows(buffer_neighbours);

	/*	Create the subgrid to break the initial grid.
	Initial_subgrid_tmp holds the data for each procedure
	after the breaking of the initial grid.	*/
	initial_subgrid = allocate_subgrid();

	/*the master procedure creates the initial grid..*/
	if (taskid == MASTER){

		/*Create the initial grid */
		initial_grid = allocate_grid();

		/* Initialize the initial_grid */
		inidat(info.side_size, info.side_size, initial_grid);

		//print_grid(initial_grid);
		
		/*	Transforming the initial grid to a suitable
			form to be passed in the MPI_scatter function.
			*/
		initial_grid = transform_initial_grid(initial_grid);
	}

	/* Scattering of the subgrid to each procedure.*/
	MPI_Scatter(initial_grid, info.nrows*info.nrows, MPI_FLOAT, initial_subgrid, info.nrows*info.nrows, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

	/*	Initializing the states_buffer_grid array which holds
		the  subgrid of each procedure, in each step.
		The states_buffer_grid array consists of two
		buffers, one for the current state of the subgrid
		(states_buffer_grid[states.current]),
		and one for the new buffer grid which is calculated after each step
		(states_buffer_grid[states.next]) .
		*/
	states_buffer_grid = states_buffer_grid_init(initial_subgrid);

	/* Calculating the neighbours of the procedure */
	calculate_neighbours(&neighbours_ids, taskid);

	time_start = MPI_Wtime();
	for (it = 0; it < steps; it++){

		/*	Calculating the id of the current and the
			next state.
			*/
		calculate_state(it, &states);

		/* Exchange the data between this procedure and the neighbour procedures. */
		exchange_data(neighbours_ids, states, states_buffer_grid, buffer_neighbours);

		/*  Updating the inner cells of the subgrid.
			The inner cells of the subgrid do not make use
			of the ghost cells.
			*/
		update_inner(info.nrows, states_buffer_grid[states.current], states_buffer_grid[states.next]);


		/*  Waiting for all the procedures to finish the
			exchange of the data.
			*/
		waiting(neighbours_ids);

		/* Put the data I received from the neighbour
			procedures to the ghost cells of the subgrid buffer.
			*/
		replace_columns(neighbours_ids, states, states_buffer_grid, buffer_neighbours);

		/* Updating the outer cells  of the subgrid buffer,
			which need the ghost cells of the buffer, to be calculated.
			*/
		update_outer(info.nrows, states_buffer_grid[states.current], states_buffer_grid[states.next]);
	}
	
	final_subgrid = allocate_subgrid();

	fill_final_subgrid(states, final_subgrid, states_buffer_grid);

	if (taskid == MASTER) {

		//allocating memory for the final grid
		final_grid = allocate_grid();
	}
	
	/* The MPI_Gather function collects the final subgrid of
		each procedure to the final_grid grid. The final grid
		keeps the subgrids in serial form. This is why the
		function transform_final_grid is used.
		*/
	MPI_Gather(final_subgrid, info.nrows*info.nrows, MPI_FLOAT, final_grid, info.nrows*info.nrows, MPI_FLOAT, MASTER, MPI_COMM_WORLD);
	time_end = MPI_Wtime();

	if (taskid == MASTER) {

		final_grid = transform_final_grid(final_grid);

		//print_grid(final_grid);
		
		if (stats == TRUE){
			printf("\n\n\n-Number of processes: %d\n", numtasks);
			printf("-Size of Grid: %d\n", info.grid_size);
			printf("-Execution time: %.4lf sec\n", time_end - time_start);
			printf("-Number of steps: %d\n", steps);
		}
	}

	MPI_Finalize();

	deallocate(initial_grid, initial_subgrid, final_grid, final_subgrid, buffer_neighbours);

	return 0;
}


