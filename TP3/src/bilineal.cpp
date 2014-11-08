#include "CImg.h"
#include "bilineal.h"

void int_bilineal(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();

	for (int i = 1; i < alto-1; ++i)
	{
		for (int j = 1; j < ancho-1; ++j)
		{
			if (i%2 == 1) //fila de rojos y verdes
			{
				if (j%2 == 1) //pixel rojo
				{
					orig(j,i,0,1) = (orig(j-1,i,0,1)+orig(j+1,i,0,1)+orig(j,i-1,0,1)+orig(j,i+1,0,1))/4;
					orig(j,i,0,2) = (orig(j-1,i-1,0,2)+orig(j+1,i+1,0,2)+orig(j+1,i-1,0,2)+orig(j-1,i+1,0,2))/4;
				}
				else //pixel verde
				{
					orig(j,i,0,0) = (orig(j-1,i,0,0)+orig(j+1,i,0,0))/2;
					orig(j,i,0,2) = (orig(j,i-1,0,2)+orig(j,i+1,0,2))/2;
				}
			}
			else //fila de azules y verdes
			{
				if (j%2 == 1) //pixel verde
				{
					orig(j,i,0,2) = (orig(j-1,i,0,2)+orig(j+1,i,0,2))/2;
					orig(j,i,0,0) = (orig(j,i-1,0,0)+orig(j,i+1,0,0))/2;
				}
				else //pixel azul
				{
					orig(j,i,0,1) = (orig(j-1,i,0,1)+orig(j+1,i,0,1)+orig(j,i-1,0,1)+orig(j,i+1,0,1))/4;
					orig(j,i,0,0) = (orig(j-1,i-1,0,0)+orig(j+1,i+1,0,0)+orig(j+1,i-1,0,0)+orig(j-1,i+1,0,0))/4;
				}
			}
		}
	}

	orig.crop(1,1,0,0,ancho-2,alto-2,0,2);
}
