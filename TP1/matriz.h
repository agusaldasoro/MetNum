#ifndef MATRIZ_H
#define MATRIZ_H

#include<vector>
#include<iostream>

typedef unsigned int Nat;

template<typename T>
class Matriz
{
public:
	Matriz(const Nat& filas, const Nat& columnas);

	const Nat Filas() const;

	const Nat Columnas() const;

	const T& Elem(const Nat& fila, const Nat& columna) const;

	T& Elem(const Nat& fila, const Nat& columna);

	void Ver();

	void EG();

private:

	void Resta(std::vector<T>& a, std::vector<T>& b);
	std::vector<T> Mult(double m, const std::vector<T>& a);

	Nat fil;
	Nat col;
	std::vector< std::vector< T > > mtrx;
};

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
			if (Elem(i,i)==0)
			{
				int r = j;
				while(Elem(r,i) == 0 && r<fil)
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

#endif