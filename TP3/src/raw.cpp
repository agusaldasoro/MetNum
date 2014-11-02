#include "CImg.h"
#include "raw.h"

void generar_crudo(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();

	for (int i = 0; i < alto; ++i)
	{
		for (int j = 0; j < ancho; ++j)
		{
			if (i%2 == 0) //fila de los azules y verdes
			{
				if (j%2 == 0) //columna de los azules
				{
					orig(j,i,0,0) = 0;
					orig(j,i,0,1) = 0;
				}
				else //columna de los verdes
				{
					orig(j,i,0,0) = 0;
					orig(j,i,0,2) = 0;
				}
			}
			else //fila de los rojos y verdes
			{
				if (j%2 == 0) //columna de los verdes
				{
					orig(j,i,0,0) = 0;
					orig(j,i,0,2) = 0;
				}
				else //columna de los rojos
				{
					orig(j,i,0,1) = 0;
					orig(j,i,0,2) = 0;
				}
			}
		}
	}
}