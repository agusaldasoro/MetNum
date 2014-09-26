#ifndef SPARSE_H
#define SPARSE_H

#include<vector>
#include<iostream>

class MatrizEsparsa //solo se pueden hacer matrices cuadradas
{
public:
	MatrizEsparsa(int dim, int elemsNoNulos);

	//Pre: (val != 0) && (fila < dim) && (col < dim)
	void definirPos(int fila, int col, double val); //los parametros tienen que ser pasados en ese orden: primero fila, despues columna, despues valor.
												 //si se quisiera insertar un elemento en una matriz traspuesta, el usuario se debera encargar de pasar los indices como correspondan.

	std::vector<double> multMatVec(std::vector<double> v) const; //verificar aridad de lo que devuelve: asi hace una copia y esta todo bien?
																 //o voy a tener que usar memoria dinamica y entonces me conviene usar una referencia?

	void trasponer();

	int dimension() const;

private:
	std::vector<double> val;
	std::vector<int> ind_col;
	std::vector<int> ptr_fil;
	bool es_traspuesta; //estamos utilizando CSR, por lo que si es_traspuesta es true, vamos a interpretar a la matriz como si estuvieramos usando CSC
};


#endif