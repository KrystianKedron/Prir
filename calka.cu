#include<stdio.h>
#include"pomiar_czasu.h"
#include"pomiar_czasu.c"
#include "cuPrintf.cuh"
#include "cuPrintf.cu"

#define N 30
#define M 8 

// Sterowanie przedzialami
#define poczatek 0
#define koniec 1

double f1(double x) { 

	return 1/(1+x*x); 
}

__global__ void compute(float *calka, float *xp, float *h, int n) 
{
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	cuPrintf("Thread number %d. \n", index);
	cuPrintf("Xp = %f \n", xp);
//	if(index<n)
//		*calka = *calka + 1/(1+(*xp+index**h)*(*xp + index**h));
//		cuPrintf("Thread number %d. calka = %f\n", threadIdx.x, *calka);		
}

int main(void) {

	//PROGRAM SEKWENCYJNY (wersja na wzkaznikach)
	float val = 0;
	float *calka = &val;	

	float xk;
	
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
	
	double czas_start_r = czas_zegara();
	int size = sizeof(float);
	float *d_xp, *d_h;
	float *d_calka, *r_calka;
	
	r_calka = (float *)malloc(size);
	xp = (float *)malloc(size);
	*xp = 10.00f;
	
	cudaMalloc((void **)&d_calka, size);
	cudaMalloc((void **)&d_xp, size);
	cudaMalloc((void **)&d_h, size);	
	
	printf("Host xp = %f \n", *xp);	
	cudaMemcpy(d_xp, xp, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_h, h, size, cudaMemcpyHostToDevice);
	
	cudaPrintfInit();
	compute<<<(N+M-1)/M,M>>>(d_calka, d_xp, d_h, N);
	cudaPrintfDisplay(stdout, true);
    	cudaPrintfEnd();

	cudaMemcpy(r_calka, d_calka, size, cudaMemcpyDeviceToHost);
	printf("Calka przed rownolegla %f\n", *r_calka);

	//*r_calka += (f1(*xp) + f1(xk)) / 2;
	//*r_calka *= *h;
	//*r_calka = 4**r_calka;
	
	//printf("Calka rownolegla %f\n", *r_calka);
	double czas_stop_r = czas_zegara();
	double czas_r = czas_stop_r - czas_start_r;
        printf("Czas operacji na CUDA %.4f \n", czas_r);
	//free(xp); free(h); free(r_calka);
	cudaFree(d_xp); cudaFree(d_h); cudaFree(d_calka);
	return 0;
}
