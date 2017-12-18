//Zrównoleglenie - kilka bloków - kilka wątków + równoważenie
#include<stdio.h>
#include"pomiar_czasu.h"
#include"pomiar_czasu.c"

#define N 30000
#define M 8 //Watki na blok

// Sterowanie przedzialami
#define poczatek 0
#define koniec 1

double f1(double x) { 

	return 1/(1+x*x); 
}

//__global__ void compute(float *calka, float *xp, float *h, int n) 
//{
//	int index = threadIdx.x + blockIdx.x * blockDim.x;
//	if(index<n)
//		*calka = *calka + f1(*xp + index**h);
//}

int main(void) {

	//PROGRAM SEKWENCYJNY (wersja na wzkaznikach)
	float val = 0;
	float *calka = &val;	

	float xk;
	int n;
	
	float zero = poczatek;
	float *xp = &zero;

	xk = koniec;

	float steps = (xk - *xp) / (double)N;
	float *h = &steps;
	
	double czas_start = czas_zegara();
	
	for (int i=1; i<=N; i++)
	{
		*calka = *calka + f1(*xp + i**h);
	}

	*calka += (f1(*xp) + f1(xk)) / 2;
	*calka *= *h;
	*calka = 4**calka;

	printf("Calka sekwencyjna %f\n", *calka);

        double czas_stop = czas_zegara();
	double czas = czas_stop - czas_start;
        printf("Czas operacji na CPU %.4f \n", czas);

	// PROGRAM ROWNOLEGLY
	
	//double czas_start_r = czas_zegara();

	//int size = N * sizeof(float);
	//float *d_xp, *d_h;
	//float *d_calka;

	//cudaMemcpy(d_xp, xp, size, cudaMemcpyHostToDevice);
	//cudaMemcpy(d_h, h, size, cudaMemcpyHostToDevice);

	//compute<<N+M-1>>(d_calka, d_xp, d_h, N);
	//float *r_calka;
	//cudaMemcpy(s_calka, d_calka, size, cudaMemcpyHostToDevice);

	//*r_calka += (f1(*xp) + f1(xk)) / 2;
	//*r_calka *= *h;
	//*r_calka = 4*r_calka;
	
	//printf("Calka rownolegla %f\n", *r_calka);
	//double czas_stop_r = czas_zegara();
	//double czas_r = czas_stop_r - czas_start_r;
        //printf("Czas operacji na CUDA %.4f \n", czas_r);

	return 0;
}


