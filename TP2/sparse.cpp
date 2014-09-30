#include "sparse.h"

MatrizEsparsa::MatrizEsparsa(int dim) : ptr_fil(dim+1, 0), es_traspuesta(false) {};

void MatrizEsparsa::definirPos(int fila, int col, double elem)
{
	int i = this->ptr_fil[fila];
	int fin = this->ptr_fil[fila+1]-1;
	std::vector<int>::iterator itcol = ind_col.begin();
	std::vector<double>::iterator itval = val.begin();
	std::advance(itcol, i);
	std::advance(itval, i);
	// if (i==fin)
	// {
	// 	ind_col.insert(itcol,col);
	// 	val.insert(itval,elem);
	// }
	// else
	// {
		while( (i < fin) && (col > *itcol) )
		{
			std::advance(itcol, 1);
			std::advance(itval, 1);
			i++;
		}

		if (i == ind_col.size()) //i == fin
		{
			ind_col.push_back(col);
			val.push_back(elem);
		}
		else
		{
			ind_col.insert(itcol,col);
			val.insert(itval,elem);
		}

		int filas = ptr_fil.size();
		for (int i = fila+1; i < filas; ++i)
			this->ptr_fil[i] += 1;

	//}
};

void MatrizEsparsa::trasponer(){this->es_traspuesta = !(this->es_traspuesta);};

std::vector<double> MatrizEsparsa::multMatVec(std::vector<double> v) const
{
	//PROBABLEMENTE TENGA QUE HACER NEW, pero habra que probar

	int numFilas = this->ptr_fil.size()-1;

	std::vector<double> res(numFilas, 0);

	if(!es_traspuesta)
	{
		int inicio;
		int fin;
		int col;

		for (int i = 0; i < numFilas; ++i)
		{
			inicio = this->ptr_fil[i];
			fin = this->ptr_fil[i+1];

			for (int j = inicio; j < fin; ++j)
			{
				col = this->ind_col[j];
				res[i] += ((this->val[j])*(v[col]));
			}
		}
	}
	else
	{
		int col;
		int j;

		for (int i = 0; i < numFilas; ++i) //por cada fila, recorro los indices de ind_col (que por estar traspuesta la matriz ahora tiene los indices de las filas, no las cols) y veo cuales corresponden a la fila actual
		{
			j = 0;

			while(j < numFilas) //como ptr_fil.size()-1 == ind_col.size(), esto va a recorrer todo el arreglo
			{
				if (this->ind_col[j] == i) //ind_col ahora tiene los indices de las filas
				{
					col = 0;
					while(j >= col)
						col++;

					//ahora col es igual a la columna a la que pertenece el elemento.
					res[i] += ((this->val[j])*(v[col]));
				}
				j++;
			}
		}
	}

	return res;
};

int MatrizEsparsa::dimension() const {return (((this->ptr_fil).size())-1);};

void MatrizEsparsa::imprimir() const
{
	std::cout << "Valores: [";
	for (int i = 0; i < val.size(); ++i)
	{
		if (i<(val.size()-1))
			std::cout << val[i] << ", ";
		else
			std::cout << val[i] << "]" << std::endl;
	}

	std::cout << "Indices columnas: [";
	for (int i = 0; i < ind_col.size(); ++i)
	{
		if (i < (ind_col.size()-1))
			std::cout << ind_col[i] << ", ";
		else
			std::cout << ind_col[i] << "]" << std::endl;
	}

	std::cout << "Puntero fila: [";
	for (int i = 0; i < ptr_fil.size(); ++i)
	{
		if (i<(ptr_fil.size()-1))
			std::cout << ptr_fil[i] << ", ";
		else
			std::cout << ptr_fil[i] << "]" << std::endl;
	}
};

