#include "sparse.h"
#include "vectores.h"

void hits(MatrizEsparsa a, double tol, std::ofstream& salida)
{
	int n = a.dimension();

	int maxIt = 100000; //NOSOTROS TENEMOS QUE ELEGIR LA CANT MAX DE ITERACIONES

	std::vector<double> x(n,1.0), y(n,1.0), xp, yp;

	std::ofstream norma("normasHITS.out");
    norma.setf( std::ios::fixed, std::ios::floatfield );
    norma.precision(6);

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


		double normaX = 0;
		double normaY = 0;
		for (int j = 0; j < x.size(); ++j){
			normaX += fabs(x[j]);
			normaY += fabs(y[j]);
		}
		norma << "Normas iteracion " << i << std::endl << "X: " << normaX << "\t Y: " << normaY << std::endl;
	}

	for (int i = 0; i < x.size(); ++i)
		salida << x[i] << std::endl;
	for (int i = 0; i < y.size(); ++i)
		salida << y[i] << std::endl;
}