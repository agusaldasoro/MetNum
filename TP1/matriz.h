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

	void Ver(); //BORRAR

	void EGComun();

	void EGBanda(const Nat& k);

	void ResolverSistema(char const *tipoMatriz, const Nat& k);

private:

	void Resta(std::vector<double>& a, const std::vector<double>& b);
	std::vector<double> Mult(double m, const std::vector<double>& a);

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

void Matriz::Ver()
{
	for (std::vector< std::vector<double> >::iterator i = mtrx.begin(); i != mtrx.end(); ++i)
	{
		for (std::vector<double>::iterator j = i->begin(); j != i->end(); ++j)
		{
			std::cout << *j << " ";
		}

		std::cout << std::endl;
	}
};

//PRE: Largo de a  = largo de b
void Matriz::Resta(std::vector<double>& a, const std::vector<double>& b)
{
	for (int i = 0; i < a.size(); ++i)
	{
		a[i] = a[i] - b[i];
	}
};

std::vector<double> Matriz::Mult(double m, const std::vector<double>& a)
{
	std::vector<double> b(a);

	for (int i = 0; i < a.size(); ++i)
		b[i] = m*a[i];

	return b;
}

void Matriz::EGComun()
{
	//Ver();
	int lim = col;
	if (fil<col)
		lim = fil;

	for (int i = 0; i < lim; ++i)
	{
		for (int j = i+1; j < fil; ++j)
		{
			if (fabs(Elem(i,i))<0.0001)
			{
				int r = j;
				while(fabs(Elem(i,i))>0.0001 && r<fil)
					r++;

				if (r==fil)
					j=fil;
				else
					(mtrx[i]).swap(mtrx[r]);
			}
			else
			{
				double m = Elem(j,i)/Elem(i,i);

				std::vector<double> f = Mult(m,mtrx[i]);

				Resta(mtrx[j], f);
			}
		}
	}

	//Ver();
}

void Matriz::EGBanda(const Nat& k)
{
	Nat j;
	//Recorro por filas
	for (int i = k+2; i < fil-k; ++i){
		j=0;
		int com;
		//Mientras no llego a la posicion que ocupa la diagonal:
		while(j<k+1){
			com = i + j - (k + 1);
			if(fabs(Elem(i,j)) >0.00000001){ //PARA QUE ANDE BIEN CON LA BANDA HAY QUE AGREGAR MUCHOS MAS CEROS: EXPERIMENTO!!!
				//Armo el coeficiente
				double m = Elem(i,j)/Elem(com, k+1);
				std::vector<double> f = Mult(m,mtrx[com]);

				//Empiezo a recorrer el vector desde el elemento de la diagonal, porque antes eran ceros
				//Y lo resto en la fila actual, excepto al resultado.
				int q = j;
				for (int w = k+1; w < (f.size() -1); ++w){
					Elem(i,q) -= f[w];
					q++;
				}
				Elem(i, col-1) -= f[f.size() -1];
			}
			j++;
		}
	}
};


//PRE: Recibe una matriz aumentada/ampliada
void Matriz::ResolverSistema(char const *tipo, const Nat& k)
{
	if (*tipo == '0')
	{
		this->EGComun();

		int i = fil-1; //i=filas

		while(i >= 0)
		{
			for (int j = col-2; j > i; --j)
			{
				mtrx[i][col-1] = mtrx[i][col-1] - ((mtrx[i][j])*(mtrx[j][col-1]));
			}

			mtrx[i][col-1] = ((mtrx[i][col-1])/(mtrx[i][i]));

			i--;
		}
	}
	else if (*tipo == '1')
	{
		this->EGBanda(k);

		int i = (fil-1)-(k+1);

		while(i > k){
			Nat f = 1;
			for (int j = k+2; j < col-1; ++j)
			{
				Elem(i, col-1) = Elem(i, col-1) - ((mtrx[i][j])*(mtrx[i+f][col-1]));
				f++;
			}
			Elem(i, col-1) = (Elem(i, col-1)/Elem(i, k+1));
			i--;
		}
	}
	else
		std::cout << "El tercer parámetro ingresado es erróneo: tiene que ser 0 (matriz común) o 1 (banda)" << std::endl;
}

#endif