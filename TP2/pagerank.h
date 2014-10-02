#include "sparse.h"
#include "vectores.h"

void pagerank(MatrizEsparsa a, double c, double tol, std::ofstream& salida)
{
	int n = a.dimension();

	int maxIt = 100000; //NOSOTROS TENEMOS QUE ELEGIR LA CANT MAX DE ITERACIONES

	//AGREGAR LA COMPARACION DE Xk > Xk-1 POR SI NO CONVERGE

	std::vector<double> x(n,1/n), xp;

	int i = 0;
	while(i < maxIt)
	{
		xp = a.multMatVec(x);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]*c;

		double factor = ((1-c)/n);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]+factor;

		double suma = 0;
		for (int i = 0; i < xp.size(); ++i)
			suma += fabs(xp[i]);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]/suma;


		if (vecIguales(x,xp,tol))
			i=maxIt;
		else
			i++;

		x = xp;
	}

	for (int i = 0; i < x.size(); ++i)
		salida << x[i] << std::endl;
}