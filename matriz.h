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

	//SUMA, MULTIPLICACION

	const T& Elem(const Nat& fila, const Nat& columna) const;

	T& Elem(const Nat& fila, const Nat& columna);

	void Ver();

private:
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

#endif