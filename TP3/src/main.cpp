//#include <fstream>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "CImg.h"
#include <chrono>
#include "raw.h"
#include "bilineal.h"
#include "vecino.h"
#include "mhc.h"
#include "sparse.h"
#include "spline.h"

//#include <algorithm>

using namespace std;

//Como ejecutar: ./tp path_imagen tipo
//Donde tipo: 0=vecino, 1=bilineal, 2=MHC, 3,4,...,n=direccionales??

int main(int argc, char const *argv[])
{
	//std::chrono::time_point<std::chrono::system_clock> start, end;

	string imagen = argv[1];
	const char * c_imagen = imagen.c_str();

	cimg_library::CImg<double> raw(c_imagen);

	generar_crudo(raw);

	string nombre_raw = (imagen.substr(0,imagen.length()-4))+"-raw.bmp";
	const char * c_nombre_raw = nombre_raw.c_str();
	raw.save(c_nombre_raw);

	if (*argv[2] == '0') //Vecino mas cercano
	{
		//start = std::chrono::system_clock::now();
		vecino(raw);
		//end = std::chrono::system_clock::now();

		string nombre_vecino = (imagen.substr(0,imagen.length()-4))+"-vecino.bmp";
		const char * c_nombre_vecino = nombre_vecino.c_str();
		raw.save(c_nombre_vecino);
	}
	else if (*argv[2] == '1') //Bilineal
	{
		//start = std::chrono::system_clock::now();
		int_bilineal(raw);
		//end = std::chrono::system_clock::now();

		string nombre_bilineal = (imagen.substr(0,imagen.length()-4))+"-bilineal.bmp";
		const char * c_nombre_bilineal = nombre_bilineal.c_str();
		raw.save(c_nombre_bilineal);
	}
	else if (*argv[2] == '2') //MHC
	{
		//start = std::chrono::system_clock::now();
		MHC(raw);
		//end = std::chrono::system_clock::now();

		string nombre_MHC = (imagen.substr(0,imagen.length()-4))+"-MHC.bmp";
		const char * c_nombre_MHC = nombre_MHC.c_str();
		raw.save(c_nombre_MHC);
	}
	else if (*argv[2] == '3') //Spline: ESTO NO ANDA BIEN
	{
		/*Matriz test(5);
		test.Elem(0,0) = 1;
		test.Elem(1,0) = 1;
		test.Elem(1,1) = 2;
		test.Elem(1,2) = 3;
		test.Elem(2,1) = 4;
		test.Elem(2,2) = 5;
		test.Elem(2,3) = 2;
		test.Elem(3,2) = 1;
		test.Elem(3,3) = 3;
		test.Elem(3,4) = 2;
		test.Elem(4,4) = 1;

		test.Res(0,0);
		test.Res(1,5);
		test.Res(2,4);
		test.Res(3,3);
		test.Res(4,0);

		test.EG();
		test.imprimir();

		std::vector<double> res = test.ResolverSistema();
		cout << "Resultado: ";
		for (std::vector<double>::iterator i = res.begin(); i != res.end(); ++i)
			cout << *i << ", ";*/

		/*std::vector<double> fx;
		fx.push_back(130);
		fx.push_back(156);
		fx.push_back(123);
		fx.push_back(75);
		fx.push_back(97);

		std::vector<double> res = generar_spline(fx);

		cout << "Resultado: ";
		for (std::vector<double>::iterator i = res.begin(); i != res.end(); ++i)
			cout << *i << ", ";

		cout << "Evaluado: " << evaluar(res,3) << endl;*/


		//start = std::chrono::system_clock::now();
		//spline(raw);
		//spline_der(raw);
		//spline_rango(raw);
		spline_diag(raw);
		//end = std::chrono::system_clock::now();

		string nombre_spline = (imagen.substr(0,imagen.length()-4))+"-spline.bmp";
		const char * c_nombre_spline = nombre_spline.c_str();
		raw.save(c_nombre_spline);

	}

	// std::chrono::duration<double> elapsed_seconds = end-start;
	// cout << "Tiempo: " << elapsed_seconds.count() << endl;

	return 0;
}
