#include "sparse.h"
#include "vectores.h"

void indeg(MatrizEsparsa a, std::ofstream& salida)
{
	int n = a.dimension();
	std::vector<double> v(n,1.0);
	a.trasponer();
	v = a.multMatVec(v);

	for (int i = 0; i < v.size(); ++i)
		salida << v[i] << std::endl;
}