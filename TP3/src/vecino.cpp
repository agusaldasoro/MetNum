#include "CImg.h"
#include "vecino.h"

//DECIDIR SI AGARRAMOS LOS ROJOS/AZULES DE ARRIBA O DE LA IZQUIERDA (codigo comentado)

void vecino(cimg_library::CImg<double>& orig){
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
					orig(j,i,0,1) = orig(j-1,i,0,1); //arriba
					// orig(j,i,0,1) = orig(j+1,i,0,1); //abajo
					// orig(j,i,0,1) = orig(j,i-1,0,1); //izquierda
					// orig(j,i,0,1) = orig(j,i+1,0,1); //derecha

					orig(j,i,0,2) = orig(j-1,i-1,0,2); //arriba izquierda
					// orig(j,i,0,2) = orig(j+1,i-1,0,2); //arriba derecha
					// orig(j,i,0,2) = orig(j-1,i+1,0,2); //abajo izquierda
					// orig(j,i,0,2) = orig(j+1,i+1,0,2); //abajo derecha
				}
				else //pixel verde
				{
					orig(j,i,0,0) = orig(j+1,i,0,0); //derecha
					//orig(j,i,0,0) = orig(j-1,i,0,0); //izquierda

					orig(j,i,0,2) = orig(j,i-1,0,2); //arriba
					//orig(j,i,0,2) = orig(j,i+1,0,2); //abajo
				}
			}
			else //fila de azules y verdes
			{
				if (j%2 == 1) //pixel verde
				{
					orig(j,i,0,0) = orig(j,i-1,0,0); //arriba
					//orig(j,i,0,0) = orig(j,i+1,0,0); //abajo

					orig(j,i,0,2) = orig(j-1,i,0,2); //izquierda
					//orig(j,i,0,2) = orig(j+1,i,0,2); //derecha
				}
				else //pixel azul
				{
					orig(j,i,0,0) = orig(j+1,i+1,0,0); //abajo derecha
					// orig(j,i,0,0) = orig(j-1,i+1,0,0); //abajo izquierda
					// orig(j,i,0,0) = orig(j+1,i-1,0,0); //arriba derecha
					// orig(j,i,0,0) = orig(j-1,i-1,0,0); //arriba izquierda

					orig(j,i,0,1) = orig(j+1,i,0,1); //derecha
					// orig(j,i,0,1) = orig(j-1,i,0,1); //izquierda
					// orig(j,i,0,1) = orig(j,i-1,0,1); //arriba
					// orig(j,i,0,1) = orig(j,i+1,0,1); //abajo
				}
			}
		}
	}
	orig.crop(1,1,0,0,ancho-2,alto-2,0,2);
}