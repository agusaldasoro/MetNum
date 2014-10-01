#include "sparse.h"
#include "vectores.h"

void hits(MatrizEsparsa a, double tol, int k, std::ofstream& salida)
{
	int n = a.dimension();

	std::vector<double> x(n,1.0), y(n,1.0), xp, yp;

	for (int i = 0; i < x.size(); ++i)
			std::cout << x[i] <<", ";
	std::cout << std::endl;

	int i = 0;
	while(i < 100000)
	{
		a.trasponer();
		xp = a.multMatVec(y);

		/*for (int i = 0; i < xp.size(); ++i)
			std::cout << xp[i] <<", ";
		std::cout << std::endl;*/

		normalizar(xp); //probar poniendolo inline

		/*for (int i = 0; i < xp.size(); ++i)
			std::cout << xp[i] <<", ";
		std::cout << std::endl;*/




		a.trasponer();
		yp = a.multMatVec(x);
		normalizar(yp);


		/*if (vecIguales(x,xp,tol) && vecIguales(y,yp,tol))
			i=10000;
		else*/
			i++;

		x = xp;
		y = yp;
	}

	for (int i = 0; i < x.size(); ++i)
		salida << x[i] << std::endl;
	for (int i = 0; i < y.size(); ++i)
		salida << y[i] << std::endl;
}