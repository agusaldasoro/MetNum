#ifndef MATRIZ_H
#define MATRIZ_H

#include<vector>
#include<iostream>
#include <cmath>

typedef unsigned int Nat;

template<typename T>
class Matriz
{
public:
	//Matriz();

	Matriz(const Nat& filas, const Nat& columnas);

	const Nat Filas() const;

	const Nat Columnas() const;

	const T& Elem(const Nat& fila, const Nat& columna) const;

	T& Elem(const Nat& fila, const Nat& columna);

	void Ver();

	void EG();

	void Diagonalizar();

private:

	void Resta(std::vector<T>& a, std::vector<T>& b);
	std::vector<T> Mult(double m, const std::vector<T>& a);

	Nat fil;
	Nat col;
	std::vector< std::vector< T > > mtrx;
};

/*template<typename T>
Matriz<T>::Matriz() {};*/

template<typename T>
Matriz<T>::Matriz(const Nat& filas, const Nat& columnas) : fil(filas), col(columnas), mtrx(fil)
{
	for (typename std::vector< std::vector< T > >::iterator i = mtrx.begin(); i != mtrx.end(); ++i)
	{
		i->resize(col);
	};
}

template<typename T>
const Nat Matriz<T>::Filas() const
{
	return this->fil;
};

template<typename T>
const Nat Matriz<T>::Columnas() const
{
	return this->col;
};

template<typename T>
const T& Matriz<T>::Elem(const Nat& fila, const Nat& columna) const
{
	return mtrx[fila][columna];
};

template<typename T>
T& Matriz<T>::Elem(const Nat& fila, const Nat& columna)
{
	return mtrx[fila][columna];
};

template<typename T>
void Matriz<T>::Ver()
{
	for (typename std::vector< std::vector< T > >::iterator i = mtrx.begin(); i != mtrx.end(); ++i)
	{
		for (typename std::vector< T >::iterator j = i->begin(); j != i->end(); ++j)
		{
			std::cout << *j << " ";
		}

		std::cout << std::endl;
	}
};

//PRE: Largo de a  = largo de b
template<typename T>
void Matriz<T>::Resta(std::vector<T>& a, std::vector<T>& b)
{
	for (int i = 0; i < a.size(); ++i)
	{
		a[i] = a[i] - b[i];
	}
};

template<typename T>
std::vector<T> Matriz<T>::Mult(double m, const std::vector<T>& a)
{
	std::vector<T> b(a);

	for (int i = 0; i < a.size(); ++i)
		{
			b[i] = m*a[i];
		}

	return b;
}

template<typename T>
void Matriz<T>::EG()
{
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

				std::vector<T> f = Mult(m,mtrx[i]);

				Resta(mtrx[j], f);
			}
		}
	}
}


//PRE: Recibe una matriz aumentada/ampliada (sino, se obtiene la identidad (en el caso de ser LI))
template<typename T>
void Matriz<T>::Diagonalizar()
{
	this->EG();

	int i = 0;

	while(i<fil) //voy recorriendo la diagonal, y divido las filas por lo que corresponda para dejar 1s
	{
		double d = Elem(i,i);

		if (fabs(d)>=0.0001)
			mtrx[i] = Mult(1/d, mtrx[i]);

		++i;
	}

	i--; //ahora i = fil, que se va del rango, por eso resto 1

	while(i >= 0) //y ahora resto de abajo para arriba para diagonalizar
	{
		for (int j = i-1; j >= 0; --j) //con j recorro cada columna de abajo hacia arriba, empezando por i, que ahora es la de más a la derecha (sin contar la columna de resultados, claramente)
		{
			if (fabs(Elem(j,i))>=0.0001)
			{
				double m = Elem(j,i);

				std::vector<T> f = Mult(m,mtrx[i]);

				Resta(mtrx[j], f);
			}
		}
		--i;
	}
}

#endif