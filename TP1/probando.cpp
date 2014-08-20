#include "matriz.h"

using namespace std;

int main(int argc, char const *argv[])
{
	Matriz<float> hola(4,4);

	/*cout<< "Filas: " << hola.Filas() << endl;
	cout<< "Columnas: " << hola.Columnas() << endl;*/

	//cout<< "Elem 1,1: " << hola.Elem(1,1) << endl;

	hola.Elem(0,0) = 0.0;
	hola.Elem(0,1) = 1.0;
	hola.Elem(0,2) = 1.0;
	hola.Elem(0,3) = 2.0;
	hola.Elem(1,0) = 2.0;
	hola.Elem(1,1) = 0.0;
	hola.Elem(1,2) = 3.0;
	hola.Elem(1,3) = 1.0;
	hola.Elem(2,0) = 1.0;
	hola.Elem(2,1) = 0.0;
	hola.Elem(2,2) = -2.0;
	hola.Elem(2,3) = -3.0;
	hola.Elem(3,0) = -1.0;
	hola.Elem(3,1) = 1.0;
	hola.Elem(3,2) = 1.0;
	hola.Elem(3,3) = 4.0;

	//cout<< "Elem 1,1: " << hola.Elem(1,1) << endl;

	cout << "Normal:" << endl;
	hola.Ver();

	hola.EG();

	cout << "Triangulada:" << endl;
	hola.Ver();

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