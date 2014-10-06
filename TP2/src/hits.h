#include "sparse.h"
#include "vectores.h"

void hits(MatrizEsparsa a, double tol, std::ofstream& salida)
{
	int n = a.dimension();

	int maxIt = 100000; //NOSOTROS TENEMOS QUE ELEGIR LA CANT MAX DE ITERACIONES

	std::vector<double> x(n,1.0), y(n,1.0), xp, yp;

	int i = 0;
	while(i < maxIt)
	{
		a.trasponer();
		xp = a.multMatVec(y);
		normalizar(xp); //probar poniendolo inline

		a.trasponer();
		yp = a.multMatVec(x);
		normalizar(yp); //idem


		if (vecIguales(x,xp,tol) && vecIguales(y,yp,tol))
			i=maxIt;
		else
			i++;

		x = xp;
		y = yp;

		double normaprint = 0;
		for (int i = 0; i < x.size(); ++i)
			normaprint += fabs(x[i])	;
		//std::cout << "Norma 1 X: " << normaprint << std::endl;

		normaprint = 0;
		for (int i = 0; i < y.size(); ++i)
			normaprint += fabs(y[i])	;
		std::cout << "Norma 1 Y: " << normaprint << std::endl;
	}

	for (int i = 0; i < x.size(); ++i)
		salida << x[i] << std::endl;
	for (int i = 0; i < y.size(); ++i)
		salida << y[i] << std::endl;
}