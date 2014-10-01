#include "vectores.h"

void normalizar(std::vector<double>& v)
{
	double suma = 0.0;

	for (int i = 0; i < v.size(); ++i)
	{
		suma += pow(v[i],2);
	}

	double norma = sqrt(suma);

	for (int i = 0; i < v.size(); ++i)
	{
		v[i] = v[i]/norma;
	}
}

//PRE: tam(x)==tam(y)
bool vecIguales(std::vector<double>& x, std::vector<double>& y, double tol)
{
	int tam = x.size();
	for (int i = 0; i < tam; ++i)
	{
		if (fabs(x[i]-y[i]) > tol)
		{
			return false;
		}
	}
	return true;
}