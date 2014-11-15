#ifndef MATRIZ_H
#define MATRIZ_H

#include<vector>
#include<iostream>
#include <cmath>

typedef unsigned int Nat;

class Matriz
{
public:
	Matriz();

	Matriz(const Nat& filas, const Nat& columnas);

	const Nat& Filas() const;

	const Nat& Columnas() const;

	const double& Elem(const Nat& fila, const Nat& columna) const;

	double& Elem(const Nat& fila, const Nat& columna);

	void Redimensionar(const Nat& filas, const Nat& columnas); //ESTA FUNCION ES NECESARIA PARA REDIMENSIONAR LA MATRIZ (por si es banda o no, al llamarla desde parabrisas.h)

	void EGComun(const Nat& k);

	void EGBanda(const Nat& k);

	void ResolverSistema(char const *tipoMatriz, const Nat& k);

private:
	Nat fil;
	Nat col;
	std::vector< std::vector<double> > mtrx;
};

Matriz::Matriz() {};

Matriz::Matriz(const Nat& filas, const Nat& columnas) : fil(filas), col(columnas), mtrx(fil)
{
	for (std::vector< std::vector<double> >::iterator i = mtrx.begin(); i != mtrx.end(); ++i)
	{
		i->resize(col);
	};
}

const Nat& Matriz::Filas() const
{
	return this->fil;
};

const Nat& Matriz::Columnas() const
{
	return this->col;
};

const double& Matriz::Elem(const Nat& fila, const Nat& columna) const
{
	return mtrx[fila][columna];
};

double& Matriz::Elem(const Nat& fila, const Nat& columna)
{
	return mtrx[fila][columna];
};

void Matriz::Redimensionar(const Nat& filas, const Nat& columnas)
{
	fil = filas;
	col = columnas;

	mtrx.resize(filas);
	for (std::vector< std::vector<double> >::iterator i = mtrx.begin(); i != mtrx.end(); ++i)
	{
		i->resize(columnas);
	}
}

void Matriz::EGComun(const Nat& k)
{
	int a;
	double m;
	int lim = col;
	if (fil<col)
		lim = fil; //para triangular, la matriz tiene que ser cuadrada,
				   //pero en el caso de recibir una matriz ampliada, la ultima columna no se tiene en cuenta

	for (int i = 0; i < lim; ++i) //recorro las columnas
	{
		for (int j = i+1; j < fil-k; ++j) //recorro las filas
		{
			if (fabs(mtrx[j][i]) > 0.00000000001) //si el elemento no es cero...
			{//armo el coeficiente
				m = mtrx[j][i]/mtrx[i][i];
				a = i;
				while(a<col) //resto las filas, multiplicando por el coeficiente
				{
					if (fabs(mtrx[i][a]) > 0.00000000001)
						mtrx[j][a] -= (m*(mtrx[i][a]));
					a++;
				}
			}
		}
	}
}

void Matriz::EGBanda(const Nat& k)
{
	Nat j;
	//Recorro por filas
	for (int i = k+2; i < fil-k; ++i){
		j=0;
		int com;
		//Mientras no llego a la posicion que ocupa la diagonal (es decir, la posicion "del medio" de la matriz):
		while(j<k+1){
			com = i + j - (k + 1);
			if(fabs(Elem(i,j)) > 0.00000000001){ //si el elemento no es cero
				//Armo el coeficiente
				double m = Elem(i,j)/Elem(com, k+1);

				//Empiezo a recorrer el vector desde el elemento de la diagonal, porque antes eran ceros
				//Y lo resto en la fila actual, excepto al resultado.
				int q = j;
				for (int w = k+1; w < (col-1); ++w){
					Elem(i,q) -= (m*(mtrx[com][w]));
					q++;
				}
				Elem(i, col-1) -= (m*(mtrx[com][col -1]));
			}
			j++;
		}
	}
};


//PRE: Recibe una matriz aumentada/ampliada
void Matriz::ResolverSistema(char const *tipo, const Nat& k)
{
	if (*tipo == '0') //si la estructura de la matriz es la convencional
	{
		this->EGComun(k);

		int i = fil-1; //i=filas

		while(i >= 0) //recorro de abajo para arriba las filas
		{
			for (int j = col-2; j > i; --j) //y las columnas desde la derecha hasta la diagonal
			{
				mtrx[i][col-1] = mtrx[i][col-1] - ((mtrx[i][j])*(mtrx[j][col-1])); //y voy restandole a la posicion i del vector independiente,
			}																	  //los valores ya despejados de las incognitas previas, multiplicados por el elem(i,j) (su coeficiente)

			mtrx[i][col-1] = ((mtrx[i][col-1])/(mtrx[i][i])); //finalmente, al llegar al valor de la diagonal, "paso dividiendo" el coeficiente para terminar de despejar X_i

			i--;
		}
	}
	else if (*tipo == '1') //si la estructura de la matriz es banda
	{
		this->EGBanda(k);

		int i = (fil-1)-(k+1); //voy ignorar recorrer las ultimas k filas porque son la identidad y no hay nada que despejar

		while(i > k){ //tambien voy a ignorar recorrer las primeras k filas por la misma razon
			Nat f = 1;
			for (int j = k+2; j < col-1; ++j) //recorro las columnas desde la posicion siguiente a la diagonal, y hacia la derecha, hasta el final
			{
				Elem(i, col-1) = Elem(i, col-1) - ((mtrx[i][j])*(mtrx[i+f][col-1])); //y despejo de la misma forma que antes, solo que de izquierda a derecha en lugar de derecha a izquierda
				f++;
			}
			Elem(i, col-1) = (Elem(i, col-1)/Elem(i, k+1)); //finalmente, al igual que antes, divido al resultado por el valor de la diagonal
			i--;
		}
	}
	else
		std::cout << "El tercer parámetro ingresado es erróneo: tiene que ser 0 (matriz común) o 1 (banda)" << std::endl;
}

#endif