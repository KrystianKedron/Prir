#include <stdio.h>

double f1(double x) { 

	return 1/(1+x*x); 
}
 
int main()
{
    double xp, xk, h, calka;
    int n;
 
    // przedzialy
    xp = 0;
    xk = 1;
 
    n = 1000;
 
    h = (xk - xp) / (double)n;

    printf("krok: h=%f \n", h);
 
    calka = 0;
    double x = 0;

    for (int i=1; i<=n; i++)
    {
	x = xp + i*h;
	printf("X = %f", x);
        calka += f1(xp + i*h);
	printf("Calka %f \n", calka);
    }
 		
    calka += (f1(xp) + f1(xk)) / 2;
    calka *= h;
    calka = 4*calka;
    printf("Wynik calkowania metoda trapezow:%f \n", calka);

    return 0;
}

