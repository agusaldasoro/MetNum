//#include <fstream>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "CImg.h"
#include <chrono>
#include "raw.h"
#include "bilineal.h"
#include "vecino.h"
//#include "MHC.h"

using namespace std;

//Como ejecutar: ./tp path_imagen tipo
//Donde tipo: 0=vecino, 1=bilineal, 2=MHC, 3,4,...,n=direccionales??

int main(int argc, char const *argv[])
{
	//std::chrono::time_point<std::chrono::system_clock> start, end;

	string imagen = argv[1];
	const char * c_imagen = imagen.c_str();

	cimg_library::CImg<unsigned int> raw(c_imagen);

	generar_crudo(raw);

	string nombre_raw = (imagen.substr(0,imagen.length()-4))+"-raw.bmp";
	const char * c_nombre_raw = nombre_raw.c_str();
	raw.save(c_nombre_raw);

	if (*argv[2] == '0')
	{
		//start = std::chrono::system_clock::now();
		vecino(raw);
		//end = std::chrono::system_clock::now();

		string nombre_vecino = (imagen.substr(0,imagen.length()-4))+"-vecino.bmp";
		const char * c_nombre_vecino = nombre_vecino.c_str();
		raw.save(c_nombre_vecino);
	}
	else if (*argv[2] == '1')
	{
		//start = std::chrono::system_clock::now();
		int_bilineal(raw);
		//end = std::chrono::system_clock::now();

		string nombre_bilineal = (imagen.substr(0,imagen.length()-4))+"-bilineal.bmp";
		const char * c_nombre_bilineal = nombre_bilineal.c_str();
		raw.save(c_nombre_bilineal);
	}
	else if (*argv[2] == '2')
	{
		//start = std::chrono::system_clock::now();
		//MHC(raw);
		//end = std::chrono::system_clock::now();

		string nombre_MHC = (imagen.substr(0,imagen.length()-4))+"-MHC.bmp";
		const char * c_nombre_MHC = nombre_MHC.c_str();
		raw.save(c_nombre_MHC);
	}

	// std::chrono::duration<double> elapsed_seconds = end-start;
	// cout << "Tiempo: " << elapsed_seconds.count() << endl;

	return 0;
}
