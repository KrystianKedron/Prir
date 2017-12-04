//Zrównoleglenie - kilka bloków - kilka wątków + równoważenie
#include<stdio.h>
#include"pomiar_czasu.h"
#include"pomiar_czasu.c"

#define N 300000000
#define M 8 //Watki na blok

__global__ void add(int *a, int *b, int *c, int n) 
{
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	if(index<n)
		c[index] = a[index] + b[index];
}

void random (int *tab, int wym )
{	
	int i;
	for(i=0;i<wym;i++)
		tab[i]=rand()%101;
}

int main(void) {

	int *a, *b, *c; // host copies of a, b, c
	int *d_a, *d_b, *d_c; // device copies of a, b, c
	int size = N * sizeof(int);
	int i;
	int j;

	srand(time(NULL));
	// Allocate space for device copies of a, b, c
	cudaMalloc((void **)&d_a, size);
	cudaMalloc((void **)&d_b, size);
	cudaMalloc((void **)&d_c, size);
	// Alloc space for host copies of a, b, c and setup input values
	a = (int *)malloc(size); random(a, N);
	b = (int *)malloc(size); random(b, N);
	c = (int *)malloc(size);
	
        int *d;
	
	double czas_start = czas_zegara();
        printf("Czas %.4f \n", czas_start);

        d = (int *)malloc(size);

        for (j = 0; j < N; j++){

                d[j] = a[j] + b[j];
        }

        for(i=0;i<N;i++)
        {
                //printf("a[%d](%d) + b[%d](%d) = d[%d](%d)\n",i,a[i],i,b[i],i,d[i]);
        }
	
        double czas_stop = czas_zegara();
	printf("Czas na CPU \n");
	printf("Czas stop %.4f \n", czas_stop);
	double czas = czas_stop - czas_start;
        printf("Czas %.4f \n", czas);

	i = 0;

	double czas_calosc_start = czas_zegara();
	// Copy inputs to device
	cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);
	// Launch add() kernel on GPU
	
	printf("Czas bez kopiowania na GPU \n");
	double czas_bez_start = czas_zegara();
        printf("Czas start %.4f \n", czas_bez_start);
	
	add<<<(N+M-1)/M,M>>>(d_a, d_b, d_c, N);
	
	double czas_bez_stop = czas_zegara();
        printf("Czas stop %.4f \n", czas_bez_stop);
        double czas_bez = czas_bez_stop - czas_bez_start;
        printf("Czas %.4f \n", czas_bez);

	// Copy result back to host
	cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
	
	printf("Czas calosci operacji na GPU \n");
	printf("Czas start %.4f \n", czas_calosc_start);
	double czas_calosc_stop = czas_zegara();
	printf("Czas stop %.4f \n", czas_calosc_stop);
        double czas_calosc = czas_calosc_stop - czas_calosc_start;
        printf("Czas %.4f \n", czas_calosc);

	for(i=0;i<N;i++)
	{
		//printf("a[%d](%d) + b[%d](%d) = c[%d](%d)\n",i,a[i],i,b[i],i,c[i]);
	}
	// Cleanup
	//printf("%d+%d=%d\n",a,b,c);
	free(a); free(b); free(c);
	cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
	
	return 0;
}

