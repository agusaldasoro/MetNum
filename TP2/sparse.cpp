#include "sparse.h"


MatrizEsparsa::MatrizEsparsa(int dim, int elemsNoNulos) : 
val(elemsNoNulos), ind_col(elemsNoNulos), ptr_fil(dim+1, 0), es_traspuesta(false) {};


void MatrizEsparsa::definirPos(int fila, int col, double val)
{
	int filas = ptr_fil.size();
	int indice = this->ind_col[fila];
	this->val[indice] = val; 
	/*SI NO PUEDO ASUMIR QUE VOY A INSERTAR LOS ELEMENTOS EN ORDEN EN LA MATRIZ, ESTO ESTA MAL Y 
	TENGO QUE USAR OTRA COSA (sino estaria pisando lo que hay en esa pos) */
	this->ind_col[indice] = col; //idem

	for (int i = fila+1; i < filas; ++i)
		this->ptr_fil[i] += 1;
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

		for (int i = 0; i < numFilas; ++i) 
			/*por cada fila, recorro los indices de ind_col 
			(que por estar traspuesta la matriz ahora tiene los indices de las filas, no las cols) 
			y veo cuales corresponden a la fila actual */
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

