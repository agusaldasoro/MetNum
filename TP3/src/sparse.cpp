#include "sparse.h"

Matriz::Matriz(const Nat& dim) : _dim(dim), _mtrx(dim)
{
	for (std::vector< std::vector<double> >::iterator i = _mtrx.begin(); i != _mtrx.end(); ++i)
	{
		i->resize(4);
	};
}

//PRE: solo puedo preguntar por elementos que esten en alguna de las tres diagonales (dentro del contexto de uso del TP, esto siempre se cumple)
double& Matriz::Elem(const Nat& fila, const Nat& columna)
{
	return _mtrx[fila][columna-fila+1];
};

void Matriz::Res(const Nat& fila, const double& res){
	_mtrx[fila][3] = res;
}

void Matriz::EG()
{
	for (int i = 1; i < _dim-1; ++i)
	{
		if(fabs(_mtrx[i-1][1]) > 0.00001)
		{
			double f = (_mtrx[i][0])/(_mtrx[i-1][1]);
			_mtrx[i][1] -= (_mtrx[i-1][2])*f;
			_mtrx[i][3] -= (_mtrx[i-1][3])*f;
		}
	}
};

//PRE: Recibe una matriz aumentada/ampliada
std::vector<double> Matriz::ResolverSistema()
{
	this->EG();
	std::vector<double> res(_dim);
	res[0] = res[_dim-1] = 0;

	for (int i = this->_dim-2; i > 0; --i)
		if (_mtrx[i][1] != 0) //hace falta?
			res[i] = (_mtrx[i][3]-(_mtrx[i][2]*res[i+1]))/_mtrx[i][1];

	return res;
}

/*void Matriz::imprimir(){
	for (int i = 0; i < _dim; ++i)
	{
		for (int j = 0; j < 4; ++j)
			std::cout << this->_mtrx[i][j] << ", ";

		std::cout << std::endl;
	}
}*/