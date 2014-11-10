#include <iostream>
#include "CImg.h"
#include "spline.h"
#include "sparse.h"
#include <vector>

std::vector<double> generar_spline(std::vector<double> fx){
//hay tantos puntos x como elementos en fx, y se consideran enumerados de 0 a vals.size().
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
		res[(i*4)+1] = ((fx[i+1]-fx[i])/2)-(((2*sol[i+1])+(4*sol[i]))/3); //despejo b
		res[(i*4)+2] = sol[i]; //c
		res[(i*4)+3] = (sol[i+1]-sol[i])/6; //despejo d
	}

	return res;
}

double evaluar(std::vector<double> spline, unsigned int x){
	unsigned int j = floor(x/2); //veo cuál es el intervalo en el que cae mi punto a evaluar para elegir el polinomio Sj correspondiente
	unsigned int i = j*4; //lo multiplico por 4 porque por cada polinomio tengo 4 elementos en el vector

	double a = spline[i];
	double b = spline[i+1];
	double c = spline[i+2];
	double d = spline[i+3];

	j *= 2; //multiplico por 2 porque la distancia entre punto y punto es 2, y se numeran desde 0: es decir, si tengo que usar el polinomio S_j=S_3
	//mi X_3 sería 2*h, y como h=2 -> X_3 = 4
	return a+b*(x-j)+c*pow((x-j),2)+d*pow((x-j),3);
}

void spline(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();
	std::vector<double> filaR, filaG, filaB, colR, colG, colB;
	std::vector<double> spline_col_r, spline_col_g, spline_col_b, spline_fila_r, spline_fila_g, spline_fila_b;

	for (int i = 0; i < alto; ++i)
	{
		filaR.clear();
		filaG.clear();
		filaB.clear();

		if (i%2==0)
		{
			for (int c = 1; c < ancho; c+=2)
			{
				filaG.push_back(orig(c,i,0,1));
				filaB.push_back(orig(c-1,i,0,2));
			}
			
			spline_fila_g = generar_spline(filaG);
			spline_fila_b = generar_spline(filaB);

			for (int c = 2; c < ancho-1; c+=2)
			{
				orig(c,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,c-1)));
				orig(c-1,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,c)));
			}
		}
		else
		{
			for (int c = 0; c < ancho; c+=2)
			{
				filaG.push_back(orig(c,i,0,1));
				filaR.push_back(orig(c+1,i,0,0));
			}
			
			spline_fila_g = generar_spline(filaG);
			spline_fila_r = generar_spline(filaR);

			for (int c = 3; c < ancho-1; c+=2)
			{
				orig(c-1,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,c-2)));
				orig(c,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,c)));
			}
		}
	}

	for (int i = 0; i < ancho; ++i)
	{
		colR.clear();
		colG.clear();
		colB.clear();

		if (i%2==0)
		{
			for (int c = 1; c < alto; c+=2)
			{
				colR.push_back(orig(i,c,0,0));
				colG.push_back(orig(i,c,0,1));
				colB.push_back(orig(i,c-1,0,2));
			}
		}
		else
		{
			for (int c = 0; c < alto; c+=2)
			{
				colB.push_back(orig(i,c,0,2));
				colG.push_back(orig(i,c,0,1));
				colR.push_back(orig(i,c+1,0,0));
			}
		}

		spline_col_g = generar_spline(colG);
		spline_col_b = generar_spline(colB);
		spline_col_r = generar_spline(colR);

		if (i%2==0)
		{
			for (int c = 2; c < alto-1; c+=2)
			{
				orig(i,c,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,c-1)));
				orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)+evaluar(spline_col_g,c-1))/2));
				orig(i,c-1,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,c)));
			}
		}
		else
		{
			for (int c = 3; c < alto-1; c+=2)
			{
				orig(i,c-1,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,c-2)));
				orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)+evaluar(spline_col_g,c))/2));
				orig(i,c,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,c)));
			}
		}
	}

	orig.crop(3,3,0,0,ancho-4,alto-4,0,2);
}

void spline_der(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();
	std::vector<double> filaR, filaG, filaB, colR, colG, colB;
	std::vector<double> spline_col_r, spline_col_g, spline_col_b, spline_fila_r, spline_fila_g, spline_fila_b;

	for (int i = 0; i < alto; ++i)
	{
		filaR.clear();
		filaG.clear();
		filaB.clear();

		if (i%2==0)
		{
			for (int c = 1; c < ancho; c+=2)
			{
				filaG.push_back(orig(c,i,0,1));
				filaB.push_back(orig(c-1,i,0,2));
			}

			spline_fila_g = generar_spline(filaG);
			spline_fila_b = generar_spline(filaB);

			for (int c = 2; c < ancho-1; c+=2)
			{
				orig(c,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,c-1)));
				orig(c-1,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,c)));
			}
		}
		else
		{
			for (int c = 0; c < ancho; c+=2)
			{
				filaG.push_back(orig(c,i,0,1));
				filaR.push_back(orig(c+1,i,0,0));
			}

			spline_fila_g = generar_spline(filaG);
			spline_fila_r = generar_spline(filaR);

			for (int c = 3; c < ancho-1; c+=2)
			{
				orig(c-1,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,c-2)));
				orig(c,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,c)));
			}
		}
	}

	for (int i = 0; i < ancho; ++i)
	{
		colR.clear();
		colG.clear();
		colB.clear();

		if (i%2==0)
		{
			for (int c = 1; c < alto; c+=2)
			{
				colR.push_back(orig(i,c,0,0));
				colG.push_back(orig(i,c,0,1));
				colB.push_back(orig(i,c-1,0,2));
			}
		}
		else
		{
			for (int c = 0; c < alto; c+=2)
			{
				colB.push_back(orig(i,c,0,2));
				colG.push_back(orig(i,c,0,1));
				colR.push_back(orig(i,c+1,0,0));
			}
		}

		spline_col_g = generar_spline(colG);
		spline_col_b = generar_spline(colB);
		spline_col_r = generar_spline(colR);

		if (i%2==0)
		{
			for (int c = 2; c < alto-2; c+=2)
			{
				// if (i>0 && i<ancho)
				// {
				// 	double derF = fabs(orig(i-1,c,0,1)-orig(i+1,c,0,1));
				// 	double derC = fabs(orig(i,c-1,0,1)-orig(i,c+1,0,1));
				// 	double fact = derF+derC;
				// 	double factorF = (fact==0) ? 0.5 : derF/fact;
				// 	double factorC = (fact==0) ? 0.5 : derC/fact;

				// 	orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*(1-factorF))+((1-factorC)*evaluar(spline_col_g,c-1))));
				// }

				orig(i,c,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,c-1)));
				if( (i>2 && i<ancho-2) && ((fabs(orig(i-1,c,0,1)-orig(i+1,c,0,1)) > fabs(orig(i,c-1,0,1)-orig(i,c+1,0,1))) || (fabs(orig(i-2,c,0,2)+orig(i+2,c,0,2)) > fabs(orig(i,c-2,0,2)+orig(i,c+2,0,2)) )))
					orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*0)+(1*evaluar(spline_col_g,c-1))));
				else
					orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*1)+(0*evaluar(spline_col_g,c-1))));
				orig(i,c-1,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,c)));
			}
		}
		else
		{
			for (int c = 3; c < alto-2; c+=2)
			{
				// if (i>0 && i<ancho)
				// {
				// 	double derF = fabs(orig(i-1,c,0,1)-orig(i+1,c,0,1));
				// 	double derC = fabs(orig(i,c-1,0,1)-orig(i,c+1,0,1));
				// 	double fact = derF+derC;
				// 	double factorF = (fact==0) ? 0.5 : derF/fact;
				// 	double factorC = (fact==0) ? 0.5 : derC/fact;

				// 	orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*(1-factorF))+((1-factorC)*evaluar(spline_col_g,c-1))));
				// }

				orig(i,c-1,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,c-2)));
				if( (i>2 && i<ancho-2) && ((fabs(orig(i-1,c,0,1)-orig(i+1,c,0,1)) > fabs(orig(i,c-1,0,1)-orig(i,c+1,0,1))) || (fabs(orig(i-2,c,0,0)+orig(i+2,c,0,0)) > fabs(orig(i,c-2,0,0)+orig(i,c+2,0,0)) )))
					orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*0)+(1*evaluar(spline_col_g,c))));
				else
					orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*1)+(0*evaluar(spline_col_g,c))));
				orig(i,c,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,c)));
			}
		}		
	}
	orig.crop(3,3,0,0,ancho-4,alto-4,0,2);
}

void generar_rango_fila(unsigned int fila, unsigned int columna, unsigned int rango, unsigned int canal, cimg_library::CImg<double>& orig, std::vector<double>& x){
	rango += (rango-1);
	x.clear();

	for (int i = 0; i < rango; i+=2)
		x.push_back(orig(columna-(rango-i),fila,0,canal));

	for (int i = rango-1; i >= 0; i-=2)
		x.push_back(orig(columna+(rango-i),fila,0,canal));
}

void generar_rango_col(unsigned int fila, unsigned int columna, unsigned int rango, unsigned int canal, cimg_library::CImg<double>& orig, std::vector<double>& x){
	rango += (rango-1);
	x.clear();

	for (int i = 0; i < rango; i+=2)
		x.push_back(orig(columna,fila-(rango-i),0,canal));

	for (int i = rango-1; i >= 0; i-=2)
		x.push_back(orig(columna,fila+(rango-i),0,canal));
}

//FIJARSE SI DEJAR LOS IFS ("MULTIPLICAR POR 1 Y 0") O EL PROMEDIO/DARLE PESOS: decidir en base a PSNR y SSIM de spline_der (cual es mejor? 1y0 o pesos?)
void spline_rango(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();
	std::vector<double> filR, filG, filB, colR, colG, colB;
	std::vector<double> spline_col_r, spline_col_g, spline_col_b, spline_fila_r, spline_fila_g, spline_fila_b;

	for (int i = 5; i < alto-5; ++i)
	{
		for (int j = 5; j < ancho-5; ++j)
		{
			if (i%2==0)
			{
				if (j%2==0) //pixel azul
				{
					//orig(j,i,0,1) = fmin(255.0,fmax(0,(evaluar(spline_fila_g,5)+evaluar(spline_col_g,5))/2));

					if( fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1)) )
					//if( (fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1))) || (orig(j-2,i,0,2)+orig(j+2,i,0,2) > orig(j,i-2,0,2)+orig(j,i+2,0,2)) )
					{
						generar_rango_col(i,j,3,1,orig,colG);
						spline_col_g = generar_spline(colG);
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_col_g,5)));
					}
					else
					{
						generar_rango_fila(i,j,3,1,orig,filG);
						spline_fila_g = generar_spline(filG);
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,5)));
					}
				}
				else //pixel verde
				{

					generar_rango_fila(i,j,3,2,orig,filB);
					spline_fila_b = generar_spline(filB);
					orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)));

					generar_rango_col(i,j,3,0,orig,colR);
					spline_col_r = generar_spline(colR);
					orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,5)));
				}
			}
			else
			{
				if (j%2==1) //pixel rojo
				{
					//orig(j,i,0,1) = fmin(255.0,fmax(0,(evaluar(spline_fila_g,5)+evaluar(spline_col_g,5))/2));

					if( fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1)) )
					//if( (fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1))) || (orig(j-2,i,0,0)+orig(j+2,i,0,0) > orig(j,i-2,0,0)+orig(j,i+2,0,0)) )
					{
						generar_rango_col(i,j,3,1,orig,colG);
						spline_col_g = generar_spline(colG);
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_col_g,5)));
					}
					else
					{
						generar_rango_fila(i,j,3,1,orig,filG);
						spline_fila_g = generar_spline(filG);
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,5)));
					}
				}
				else //pixel verde
				{
					generar_rango_fila(i,j,3,0,orig,filR);
					spline_fila_r = generar_spline(filR);
					orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)));

					generar_rango_col(i,j,3,2,orig,colB);
					spline_col_b = generar_spline(colB);
					orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,5)));
				}
			}
		}
	}

	for (int i = 5; i < alto-5; ++i)
	{
		for (int j = 5; j < ancho-5; ++j)
		{
			if (i%2==0)
			{
				if (j%2==0) //pixel azul
				{
					//orig(j,i,0,0) = fmin(255.0,fmax(0,(evaluar(spline_fila_r,5)+evaluar(spline_col_r,5))/2));

					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
					//if( ((fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0))) || (fabs(orig(j-2,i,0,2)+orig(j+2,i,0,2)) > fabs(orig(j,i-2,0,2)+orig(j,i+2,0,2)) )))
					{
						generar_rango_col(i,j,3,0,orig,colR);
						spline_col_r = generar_spline(colR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,5)));
					}
					else
					{
						generar_rango_fila(i,j,3,0,orig,filR);
						spline_fila_r = generar_spline(filR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)));
					}
				}
			}
			else
			{
				if (j%2==1) //pixel rojo
				{
					//orig(j,i,0,2) = fmin(255.0,fmax(0,(evaluar(spline_fila_b,5)+evaluar(spline_col_b,5))/2));

					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
					//if( ((fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2))) || (fabs(orig(j-2,i,0,0)+orig(j+2,i,0,0)) > fabs(orig(j,i-2,0,0)+orig(j,i+2,0,0)) )))
					{
						generar_rango_col(i,j,3,2,orig,colB);
						spline_col_b = generar_spline(colB);
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,5)));
					}
					else
					{
						generar_rango_fila(i,j,3,2,orig,filB);
						spline_fila_b = generar_spline(filB);
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)));
					}
				}
			}
		}
	}

	for (int i = 5; i < alto-5; ++i)
	{
		for (int j = 5; j < ancho-5; ++j)
		{
			if (i%2==0)
			{
				if (j%2==1) //pixel verde
				{
					//orig(j,i,0,0) = fmin(255.0,fmax(0,(orig(j,i,0,0)+evaluar(spline_fila_r,5))/2));
					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) < fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
					//if( ((fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) < fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0))) || (fabs(orig(j-2,i,0,1)+orig(j+2,i,0,1)) < fabs(orig(j,i-2,0,1)+orig(j,i+2,0,1)) )))
					{
						generar_rango_fila(i,j,3,0,orig,filR);
						spline_fila_r = generar_spline(filR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)));
					}

					//orig(j,i,0,2) = fmin(255.0,fmax(0,(orig(j,i,0,2)+evaluar(spline_col_b,5))/2));
					//if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
					if( ((fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2))) || (fabs(orig(j-2,i,0,1)+orig(j+2,i,0,1)) > fabs(orig(j,i-2,0,1)+orig(j,i+2,0,1)) )))
					{
						generar_rango_col(i,j,3,2,orig,colB);
						spline_col_b = generar_spline(colB);
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,5)));
					}
				}
			}
			else
			{
				if (j%2==0) //pixel verde
				{
					//orig(j,i,0,2) = fmin(255.0,fmax(0,(orig(j,i,0,2)+evaluar(spline_fila_b,5))/2));
					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) < fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
					//if( ((fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) < fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2))) || (fabs(orig(j-2,i,0,1)+orig(j+2,i,0,1)) < fabs(orig(j,i-2,0,1)+orig(j,i+2,0,1)) )))
					{
						generar_rango_fila(i,j,3,2,orig,filB);
						spline_fila_b = generar_spline(filB);
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)));
					}

					//orig(j,i,0,0) = fmin(255.0,fmax(0,(orig(j,i,0,0)+evaluar(spline_col_r,5))/2));
					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
					//if( ((fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0))) || (fabs(orig(j-2,i,0,1)+orig(j+2,i,0,1)) > fabs(orig(j,i-2,0,1)+orig(j,i+2,0,1)) )))
					{
						generar_rango_col(i,j,3,0,orig,colR);
						spline_col_r = generar_spline(colR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,5)));
					}
				}
			}
		}
	}

	orig.crop(5,5,0,0,ancho-6,alto-6,0,2);
}

//IDEM
void spline_rango_MHC(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();
	std::vector<double> filR, filG, filB, colR, colG, colB;
	std::vector<double> spline_col_r, spline_col_g, spline_col_b, spline_fila_r, spline_fila_g, spline_fila_b;

	for (int i = 5; i < alto-5; ++i)
	{
		for (int j = 5; j < ancho-5; ++j)
		{
			if (i%2==0)
			{
				if (j%2==0) //pixel azul
				{
					//orig(j,i,0,1) = fmin(255.0,fmax(0,(evaluar(spline_fila_g,5)+evaluar(spline_col_g,5))/2));

					if( fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1)) )
					//if( (fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1))) || (orig(j-2,i,0,2)+orig(j+2,i,0,2) > orig(j,i-2,0,2)+orig(j,i+2,0,2)) )
					{
						generar_rango_col(i,j,3,1,orig,colG);
						spline_col_g = generar_spline(colG);
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_col_g,5)+((double)1/2)*(orig(j,i,0,2)-((double)1/4)*(orig(j-2,i,0,2)+orig(j+2,i,0,2)+orig(j,i-2,0,2)+orig(j,i+2,0,2)))));
					}
					else
					{
						generar_rango_fila(i,j,3,1,orig,filG);
						spline_fila_g = generar_spline(filG);
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,5)+((double)1/2)*(orig(j,i,0,2)-((double)1/4)*(orig(j-2,i,0,2)+orig(j+2,i,0,2)+orig(j,i-2,0,2)+orig(j,i+2,0,2)))));
					}
				}
				else //pixel verde
				{
					generar_rango_fila(i,j,3,2,orig,filB);
					spline_fila_b = generar_spline(filB);
					orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)));

					generar_rango_col(i,j,3,0,orig,colR);
					spline_col_r = generar_spline(colR);
					orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,5)));
				}
			}
			else
			{
				if (j%2==1) //pixel rojo
				{
					//orig(j,i,0,1) = fmin(255.0,fmax(0,(evaluar(spline_fila_g,5)+evaluar(spline_col_g,5))/2));

					if( fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1)) )
					//if( (fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1))) || (orig(j-2,i,0,0)+orig(j+2,i,0,0) > orig(j,i-2,0,0)+orig(j,i+2,0,0)) )
					{
						generar_rango_col(i,j,3,1,orig,colG);
						spline_col_g = generar_spline(colG);
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_col_g,5)+((double)1/2)*(orig(j,i,0,0)-((double)1/4)*(orig(j-2,i,0,0)+orig(j+2,i,0,0)+orig(j,i-2,0,0)+orig(j,i+2,0,0)))));
					}
					else
					{
						generar_rango_fila(i,j,3,1,orig,filG);
						spline_fila_g = generar_spline(filG);
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,5)+((double)1/2)*(orig(j,i,0,0)-((double)1/4)*(orig(j-2,i,0,0)+orig(j+2,i,0,0)+orig(j,i-2,0,0)+orig(j,i+2,0,0)))));
					}
				}
				else //pixel verde
				{
					generar_rango_fila(i,j,3,0,orig,filR);
					spline_fila_r = generar_spline(filR);
					orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)));

					generar_rango_col(i,j,3,2,orig,colB);
					spline_col_b = generar_spline(colB);
					orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,5)));
				}
			}
		}
	}

	for (int i = 5; i < alto-5; ++i)
	{
		for (int j = 5; j < ancho-5; ++j)
		{
			if (i%2==0)
			{
				if (j%2==0) //pixel azul
				{
					//orig(j,i,0,0) = fmin(255.0,fmax(0,(evaluar(spline_fila_r,5)+evaluar(spline_col_r,5))/2));

					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
					//if( ((fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0))) || (fabs(orig(j-2,i,0,2)+orig(j+2,i,0,2)) > fabs(orig(j,i-2,0,2)+orig(j,i+2,0,2)) )))
					{
						generar_rango_col(i,j,3,0,orig,colR);
						spline_col_r = generar_spline(colR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,5)));
					}
					else
					{
						generar_rango_fila(i,j,3,0,orig,filR);
						spline_fila_r = generar_spline(filR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)));
					}
				}
			}
			else
			{
				if (j%2==1) //pixel rojo
				{
					//orig(j,i,0,2) = fmin(255.0,fmax(0,(evaluar(spline_fila_b,5)+evaluar(spline_col_b,5))/2));

					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
					//if( ((fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2))) || (fabs(orig(j-2,i,0,0)+orig(j+2,i,0,0)) > fabs(orig(j,i-2,0,0)+orig(j,i+2,0,0)) )))
					{
						generar_rango_col(i,j,3,2,orig,colB);
						spline_col_b = generar_spline(colB);
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,5)+((double)3/4)*(orig(j,i,0,0)-(orig(j-2,i,0,0)+orig(j+2,i,0,0)+orig(j,i-2,0,0)+orig(j,i+2,0,0))/4)));
					}
					else
					{
						generar_rango_fila(i,j,3,2,orig,filB);
						spline_fila_b = generar_spline(filB);
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)+((double)3/4)*(orig(j,i,0,0)-(orig(j-2,i,0,0)+orig(j+2,i,0,0)+orig(j,i-2,0,0)+orig(j,i+2,0,0))/4)));
					}
				}
			}
		}
	}

	for (int i = 5; i < alto-5; ++i)
	{
		for (int j = 5; j < ancho-5; ++j)
		{
			if (i%2==0)
			{
				if (j%2==1) //pixel verde
				{
					//orig(j,i,0,0) = fmin(255.0,fmax(0,(orig(j,i,0,0)+evaluar(spline_fila_r,5))/2));
					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) < fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
					//if( ((fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) < fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0))) || (fabs(orig(j-2,i,0,1)+orig(j+2,i,0,1)) < fabs(orig(j,i-2,0,1)+orig(j,i+2,0,1)) )))
					{
						generar_rango_fila(i,j,3,0,orig,filR);
						spline_fila_r = generar_spline(filR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)+((double)5/8)*(orig(j,i,0,1)-(-(orig(j-2,i,0,1)/2)-(orig(j+2,i,0,1)/2)+orig(j,i-2,0,1)+orig(j,i+2,0,1)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5)));
					}

					//orig(j,i,0,2) = fmin(255.0,fmax(0,(orig(j,i,0,2)+evaluar(spline_col_b,5))/2));
					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
					//if( ((fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2))) || (fabs(orig(j-2,i,0,1)+orig(j+2,i,0,1)) > fabs(orig(j,i-2,0,1)+orig(j,i+2,0,1)) )))
					{
						generar_rango_col(i,j,3,2,orig,colB);
						spline_col_b = generar_spline(colB);
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,5)+((double)5/8)*(orig(j,i,0,1)-(orig(j-2,i,0,1)+orig(j+2,i,0,1)-(orig(j,i-2,0,1)/2)-(orig(j,i+2,0,1)/2)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5)));
					}
				}
			}
			else
			{
				if (j%2==0) //pixel verde
				{
					//orig(j,i,0,2) = fmin(255.0,fmax(0,(orig(j,i,0,2)+evaluar(spline_fila_b,5))/2));
					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) < fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
					//if( ((fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) < fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2))) || (fabs(orig(j-2,i,0,1)+orig(j+2,i,0,1)) < fabs(orig(j,i-2,0,1)+orig(j,i+2,0,1)) )))
					{
						generar_rango_fila(i,j,3,2,orig,filB);
						spline_fila_b = generar_spline(filB);
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)+((double)5/8)*(orig(j,i,0,1)-(-(orig(j-2,i,0,1)/2)-(orig(j+2,i,0,1)/2)+orig(j,i-2,0,1)+orig(j,i+2,0,1)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5)));
					}

					//orig(j,i,0,0) = fmin(255.0,fmax(0,(orig(j,i,0,0)+evaluar(spline_col_r,5))/2));
					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
					//if( ((fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0))) || (fabs(orig(j-2,i,0,1)+orig(j+2,i,0,1)) > fabs(orig(j,i-2,0,1)+orig(j,i+2,0,1)) )))
					{
						generar_rango_col(i,j,3,0,orig,colR);
						spline_col_r = generar_spline(colR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,5)+((double)5/8)*(orig(j,i,0,1)-(orig(j-2,i,0,1)+orig(j+2,i,0,1)-(orig(j,i-2,0,1)/2)-(orig(j,i+2,0,1)/2)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5)));
					}
				}
			}
		}
	}

	orig.crop(5,5,0,0,ancho-6,alto-6,0,2); //no se si dejarlo aca o meterlo en el main
}

void spline_diag(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();
	std::vector<double> diagCrecR, filG, diagCrecB, diagDecR, colG, diagDecB, colR, filR, colB, filB;
	std::vector<double> spline_diagc_r, spline_col_g, spline_diagc_b, spline_diagd_r, spline_fila_g, spline_diagd_b, spline_fila_b, spline_fila_r, spline_col_r, spline_col_b;

	for (int i = 5; i < alto-5; ++i)
	{
		for (int j = 5; j < ancho-5; ++j)
		{
			if (i%2==0)
			{
				if (j%2==0) //pixel azul
				{
					filG.clear();
					colG.clear();

					filG.push_back(orig(j-5,i,0,1));
					filG.push_back(orig(j-3,i,0,1));
					filG.push_back(orig(j-1,i,0,1));
					filG.push_back(orig(j+1,i,0,1));
					filG.push_back(orig(j+3,i,0,1));
					filG.push_back(orig(j+5,i,0,1));

					colG.push_back(orig(j,i-5,0,1));
					colG.push_back(orig(j,i-3,0,1));
					colG.push_back(orig(j,i-1,0,1));
					colG.push_back(orig(j,i+1,0,1));
					colG.push_back(orig(j,i+3,0,1));
					colG.push_back(orig(j,i+5,0,1));

					spline_fila_g = generar_spline(filG);
					spline_col_g = generar_spline(colG);

					//orig(j,i,0,1) = fmin(255.0,fmax(0,(evaluar(spline_fila_g,5)+evaluar(spline_col_g,5))/2));

					if( fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1)) )
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_col_g,5)));
					else
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,5)));

					diagCrecR.clear();
					diagDecR.clear();

					diagCrecR.push_back(orig(j-5,i-5,0,0));
					diagCrecR.push_back(orig(j-3,i-3,0,0));
					diagCrecR.push_back(orig(j-1,i-1,0,0));
					diagCrecR.push_back(orig(j+1,i+1,0,0));
					diagCrecR.push_back(orig(j+3,i+3,0,0));
					diagCrecR.push_back(orig(j+5,i+5,0,0));

					diagDecR.push_back(orig(j-5,i+5,0,0));
					diagDecR.push_back(orig(j-3,i+3,0,0));
					diagDecR.push_back(orig(j-1,i+1,0,0));
					diagDecR.push_back(orig(j+1,i-1,0,0));
					diagDecR.push_back(orig(j+3,i-3,0,0));
					diagDecR.push_back(orig(j+5,i-5,0,0));

					spline_diagc_r = generar_spline(diagCrecR);
					spline_diagd_r = generar_spline(diagDecR);

					if( fabs(orig(j-1,i+1,0,0)-orig(j+1,i-1,0,0)) > fabs(orig(j-1,i-1,0,0)-orig(j+1,i+1,0,0)) )
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_diagc_r,5)));
					else
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_diagd_r,5)));

				}
				else //pixel verde
				{
					filB.clear();

					filB.push_back(orig(j-5,i,0,2));
					filB.push_back(orig(j-3,i,0,2));
					filB.push_back(orig(j-1,i,0,2));
					filB.push_back(orig(j+1,i,0,2));
					filB.push_back(orig(j+3,i,0,2));
					filB.push_back(orig(j+5,i,0,2));

					spline_fila_b = generar_spline(filB);

					orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)));

					colR.clear();

					colR.push_back(orig(j,i-5,0,0));
					colR.push_back(orig(j,i-3,0,0));
					colR.push_back(orig(j,i-1,0,0));
					colR.push_back(orig(j,i+1,0,0));
					colR.push_back(orig(j,i+3,0,0));
					colR.push_back(orig(j,i+5,0,0));

					spline_col_r = generar_spline(colR);

					orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,5)));
				}
			}
			else
			{
				if (j%2==1) //pixel rojo
				{
					filG.clear();
					colG.clear();

					colG.push_back(orig(j,i-5,0,1));
					colG.push_back(orig(j,i-3,0,1));
					colG.push_back(orig(j,i-1,0,1));
					colG.push_back(orig(j,i+1,0,1));
					colG.push_back(orig(j,i+3,0,1));
					colG.push_back(orig(j,i+5,0,1));

					filG.push_back(orig(j-5,i,0,1));
					filG.push_back(orig(j-3,i,0,1));
					filG.push_back(orig(j-1,i,0,1));
					filG.push_back(orig(j+1,i,0,1));
					filG.push_back(orig(j+3,i,0,1));
					filG.push_back(orig(j+5,i,0,1));

					spline_fila_g = generar_spline(filG);
					spline_col_g = generar_spline(colG);

					//orig(j,i,0,1) = fmin(255.0,fmax(0,(evaluar(spline_fila_g,5)+evaluar(spline_col_g,5))/2));

					if( fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1)) )
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_col_g,5)));
					else
						orig(j,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,5)));

					diagCrecB.clear();
					diagDecB.clear();

					diagCrecB.push_back(orig(j-5,i-5,0,2));
					diagCrecB.push_back(orig(j-3,i-3,0,2));
					diagCrecB.push_back(orig(j-1,i-1,0,2));
					diagCrecB.push_back(orig(j+1,i+1,0,2));
					diagCrecB.push_back(orig(j+3,i+3,0,2));
					diagCrecB.push_back(orig(j+5,i+5,0,2));

					diagDecB.push_back(orig(j-5,i+5,0,2));
					diagDecB.push_back(orig(j-3,i+3,0,2));
					diagDecB.push_back(orig(j-1,i+1,0,2));
					diagDecB.push_back(orig(j+1,i-1,0,2));
					diagDecB.push_back(orig(j+3,i-3,0,2));
					diagDecB.push_back(orig(j+5,i-5,0,2));

					spline_diagc_b = generar_spline(diagCrecB);
					spline_diagd_b = generar_spline(diagDecB);

					if( fabs(orig(j-1,i+1,0,2)-orig(j+1,i-1,0,2)) > fabs(orig(j-1,i-1,0,2)-orig(j+1,i+1,0,2)) )
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_diagc_b,5)));
					else
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_diagd_b,5)));
				}
				else //pixel verde
				{
					filR.clear();

					filR.push_back(orig(j-5,i,0,0));
					filR.push_back(orig(j-3,i,0,0));
					filR.push_back(orig(j-1,i,0,0));
					filR.push_back(orig(j+1,i,0,0));
					filR.push_back(orig(j+3,i,0,0));
					filR.push_back(orig(j+5,i,0,0));

					spline_fila_r = generar_spline(filR);

					orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)));

					colB.clear();

					colB.push_back(orig(j,i-5,0,2));
					colB.push_back(orig(j,i-3,0,2));
					colB.push_back(orig(j,i-1,0,2));
					colB.push_back(orig(j,i+1,0,2));
					colB.push_back(orig(j,i+3,0,2));
					colB.push_back(orig(j,i+5,0,2));

					spline_col_b = generar_spline(colB);

					orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,5)));
				}
			}
		}
	}

	//fijarse si interpolando otra vez el ojo y el azul para los pixeles verdes (por fila) cambia algo

	orig.crop(5,5,0,0,ancho-5,alto-5,0,2); //no se si dejarlo aca o meterlo en el main
}