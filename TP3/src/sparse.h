#ifndef SPARSE_H
#define SPARSE_H

#include <vector>
#include <iostream>
#include <cmath>


typedef unsigned int Nat;

class Matriz
{
public:

	Matriz(const Nat& dim);

	double& Elem(const Nat& fila, const Nat& columna);

	void Res(const Nat& fila, const double& res);

	void EG();

	std::vector<double> ResolverSistema();

private:
	Nat _dim;
	std::vector< std::vector<double> > _mtrx;
};

#endif