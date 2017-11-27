#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include"pomiar_czasu.h"

typedef struct {

	char name[20];
	int level;
	char klasa_postaci[20];
	char gildia[20];
	double time;
} Data;


int main(int argc, char **argv)
{
	int rank, size, r;
	int src, dst, tag, i;	
	MPI_Status status;
	MPI_Datatype new_type;
	MPI_Datatype type[5] = { MPI_CHAR, MPI_INT, MPI_CHAR, MPI_CHAR, MPI_DOUBLE };
	int blen[5] = { 20, 1, 20, 20,  1};
	MPI_Aint disp[5];
	MPI_Aint base, addr;
	Data tabrecord,tabrecieved;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0) 
		printf("MPI_Type_create_struct()\n");

	MPI_Get_address(&tabrecord, &base);
	MPI_Get_address(&(tabrecord.name), &addr);
	disp[0] = addr - base;
	MPI_Get_address(&(tabrecord.level), &addr);
	disp[1] = addr - base;
	MPI_Get_address(&(tabrecord.klasa_postaci), &addr);
	disp[2] = addr - base;
	MPI_Get_address(&(tabrecord.gildia), &addr);
	disp[3] = addr - base;
	MPI_Get_address(&(tabrecord.time), &addr);
	disp[4] = addr - base;
	
	// ile, dlugosc(tablica), tablica_przesuniec, typy(tablica), nazwa 
	MPI_Type_create_struct(5, blen, disp, type, &new_type);
	MPI_Type_commit(&new_type);

	if(rank > 1)
	{
		strcpy(tabrecieved.name, "Peter");
		tabrecieved.level = 14;
		strcpy(tabrecieved.name, "Mag");
		strcpy(tabrecieved.name, "SUPER");
		tabrecieved.time = 0;
		printf("In process %d my name is %s and my level is %d \n", 
			rank,tabrecieved.name,tabrecieved.level);		
		MPI_Recv(&tabrecieved, 1, new_type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("In process %d I have received name: %s ",rank,tabrecieved.name);
		printf("and level: %d\n",tabrecieved.level);
	}
	else
	{

		double t2 = 0;
		double t1 = 0;
		if (rank == 0){
			
			strcpy(tabrecord.name, "John");
			tabrecord.level = 54;
			strcpy(tabrecord.name, "Mag");
			strcpy(tabrecord.name, "SUPER EXTRA");
			tabrecord.time = czas_zegara();
			printf("Czas t1 %lf \n", tabrecord.time);
			for (r=1; r<size; r++)
				MPI_Send(&tabrecord, 1, new_type, r, 0, MPI_COMM_WORLD);
		}
		if (rank == 1){

			MPI_Recv(&tabrecieved, 1, new_type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			strcpy(tabrecord.name, "John");
			tabrecord.level = 555;
			strcpy(tabrecord.name, "Mag");
			strcpy(tabrecord.name, "NIE SUPER");
			double czas = czas_zegara() - tabrecieved.time;
			tabrecord.time = czas;
			printf("In process %d I have received name: %s ",rank,tabrecieved.name);
			printf("and level: %d\n",tabrecieved.level);
			
			MPI_Send(&tabrecord, 1, new_type, 0, 0, MPI_COMM_WORLD);
		}

		if (rank == 0){
			printf("Weszlo \n");
			MPI_Recv(&tabrecieved, 1, new_type, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			printf("In process %d I have received name: %s ",rank,tabrecieved.name);
			printf("and level: %d\n",tabrecieved.level);
			printf("Czas operacji %lf \n", tabrecieved.time);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}
