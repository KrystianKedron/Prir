#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv) 
{ 
	int rank, sent, size, source, i, len, a, next, prev;
	MPI_Status status; 

	MPI_Init(&argc, &argv); 
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &size); 

	prev = rank-1;
   	next = rank+1;
	if (rank == 0)  prev = size - 1;
	if (rank == (size - 1))  next = 0;

	a = rank * 10;
	MPI_Send(&a,1,MPI_INT,next,0,MPI_COMM_WORLD);
	printf("Proces %d wyslal %d do procesu %d \n",rank,a,next);	
	MPI_Recv(&sent,1,MPI_INT,prev,MPI_ANY_TAG,MPI_COMM_WORLD,&status); 
	printf("Proces %d otrzymal %d od procesu %d \n",rank,sent,status.MPI_SOURCE);

	MPI_Finalize(); 
	return(0); 
}
