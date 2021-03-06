#include "sparse.h"
#include "vectores.h"

void pagerank(MatrizEsparsa a, double c, double tol, std::ofstream& salida)
{
	double n = a.dimension();

	int maxIt = 1000; //NOSOTROS TENEMOS QUE ELEGIR LA CANT MAX DE ITERACIONES

	//HAY QUE AGREGAR LA COMPARACION DE Xk > Xk-1 POR SI NO CONVERGE
	//RANDOMIZAMOS EL VECTOR O QUE?

	//std::vector<double> dif(n);
	std::vector<double> x(n,1), xp;
	/*for (int i = 0; i < n; ++i)
		x[i] = 1/n;*/
	double factor = ((1-c)/n);

	// std::ofstream normaFile("normasPageRank.out");
	// normaFile.setf( std::ios::fixed, std::ios::floatfield );
	// normaFile.precision(6);

	int j = 0;
	while(j < maxIt)
	{
		//std::cout << "Iteración " << j << std::endl;
		xp = a.multMatVec(x);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]*c;

		////FORMA NORMAL:
		// for (int i = 0; i < xp.size(); ++i)
		// 	xp[i] = xp[i]+factor;

		// double norma1 = 0.0;
		// for (int i = 0; i < xp.size(); ++i)
		// 	norma1 += fabs(xp[i]);

		// for (int i = 0; i < xp.size(); ++i)
		// 	xp[i] = xp[i]/norma1;

		////FORMA MAGICA:
		double norma1p = 0.0;
		for (int i = 0; i < xp.size(); ++i)
			norma1p += fabs(xp[i]);

		double norma1 = 0.0;
		for (int i = 0; i < x.size(); ++i)
			norma1 += fabs(x[i]);

		double w = norma1 - norma1p;
		w = w/n;

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]+w;

		if (vecIguales(x,xp,tol))
			j=maxIt;
		else
			j++;


		// for (int i = 0; i < x.size(); ++i)
		// 	dif[i] = x[i]-xp[i];

		x = xp;

		// double normaprint = 0;
		// for (int i = 0; i < x.size(); ++i)
		// 	normaprint += fabs(dif[i]);
		// normaFile /*<< "Norma iteracion " << j << ": "*/ << normaprint << std::endl;
	}

	double norma = 0.0;
	for (int i = 0; i < x.size(); ++i)
		norma += fabs(x[i]);

	for (int i = 0; i < x.size(); ++i)
		x[i] = x[i]/norma;

	for (int i = 0; i < x.size(); ++i)
		salida << x[i] << std::endl;
}
