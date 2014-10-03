#ifndef VECTORES_H
#define VECTORES_H

#include <cmath>
#include <vector>

void normalizar(std::vector<double>& v);

//PRE: tam(x)==tam(y)
bool vecIguales(std::vector<double>& x, std::vector<double>& y, double tol);

#endif