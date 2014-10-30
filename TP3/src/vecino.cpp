#include "CImg.h"
#include "vecino.h"

//DECIDIR SI AGARRAMOS LOS ROJOS/AZULES DE ARRIBA O DE LA IZQUIERDA (codigo comentado)

void vecino(cimg_library::CImg<unsigned int>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();

	if (ancho%2 != 0)
	{
		orig.crop(0,0,0,0,ancho-2,alto,0,2);
		ancho--;
	}

	if (alto%2 != 0)
	{
		orig.crop(0,0,0,0,ancho,alto-2,0,2);
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
					orig(j,i,0,1) = orig(j-1,i,0,1);
					//orig(j,i,0,1) = orig(j,i-1,0,1);

					orig(j,i,0,2) = orig(j-1,i-1,0,2);
				}
				else //pixel verde
				{
					orig(j,i,0,0) = orig(j+1,i,0,0);
					orig(j,i,0,2) = orig(j,i-1,0,2);
				}
			}
			else //fila de azules y verdes
			{
				if (j%2 == 1) //pixel verde
				{
					orig(j,i,0,0) = orig(j,i+1,0,0);
					orig(j,i,0,2) = orig(j-1,i,0,2);
				}
				else //pixel azul
				{
					orig(j,i,0,0) = orig(j+1,i+1,0,0);

					orig(j,i,0,1) = orig(j+1,i,0,1);
					//orig(j,i,0,1) = orig(j,i+1,0,1);
				}
			}
		}
	}
}