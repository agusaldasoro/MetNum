#include "parabrisas.h"

using namespace std;

int main(int argc, char const *argv[])
{
	std::vector<Posicion> pos;

	Posicion s1;
	s1.x = 20;
	s1.y = 20;

	Posicion s2;
	s2.x = 80;
	s2.y = 80;

	Posicion s3;
	s3.x = 80;
	s3.y = 20;

	Posicion s4;
	s4.x = 20;
	s4.y = 80;

	pos.push_back(s1);
	pos.push_back(s2);
	pos.push_back(s3);
	pos.push_back(s4);

	Parabrisas hola(100,100,2.5,10,400,pos);

	//hola.VerMatriz();

	hola.CalcularTemperaturas();

	//hola.VerMatriz();



	//Matriz<float> hola(2,3);

	/*cout<< "Filas: " << hola.Filas() << endl;
	cout<< "Columnas: " << hola.Columnas() << endl;*/

	//cout<< "Elem 1,1: " << hola.Elem(1,1) << endl;

	// hola.Elem(0,0) = 0.003;
	// hola.Elem(0,1) = 59.14;
	// hola.Elem(0,2) = 59.17;
	//hola.Elem(0,3) = -100.0;

	// hola.Elem(1,0) = 5.291;
	// hola.Elem(1,1) = -6.13;
	// hola.Elem(1,2) = 46.78;
	//hola.Elem(1,3) = 0.0;

	/*hola.Elem(2,0) = 1.0;
	hola.Elem(2,1) = 9.0;
	hola.Elem(2,2) = 2.0;
	hola.Elem(2,3) = 135.0;
*/
	//cout<< "Elem 1,1: " << hola.Elem(1,1) << endl;

	// cout << "Normal:" << endl;
	// hola.Ver();
/*
	hola.EG();

	cout << "Triangulada:" << endl;
	hola.Ver();*/

	// hola.Diagonalizar();

	// cout << "Diagonalizada:" << endl;
	// hola.Ver();

	/*double c = 0.0;

	cout << c/(-1) << endl;

	cout << (c == c/(-1)) << endl;*/

	/*float h = 8.0/9.0;

	cout << h;*/

	/*std::vector<int> a(3);
	std::vector<int> b(3);

	for (int i = 0; i < a.size(); ++i)
	{
		a[i]=2;
	}

	for (int i = 0; i < b.size(); ++i)
	{
		b[i]=1;
	}*/



	return 0;
}