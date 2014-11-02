#include <iostream>
#include "CImg.h"
#include "spline.h"
#include "sparse.h"
#include <vector>

std::vector<double> generar_spline(std::vector<double> fx){ //hay tantos puntos x como elementos en fx, y se consideran enumerados de 0 a vals.size().
																			//i.e: vals.size=3 := 0,1,2, y se generarán 2 polinomios (8 coefs).
	unsigned int n = fx.size();
	Matriz sist(n);
	sist.Elem(0,0) = 1;
	sist.Res(0,0);
	sist.Elem(n-1,n-1) = 1;
	sist.Res(n-1,0);

	//double h;
	double b;
	for (int i = 1; i < n-1; ++i) // i=0:n si h no llega a ser siempre 1
	{
		//h = i+1 - i;
		b = 3*(fx[i+1]+fx[i+1]);
		sist.Elem(i,i-1) = 1;
		sist.Elem(i,i) = 4;
		sist.Elem(i,i+1) = 1;
		sist.Res(i,b);
	}
	sist.imprimir();

	std::vector<double> sol = sist.ResolverSistema();

	std::vector<double> res(4*(n-1));
	for (int i = 0; i < n-1; ++i)
	{
		res[i*4] = fx[i]; //a
		res[(i*4)+1] = fx[i+1]-fx[i]-((sol[i+1]+2*sol[i])/3); //b
		res[(i*4)+2] = sol[i]; //c
		res[(i*4)+3] = (sol[i+1]-sol[i])/3; //d
	}

	std::cout << "Resultado: ";
	for (std::vector<double>::iterator i = res.begin(); i != res.end(); ++i)
			std::cout << *i << ", ";
	std::cout << std::endl;

	return res;
}



void spline(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();

	for (int i = 2; i < alto-2; ++i)
	{
		for (int j = 2; j < ancho-2; ++j)
		{
			if (i%2 == 1) //fila de rojos y verdes
			{
				if (j%2 == 1) //pixel rojo
				{
					orig(j,i,0,1) = fmin(255.0,((orig(j-1,i,0,1)+orig(j+1,i,0,1)+orig(j,i-1,0,1)+orig(j,i+1,0,1))/4)+((double)1/2)*(orig(j,i,0,0)-((double)1/4)*(orig(j-2,i,0,0)+orig(j+2,i,0,0)+orig(j,i-2,0,0)+orig(j,i+2,0,0))));
					orig(j,i,0,2) = fmin(255.0,((orig(j-1,i-1,0,2)+orig(j+1,i+1,0,2)+orig(j+1,i-1,0,2)+orig(j-1,i+1,0,2))/4)+((double)3/4)*(orig(j,i,0,0)-(orig(j-2,i,0,0)+orig(j+2,i,0,0)+orig(j,i-2,0,0)+orig(j,i+2,0,0))/4));
				}
				else //pixel verde
				{
					orig(j,i,0,0) = fmin(255.0,((orig(j-1,i,0,0)+orig(j+1,i,0,0))/2)+((double)5/8)*(orig(j,i,0,1)-(orig(j-2,i,0,1)+orig(j+2,i,0,1)-(orig(j,i-2,0,1)/2)-(orig(j,i+2,0,1)/2)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5));
					orig(j,i,0,2) = fmin(255.0,((orig(j,i-1,0,2)+orig(j,i+1,0,2))/2)+((double)5/8)*(orig(j,i,0,1)-(-(orig(j-2,i,0,1)/2)-(orig(j+2,i,0,1)/2)+orig(j,i-2,0,1)+orig(j,i+2,0,1)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5));
				}
			}
			else //fila de azules y verdes
			{
				if (j%2 == 1) //pixel verde
				{
					orig(j,i,0,0) = fmin(255.0,((orig(j,i-1,0,0)+orig(j,i+1,0,0))/2)+((double)5/8)*(orig(j,i,0,1)-(-(orig(j-2,i,0,1)/2)-(orig(j+2,i,0,1)/2)+orig(j,i-2,0,1)+orig(j,i+2,0,1)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5));
					orig(j,i,0,2) = fmin(255.0,((orig(j-1,i,0,2)+orig(j+1,i,0,2))/2)+((double)5/8)*(orig(j,i,0,1)-(orig(j-2,i,0,1)+orig(j+2,i,0,1)-(orig(j,i-2,0,1)/2)-(orig(j,i+2,0,1)/2)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5));
				}
				else //pixel azul
				{
					orig(j,i,0,0) = fmin(255.0,((orig(j-1,i-1,0,0)+orig(j+1,i+1,0,0)+orig(j+1,i-1,0,0)+orig(j-1,i+1,0,0))/4)+((double)3/4)*(orig(j,i,0,2)-(orig(j-2,i,0,2)+orig(j+2,i,0,2)+orig(j,i-2,0,2)+orig(j,i+2,0,2))/4));
					orig(j,i,0,1) = fmin(255.0,((orig(j-1,i,0,1)+orig(j+1,i,0,1)+orig(j,i-1,0,1)+orig(j,i+1,0,1))/4)+(((double)1/2)*(orig(j,i,0,2)-((orig(j-2,i,0,2)+orig(j+2,i,0,2)+orig(j,i-2,0,2)+orig(j,i+2,0,2))/4))));
				}
			}
		}
	}

	orig.crop(2,2,0,0,ancho-3,alto-3,0,2); //no se si dejarlo aca o meterlo en el main
}