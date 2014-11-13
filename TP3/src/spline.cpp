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

	double b;
	for (int i = 1; i < n-1; ++i)
	{
		b = ((double)3/2)*(fx[i+1]+fx[i-1]-2*fx[i]);
		sist.Elem(i,i-1) = 2;
		sist.Elem(i,i) = 8;
		sist.Elem(i,i+1) = 2;
		sist.Res(i,b);
	}

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
				orig(i,c,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,c-1)));

				if( (i>2 && i<ancho-2) && ((fabs(orig(i-1,c,0,1)-orig(i+1,c,0,1)) > fabs(orig(i,c-1,0,1)-orig(i,c+1,0,1)))))
					orig(i,c,0,1) = fmin(255.0,fmax(0,evaluar(spline_col_g,c-1)));

				orig(i,c-1,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,c)));
			}
		}
		else
		{
			for (int c = 3; c < alto-2; c+=2)
			{
				orig(i,c-1,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,c-2)));

				if( (i>2 && i<ancho-2) && ((fabs(orig(i-1,c,0,1)-orig(i+1,c,0,1)) > fabs(orig(i,c-1,0,1)-orig(i,c+1,0,1)))))
					orig(i,c,0,1) = fmin(255.0,fmax(0,evaluar(spline_col_g,c)));

				orig(i,c,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,c)));
			}
		}
	}
	orig.crop(3,3,0,0,ancho-4,alto-4,0,2);
}

void spline_der_prop(cimg_library::CImg<double>& orig){
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
				if (i>0 && i<ancho)
				{
					double derF = fabs(orig(i-1,c,0,1)-orig(i+1,c,0,1));
					double derC = fabs(orig(i,c-1,0,1)-orig(i,c+1,0,1));
					double fact = derF+derC;
					double factorF = (fact==0) ? 0.5 : derF/fact;
					double factorC = (fact==0) ? 0.5 : derC/fact;

					orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*(1-factorF))+((1-factorC)*evaluar(spline_col_g,c-1))));
				}

				orig(i,c,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,c-1)));
				orig(i,c-1,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,c)));
			}
		}
		else
		{
			for (int c = 3; c < alto-2; c+=2)
			{
				if (i>0 && i<ancho)
				{
					double derF = fabs(orig(i-1,c,0,1)-orig(i+1,c,0,1));
					double derC = fabs(orig(i,c-1,0,1)-orig(i,c+1,0,1));
					double fact = derF+derC;
					double factorF = (fact==0) ? 0.5 : derF/fact;
					double factorC = (fact==0) ? 0.5 : derC/fact;

					orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*(1-factorF))+((1-factorC)*evaluar(spline_col_g,c-1))));
				}

				orig(i,c-1,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,c-2)));
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
					if( fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1)) )
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
					if( fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1)) )
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
					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
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
					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
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
					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) < fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
					{
						generar_rango_fila(i,j,3,0,orig,filR);
						spline_fila_r = generar_spline(filR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)));
					}

					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
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
					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) < fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
					{
						generar_rango_fila(i,j,3,2,orig,filB);
						spline_fila_b = generar_spline(filB);
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)));
					}

					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
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
					if( fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1)) )
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
					if( fabs(orig(j-1,i,0,1)-orig(j+1,i,0,1)) > fabs(orig(j,i-1,0,1)-orig(j,i+1,0,1)) )
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
					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
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
					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
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
					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) < fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
					{
						generar_rango_fila(i,j,3,0,orig,filR);
						spline_fila_r = generar_spline(filR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)+((double)5/8)*(orig(j,i,0,1)-(-(orig(j-2,i,0,1)/2)-(orig(j+2,i,0,1)/2)+orig(j,i-2,0,1)+orig(j,i+2,0,1)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5)));
					}

					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
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
					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) < fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
					{
						generar_rango_fila(i,j,3,2,orig,filB);
						spline_fila_b = generar_spline(filB);
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)+((double)5/8)*(orig(j,i,0,1)-(-(orig(j-2,i,0,1)/2)-(orig(j+2,i,0,1)/2)+orig(j,i-2,0,1)+orig(j,i+2,0,1)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5)));
					}

					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
					{
						generar_rango_col(i,j,3,0,orig,colR);
						spline_col_r = generar_spline(colR);
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,5)+((double)5/8)*(orig(j,i,0,1)-(orig(j-2,i,0,1)+orig(j+2,i,0,1)-(orig(j,i-2,0,1)/2)-(orig(j,i+2,0,1)/2)+orig(j+1,i+1,0,1)+orig(j-1,i-1,0,1)+orig(j-1,i+1,0,1)+orig(j+1,i-1,0,1))/5)));
					}
				}
			}
		}
	}
	orig.crop(5,5,0,0,ancho-6,alto-6,0,2);
}