#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {
        int rank;
        int message;
	int repeat = 1;
	
	MPI_Init(&argc, &argv);
	while (repeat){
		
	        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		if(rank == 0) {
		   scanf("%d", &message);
		}
		if (message < 0){

			repeat = 0;
		}

		if (repeat == 1){

			MPI_Bcast(&message, 1, MPI_INT, rank, MPI_COMM_WORLD);
			printf("[Process %d]: Otrzymano wiadomosc %d\n", rank, message);
		}
	
	};

	MPI_Finalize();
	return 0;
}
