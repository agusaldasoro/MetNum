/*#include <fstream>
#include <stdlib.h>
//#include <string>
#include <iostream>
//#include "sparse.h"
#include "CImg.h"
//#include <limits>
#include <chrono>

using namespace std;

int main(int argc, char const *argv[])
{
	//std::chrono::time_point<std::chrono::system_clock> start, end;

	cimg_library::CImg<unsigned int> raw(800,600,1,3,0);

	ifstream main(argv[1]);
	unsigned int valor;
	for (int i = 0; i < 600; ++i)
	{
		for (int j = 0; j < 800; ++j)
		{
			main >> valor;
			if (i%2 == 0)
			{
				if (j%2 == 0)
					raw(j,i,0,2) = valor;
				else
					raw(j,i,0,1) = valor;
			}
			else
			{
				if (j%2 == 0)
					raw(j,i,0,1) = valor;
				else
					raw(j,i,0,0) = valor;
			}
		}
	}

	for (int i = 0; i < 600; ++i)
	{
		for (int j = 0; j < 800; ++j)
		{
			if (i%2 == 1)
			{
				if (j%2 == 1) //rojo
				{
					if (j<400)
						raw(j,i,0,1) = raw(j+1,i,0,1);
					else
						raw(j,i,0,1) = raw(j-1,i,0,1);

					if (i<598 && j<798)
						raw(j,i,0,2) = raw(j+1,i+1,0,2);
				}
				else //verde
				{
					if (j<400)
						raw(j,i,0,0) = raw(j+1,i,0,0);
					else
						raw(j,i,0,0) = raw(j-1,i,0,0);

					if (i<300)
						raw(j,i,0,2) = raw(j,i+1,0,2);
					else
						raw(j,i,0,2) = raw(j,i-1,0,2);
				}
			}
			else
			{
				if (j%2 == 1) //verde
				{
					if (i<300)
						raw(j,i,0,0) = raw(j,i+1,0,0);
					else
						raw(j,i,0,0) = raw(j,i-1,0,0);

					if (j<400)
						raw(j,i,0,2) = raw(j+1,i,0,2);
					else
						raw(j,i,0,2) = raw(j-1,i,0,2);
				}
				else //azul
				{
					if (j<798 && i<598)
						raw(j,i,0,0) = raw(j+1,i+1,0,0);

					if (j<400)
						raw(j,i,0,1) = raw(j+1,i,0,1);
					else
						raw(j,i,0,1) = raw(j-1,i,0,1);
				}
			}
		}
	}

	raw.save("vecino.bmp");

	// std::chrono::duration<double> elapsed_seconds = end-start;
	// cout << "Tiempo: " << elapsed_seconds.count() << endl;

	return 0;
}*/