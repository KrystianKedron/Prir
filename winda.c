#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include"pomiar_czasu.h"
#include <time.h>
#include <unistd.h>

#define numberOfFloors 5

typedef struct {

	int currentFloor;
	int wantedFloor;
	int signalFromFloor[20];
	int goToFloor;
} Data;


int main(int argc, char **argv)
{
	int rank, size, r, sign;
	int repeat = 1;
	int src, dst, tag, i;	
	double t1;
	MPI_Status status;
	MPI_Datatype new_type;
	MPI_Datatype type[4] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT };
	int blen[4] = { 1, 1, 20, 1};
	MPI_Aint disp[4];
	MPI_Aint base, addr;
	Data tabrecord,tabrecieved;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0) 
		printf("MPI_Type_create_struct()\n");

	MPI_Get_address(&tabrecord, &base);
	MPI_Get_address(&(tabrecord.currentFloor), &addr);
	disp[0] = addr - base;
	MPI_Get_address(&(tabrecord.wantedFloor), &addr);
	disp[1] = addr - base;
	MPI_Get_address(&(tabrecord.signalFromFloor), &addr);
	disp[2] = addr - base;
	MPI_Get_address(&(tabrecord.goToFloor), &addr);
	disp[3] = addr - base;
	
	// ile, dlugosc(tablica), tablica_przesuniec, typy(tablica), nazwa 
	MPI_Type_create_struct(4, blen, disp, type, &new_type);
	MPI_Type_commit(&new_type);	

	if(rank == 0) 
	{ 	
		// init!!!
		tabrecord.currentFloor = 0;
		srand(czas_zegara());
		tabrecord.signalFromFloor[0] = -1;
		while(tabrecord.currentFloor >= tabrecord.signalFromFloor[0]) {
			tabrecord.signalFromFloor[0] = rand() % numberOfFloors;
		}
		tabrecord.wantedFloor = -1;
		MPI_Send(&tabrecord, 1, new_type, 1, 0, MPI_COMM_WORLD); 
		printf("Proces uzytkiwnika wylosowal dane: current %d signalFrom %d \n", 
			tabrecord.currentFloor, tabrecord.signalFromFloor[0]);

		//process user rank 0
		while (1){
			MPI_Recv(&tabrecieved, 1, new_type, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			printf("+++++++++++++++++++++++++++++++++++++ \n");
			printf("User \n");
			printf("+++++++++++++++++++++++++++++++++++++ \n");
			printf("Odebrano od winda jedzie na %d obecnie jest na %d! \n", tabrecieved.goToFloor, tabrecieved.currentFloor);

			tabrecord.currentFloor = tabrecieved.currentFloor;
			tabrecord.goToFloor = tabrecieved.goToFloor;
			
			sleep(5);
			if (tabrecord.currentFloor == tabrecord.goToFloor){

				printf("Dotralem na zadane pietro! %d\n", tabrecieved.currentFloor);
				srand(czas_zegara());
				repeat = 1;
				if (tabrecieved.wantedFloor == -1){
					while(repeat) {

						tabrecord.wantedFloor = 1 + rand() % numberOfFloors;
						if (tabrecord.wantedFloor != tabrecieved.currentFloor)
							repeat = 0;

						printf("Wylosowano wantedFloor %d \n", tabrecord.wantedFloor);
					}
					MPI_Send(&tabrecord, 1, new_type, 1, 0, MPI_COMM_WORLD); 
				}else {
					tabrecord.wantedFloor = -1;
					while(repeat) {

						tabrecord.signalFromFloor[0] = 1 + rand() % numberOfFloors;
						if (tabrecord.signalFromFloor[0] != tabrecieved.currentFloor)
							repeat = 0;

						printf("Wylosowano signalFromFloor %d \n", tabrecord.signalFromFloor[0]);
					}
					MPI_Send(&tabrecord, 1, new_type, 1, 0, MPI_COMM_WORLD); 
				}
			} else {

				MPI_Send(&tabrecord, 1, new_type, 1, 0, MPI_COMM_WORLD); 
			}
		}
	} 
	else 
	{ 
		//process windy rank 1
		while (1){
			MPI_Recv(&tabrecieved, 1, new_type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			printf("+++++++++++++++++++++++++++++++++++++ \n");
			printf("Winda! \n");
			printf("+++++++++++++++++++++++++++++++++++++ \n");

			if (tabrecieved.wantedFloor == -1) {
				tabrecord.goToFloor = tabrecieved.signalFromFloor[0];
				if (tabrecieved.signalFromFloor[0] - tabrecieved.currentFloor > 0){
					sign = 1;
				} else {
					sign = -1;
				}
				tabrecord.currentFloor = tabrecieved.currentFloor + (sign*1);
				tabrecord.signalFromFloor[0] = tabrecieved.signalFromFloor[0];
				tabrecord.wantedFloor = -1;
				printf("Jade na pietro z oczkujacym uzytkownikiem: jestem na %d, jade na %d \n",
					tabrecord.currentFloor, tabrecord.goToFloor);
			} else {

				tabrecord.goToFloor = tabrecieved.wantedFloor;
				tabrecord.wantedFloor = tabrecieved.wantedFloor;
				if (tabrecieved.wantedFloor - tabrecieved.currentFloor > 0){
					sign = 1;
				} else {
					sign = -1;
				}
				tabrecord.currentFloor = tabrecieved.currentFloor + (sign*1);
				tabrecord.signalFromFloor[0] = -1;
				printf("Jade na piertro wybrane przez uzytkownika: jestem na %d, jade na %d \n",
					tabrecord.currentFloor, tabrecord.goToFloor);
				
			}
			
			MPI_Send(&tabrecord,1, new_type, 0, 1, MPI_COMM_WORLD); 
		}
	}  


	
	MPI_Finalize();
	return 0;
}
