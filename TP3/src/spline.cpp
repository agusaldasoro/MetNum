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
	for (int i = 1; i < n-1; ++i) // i=0:n si h no llega a ser siempre 2
	{
		//h = 2;
		b = ((double)3/2)*(fx[i+1]+fx[i-1]-2*fx[i]);
		sist.Elem(i,i-1) = 2;
		sist.Elem(i,i) = 8;
		sist.Elem(i,i+1) = 2;
		sist.Res(i,b);
	}
	//sist.imprimir();

	std::vector<double> sol = sist.ResolverSistema();

	std::vector<double> res(4*(n-1));
	for (int i = 0; i < n-1; ++i)
	{
		res[i*4] = fx[i]; //a
		res[(i*4)+1] = (fx[i+1]-fx[i]-((4*sol[i+1]+8*sol[i])/3))/2; //b
		res[(i*4)+2] = sol[i]; //c
		res[(i*4)+3] = (sol[i+1]-sol[i])/6; //d
	}

	/*std::cout << "Resultado: ";
	for (std::vector<double>::iterator i = res.begin(); i != res.end(); ++i)
			std::cout << *i << ", ";
	std::cout << std::endl;*/

	return res;
}

double evaluar(std::vector<double> spline, unsigned int x){
	unsigned int j = x;
	x = floor(x/2)*4;
	double a = spline[x];
	double b = spline[x+1];
	double c = spline[x+2];
	double d = spline[x+3];

	//std::cout << "a: " << a << std::endl << "b: " << b << std::endl << "c: " << c << std::endl << "d: " << d << std::endl;

	return a+b*(x-j)+c*pow((x-j),2)+d*pow((x-j),3);
}

/*void spline(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();

	std::vector<double> filaR(ancho), filaG(ancho), filaB(ancho), colR(alto), colG(alto), colB(alto);

	for (int i = 2; i < alto-2; ++i)
	{
		for (int c = 0; c < ancho; ++c)
		{
			filaR[c] = orig(c,i,0,0);
			filaG[c] = orig(c,i,0,1);
			filaB[c] = orig(c,i,0,2);
		}

		for (int j = 2; j < ancho-2; ++j)
		{
			for (int f = 0; f < alto; ++f)
			{
				colR[f] = orig(j,f,0,0);
				colG[f] = orig(j,f,0,1);
				colB[f] = orig(j,f,0,2);
			}

			if (i%2 == 1) //fila de rojos y verdes
			{
				if (j%2 == 1) //pixel rojo
				{
					//std::cout << "hola rojo" << std::endl;
					orig(j,i,0,1) = (evaluar(generar_spline(filaG),j)+evaluar(generar_spline(colG),i))/2;
					//std::cout << "chau rojo" << std::endl;
					//orig(j,i,0,2) = (evaluar(generar_spline(filaB),j)+evaluar(generar_spline(colB),i))/2;
				}
				else //pixel verde
				{
					//orig(j,i,0,0) = fmin(255.0,((orig(j-1,i,0,0)+orig(j+1,i,0,0))/2)+((double)5/8)*(orig(j,i,0,1)-(orig(j-2,i,0,1)+orig(j+2,i,0,1)-(orig(j,i-2,0,1)/2)-(orig(j,i+2,0,1)/2)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5));
					//orig(j,i,0,2) = fmin(255.0,((orig(j,i-1,0,2)+orig(j,i+1,0,2))/2)+((double)5/8)*(orig(j,i,0,1)-(-(orig(j-2,i,0,1)/2)-(orig(j+2,i,0,1)/2)+orig(j,i-2,0,1)+orig(j,i+2,0,1)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5));
				}
			}
			else //fila de azules y verdes
			{
				if (j%2 == 1) //pixel verde
				{
					//orig(j,i,0,0) = fmin(255.0,((orig(j,i-1,0,0)+orig(j,i+1,0,0))/2)+((double)5/8)*(orig(j,i,0,1)-(-(orig(j-2,i,0,1)/2)-(orig(j+2,i,0,1)/2)+orig(j,i-2,0,1)+orig(j,i+2,0,1)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5));
					//orig(j,i,0,2) = fmin(255.0,((orig(j-1,i,0,2)+orig(j+1,i,0,2))/2)+((double)5/8)*(orig(j,i,0,1)-(orig(j-2,i,0,1)+orig(j+2,i,0,1)-(orig(j,i-2,0,1)/2)-(orig(j,i+2,0,1)/2)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5));
				}
				else //pixel azul
				{
					//std::cout << "hola azul" << std::endl;
					//orig(j,i,0,0) = fmin(255.0,((orig(j-1,i-1,0,0)+orig(j+1,i+1,0,0)+orig(j+1,i-1,0,0)+orig(j-1,i+1,0,0))/4)+((double)3/4)*(orig(j,i,0,2)-(orig(j-2,i,0,2)+orig(j+2,i,0,2)+orig(j,i-2,0,2)+orig(j,i+2,0,2))/4));
					orig(j,i,0,1) = (evaluar(generar_spline(filaG),j)+evaluar(generar_spline(colG),i))/2;
					//std::cout << "chau azul" << std::endl;
				}
			}
		}
	}

	orig.crop(2,2,0,0,ancho-3,alto-3,0,2); //no se si dejarlo aca o meterlo en el main
}*/

void spline(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();

	std::vector<double> /*filaR(ancho), */filaG(ancho), /*filaB(ancho), colR(alto), */colG(alto)/*, colB(alto)*/;

	std::vector<double> spline_col, spline_fila;

	for (int i = 0; i < alto; ++i)
	{
		for (int c = ((i%2 == 0) ? 1 : 0); c < ancho; c+=2)
		{
			//filaR[c] = orig(c,i,0,0);
			filaG[c] = orig(c,i,0,1);
			//filaB[c] = orig(c,i,0,2);
		}

		spline_fila = generar_spline(filaG);

		for (int f = ((i%2 == 0) ? 2 : 1); f < ancho-1; f+=2)
		{
			orig(f,i,0,1) = evaluar(spline_fila,f);
		}
	}

	for (int j = 0; j < ancho; ++j)
	{
		for (int f = ((j%2 == 0) ? 1 : 0); f < alto; f+=2)
		{
			//filaR[c] = orig(c,i,0,0);
			filaG[f] = orig(j,f,0,1);
			//filaB[c] = orig(c,i,0,2);
		}

		spline_col = generar_spline(colG);

		for (int c = ((j%2 == 0) ? 2 : 1); c < alto-1; c+=2)
		{
			orig(j,c,0,1) = (orig(j,c,0,1)+evaluar(spline_col,c))/2;
		}
	}
	orig.crop(2,2,0,0,ancho-3,alto-3,0,2); //no se si dejarlo aca o meterlo en el main
}