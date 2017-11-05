#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "pomiar_czasu.h"

omp_lock_t lock;

int main() {

	int N = 200;
	int M = 300;
	int P = 300;
	
	int **a; 
	a = (int)malloc(N*sizeof(int*));
	// alokacja pamieci a
	for (int i = 0; i < N; i++){

		a[i] = (int)malloc(M*sizeof(int*));
	}

	// incjalizacja watrosci a
	for (int i = 0; i < N; i++){
		for (int j = 0; j < M; j++){
			a[i][j] = rand()%11;
		}
	}

	// wypisywanie a
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			printf("%d\t", a[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	int **b; 
	b = (int)malloc(M*sizeof(int*));
	// alokacja pamieci b
	for (int i = 0; i < M; i++){

		b[i] = (int)malloc(P*sizeof(int*));
	}

	// incjalizacja watrosci b
	for (int i = 0; i < M; i++){
		for (int j = 0; j < P; j++){
			b[i][j] = rand()%11;
		}
	}

	// wypisywanie b
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			printf("%d\t", b[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	int **wynik; 
	wynik = (int)malloc(N*sizeof(int*));
	// inicjalizacja pamieci wyniku
	for (int i = 0; i < N; i++){

		wynik[i] = (int)malloc(P*sizeof(int*));
	}

	// obliczanie wyniku
	printf("N%d\n", N);
	printf("P%d\n", P);
	printf("M%d\n",M);

	int nr_w = 4;
	omp_set_num_threads(nr_w);
	omp_init_lock(&lock);
	inicjuj_czas();
	int suma = 0;
	// #pragma omp oarallel for schedule(static) shared(suma) 
	for (int i = 0; i < N; i++){
		for (int j = 0; j < P; j++){
			wynik[i][j] = 0;
			suma = 0;
			int my_id= omp_get_thread_num();
			//omp_set_lock(&lock);
			#pragma omp oarallel for schedule(static) reduction(+:suma)
			for (int k = 0; k < M; k++){
				suma = suma + a[i][k] * b[k][j]; 
			}
			wynik[i][j] = suma;
			//omp_unset_lock(&lock);
		}		
	}
	drukuj_czas();

	// wypisywanie wyniku
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			printf("%d\t", wynik[i][j]);
		}
		printf("\n");
	}
}
