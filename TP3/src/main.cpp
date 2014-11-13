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

using namespace std;

//Como ejecutar: leer README

int main(int argc, char const *argv[])
{
	if (argc < 3)
	{
		cout << "\nUso del programa: ./tp path_imagen algoritmo \n\nDonde algoritmo puede ser: \n [0] para Vecino Mas Cercano \n [1] para Bilineal \n [2] para el algoritmo de Malvar, He y Cutler (MHC) \n [3] para interpolacion mediante splines, seguida de alguna de las siguiente opciones:" << endl;
		cout << "\t 's' -metodo standard, interpolando una fila y una columna enteras \n\t 'd' -interpolacion descartando direcciones en base a las derivadas direccionales \n\t 'p' -interpolacion otorgando pesos proporcionales en base a las derivadas direccionales \n\t 'r' -interpolacion tomando un rango de 3 pixeles del mismo color \n\t 'm' -interpolacion utilizando las mejoras basadas en el algoritmo de Malvar, He y Cutler \n" << endl;
	}
	else
	{
		std::chrono::time_point<std::chrono::system_clock> start, end;

		string imagen = argv[1];
		const char * c_imagen = imagen.c_str();

		cimg_library::CImg<double> raw(c_imagen);

		generar_crudo(raw);

		string nombre_raw = (imagen.substr(0,imagen.length()-4))+"-raw.bmp";
		const char * c_nombre_raw = nombre_raw.c_str();
		raw.save(c_nombre_raw);

		if (*argv[2] == '0') //Vecino mas cercano
		{
			cout << "Aplicando demosaicing mediante vecino mas cercano..." << endl;
			start = std::chrono::system_clock::now();
			vecino(raw);
			end = std::chrono::system_clock::now();

			string nombre_vecino = (imagen.substr(0,imagen.length()-4))+"-vecino.bmp";
			const char * c_nombre_vecino = nombre_vecino.c_str();
			raw.save(c_nombre_vecino);
		}
		else if (*argv[2] == '1') //Bilineal
		{
			cout << "Aplicando demosaicing mediante interpolacion bilineal..." << endl;
			start = std::chrono::system_clock::now();
			int_bilineal(raw);
			end = std::chrono::system_clock::now();

			string nombre_bilineal = (imagen.substr(0,imagen.length()-4))+"-bilineal.bmp";
			const char * c_nombre_bilineal = nombre_bilineal.c_str();
			raw.save(c_nombre_bilineal);
		}
		else if (*argv[2] == '2') //MHC
		{
			cout << "Aplicando demosaicing mediante el algoritmo de Malvar, He y Cutler..." << endl;
			start = std::chrono::system_clock::now();
			MHC(raw);
			end = std::chrono::system_clock::now();

			string nombre_MHC = (imagen.substr(0,imagen.length()-4))+"-MHC.bmp";
			const char * c_nombre_MHC = nombre_MHC.c_str();
			raw.save(c_nombre_MHC);
		}
		else if (*argv[2] == '3') //Spline
		{
			if (argc != 4)
			{
				cout << "Es necesario especificar el tipo de spline a aplicar: \t 's' -metodo standard, interpolando una fila y una columna enteras \n\t 'd' -interpolacion descartando direcciones en base a las derivadas direccionales \n\t 'p' -interpolacion otorgando pesos proporcionales en base a las derivadas direccionales \n\t 'r' -interpolacion tomando un rango de 3 pixeles del mismo color \n\t 'm' -interpolacion utilizando las mejoras basadas en el algoritmo de Malvar, He y Cutler \n" << endl;
				return 0;
			}

			string nombre_spline;
			switch(*argv[3]){
				case 's': //standard
					cout << "Aplicando demosaicing mediante splines de una fila y una columna..." << endl;
					start = std::chrono::system_clock::now();
					spline(raw);
					end = std::chrono::system_clock::now();
					nombre_spline = (imagen.substr(0,imagen.length()-4))+"-spline.bmp";
					break;
				case 'd': //descarte mediante derivadas
					cout << "Aplicando demosaicing mediante splines: peso mediante derivadas direccionales..." << endl;
					start = std::chrono::system_clock::now();
					spline_der(raw);
					end = std::chrono::system_clock::now();
					nombre_spline = (imagen.substr(0,imagen.length()-4))+"-spline-derivadas-descarte.bmp";
					break;
				case 'p': //peso proporcional mediante derivadas
					cout << "Aplicando demosaicing mediante splines: peso proporcional mediante derivadas direccionales..." << endl;
					start = std::chrono::system_clock::now();
					spline_der_prop(raw);
					end = std::chrono::system_clock::now();
					nombre_spline = (imagen.substr(0,imagen.length()-4))+"-spline-derivadas-proporcional.bmp";
					break;
				case 'r': //rango
					cout << "Aplicando demosaicing mediante splines: rango..." << endl;
					start = std::chrono::system_clock::now();
					spline_rango(raw);
					end = std::chrono::system_clock::now();
					nombre_spline = (imagen.substr(0,imagen.length()-4))+"-spline-rango.bmp";
					break;
				case 'm': //MHC
					cout << "Aplicando demosaicing mediante splines: rango con mejoras basadas en MHC..." << endl;
					start = std::chrono::system_clock::now();
					spline_rango_MHC(raw);
					end = std::chrono::system_clock::now();
					nombre_spline = (imagen.substr(0,imagen.length()-4))+"-spline-MHC.bmp";
					break;
			}

			const char * c_nombre_spline = nombre_spline.c_str();
			raw.save(c_nombre_spline);
		}
		std::chrono::duration<double> elapsed_seconds = end-start;
		cout << "Tiempo transcurrido: " << elapsed_seconds.count() << " segundos" << endl;
	}
	return 0;
}
