#include <fstream>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "CImg.h"
#include <chrono>

using namespace std;

int main(int argc, char const *argv[])
{
	//std::chrono::time_point<std::chrono::system_clock> start, end;

	string imagen = argv[1];
	const char * c_imagen = imagen.c_str();

	cimg_library::CImg<unsigned int> raw(c_imagen);

	unsigned int ancho = raw.width();
	unsigned int alto = raw.height();

	for (int i = 0; i < alto; ++i)
	{
		for (int j = 0; j < ancho; ++j)
		{
			if (i%2 == 0) //fila de los azules y verdes
			{
				if (j%2 == 0) //columna de los azules
				{
					raw(j,i,0,0) = 0;
					raw(j,i,0,1) = 0;
				}
				else //columna de los verdes
				{
					raw(j,i,0,0) = 0;
					raw(j,i,0,2) = 0;
				}
			}
			else //fila de los rojos y verdes
			{
				if (j%2 == 0) //columna de los verdes
				{
					raw(j,i,0,0) = 0;
					raw(j,i,0,2) = 0;
				}
				else //columna de los rojos
				{
					raw(j,i,0,1) = 0;
					raw(j,i,0,2) = 0;
				}
			}
		}
	}

	string nombre_raw = (imagen.substr(0,imagen.length()-4))+"-raw.bmp";
	const char * c_nombre_raw = nombre_raw.c_str();
	raw.save(c_nombre_raw);

	//start = std::chrono::system_clock::now();

	if (ancho%2 != 0)
	{
		raw.crop(0,0,0,0,ancho-2,alto,0,2);
		ancho--;
	}

	if (alto%2 != 0)
	{
		raw.crop(0,0,0,0,ancho,alto-2,0,2);
		alto--;
	}

	for (int i = 0; i < alto; ++i)
	{
		for (int j = 0; j < ancho; ++j)
		{
			if (i%2 == 1) //fila de rojos y verdes
			{
				if (j%2 == 1) //pixel rojo
				{
					raw(j,i,0,1) = raw(j-1,i,0,1);
					//raw(j,i,0,1) = raw(j,i-1,0,1);

					raw(j,i,0,2) = raw(j-1,i-1,0,2);
				}
				else //pixel verde
				{
					raw(j,i,0,0) = raw(j+1,i,0,0);
					raw(j,i,0,2) = raw(j,i-1,0,2);
				}
			}
			else //fila de azules y verdes
			{
				if (j%2 == 1) //pixel verde
				{
					raw(j,i,0,0) = raw(j,i+1,0,0);
					raw(j,i,0,2) = raw(j-1,i,0,2);
				}
				else //pixel azul
				{
					raw(j,i,0,0) = raw(j+1,i+1,0,0);

					raw(j,i,0,1) = raw(j+1,i,0,1);
					//raw(j,i,0,1) = raw(j,i+1,0,1);
				}
			}
		}
	}

	//end = std::chrono::system_clock::now();

	string nombre_vecino = (imagen.substr(0,imagen.length()-4))+"-vecino.bmp";
	const char * c_nombre_vecino = nombre_vecino.c_str();
	raw.save(c_nombre_vecino);

	// std::chrono::duration<double> elapsed_seconds = end-start;
	// cout << "Tiempo: " << elapsed_seconds.count() << endl;

	return 0;
}