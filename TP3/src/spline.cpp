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
		res[(i*4)+1] = ((fx[i+1]-fx[i])/2)-(((2*sol[i+1])+(4*sol[i]))/3); //b
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
	unsigned int j = floor(x/2);
	unsigned int i = j*4;

	double a = spline[i];
	double b = spline[i+1];
	double c = spline[i+2];
	double d = spline[i+3];

	//std::cout << "a: " << a << std::endl << "b: " << b << std::endl << "c: " << c << std::endl << "d: " << d << std::endl;
	j *= 2;
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
		}
		else
		{
			for (int c = 0; c < ancho; c+=2)
			{
				filaG.push_back(orig(c,i,0,1));
				filaR.push_back(orig(c+1,i,0,0));
			}
		}

		spline_fila_g = generar_spline(filaG);
		if (i%2==0)
			spline_fila_b = generar_spline(filaB);
		else
			spline_fila_r = generar_spline(filaR);

		if (i%2==0)
		{
			for (int c = 2; c < ancho-1; c+=2)
			{
				orig(c,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,c-1)));
				orig(c-1,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,c)));
			}
		}
		else
		{
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

	//BETA:
	/*for (int i = 0; i < alto; ++i)
	{
		filaR.clear();
		filaB.clear();

		if (i%2==0)
			for (int c = 1; c < ancho; c+=2)
				filaR.push_back(orig(c,i,0,0));
		else
			for (int c = 0; c < ancho; c+=2)
				filaB.push_back(orig(c,i,0,2));

		if (i%2==0)
			spline_fila_r = generar_spline(filaR);
		else
			spline_fila_b = generar_spline(filaB);

		if (i%2==0)
			for (int c = 2; c < ancho-1; c+=2)
				orig(c,i,0,0) = fmin(255.0,fmax(0,(orig(c,i,0,0)+evaluar(spline_fila_r,c-1))/2));
		else
			for (int c = 3; c < ancho-1; c+=2)
				orig(c,i,0,2) = fmin(255.0,fmax(0,(orig(c,i,0,2)+evaluar(spline_fila_b,c))/2));
	}*/

	orig.crop(3,3,0,0,ancho-4,alto-4,0,2); //no se si dejarlo aca o meterlo en el main
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
		}
		else
		{
			for (int c = 0; c < ancho; c+=2)
			{
				filaG.push_back(orig(c,i,0,1));
				filaR.push_back(orig(c+1,i,0,0));
			}
		}

		spline_fila_g = generar_spline(filaG);
		if (i%2==0)
			spline_fila_b = generar_spline(filaB);
		else
			spline_fila_r = generar_spline(filaR);

		if (i%2==0)
		{
			for (int c = 2; c < ancho-1; c+=2)
			{
				orig(c,i,0,1) = fmin(255.0,fmax(0,evaluar(spline_fila_g,c-1)));
				orig(c-1,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,c)));
			}
		}
		else
		{
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
				if( (i>0 && i<ancho) && ((fabs(orig(i-1,c,0,1)-orig(i+1,c,0,1)) > fabs(orig(i,c-1,0,1)-orig(i,c+1,0,1))) || (fabs(orig(i-1,c,0,0)-orig(i+1,c,0,0)) > fabs(orig(i,c-1,0,2)-orig(i,c+1,0,2)))) )
					orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*0)+(1*evaluar(spline_col_g,c-1))));
				else
					orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*1)+(0*evaluar(spline_col_g,c-1))));
				orig(i,c-1,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,c)));
			}
		}
		else
		{
			for (int c = 3; c < alto-1; c+=2)
			{
				orig(i,c-1,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,c-2)));
				if( (i>0 && i<ancho) && ((fabs(orig(i-1,c,0,1)-orig(i+1,c,0,1)) > fabs(orig(i,c-1,0,1)-orig(i,c+1,0,1))) || (fabs(orig(i-1,c,0,2)-orig(i+1,c,0,2)) > fabs(orig(i,c-1,0,0)-orig(i,c+1,0,0)))) )
					orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*0)+(1*evaluar(spline_col_g,c))));
				else
					orig(i,c,0,1) = fmin(255.0,fmax(0,(orig(i,c,0,1)*1)+(0*evaluar(spline_col_g,c))));
				orig(i,c,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,c)));
			}
		}
	}

	orig.crop(3,3,0,0,ancho-4,alto-4,0,2); //no se si dejarlo aca o meterlo en el main
}

void spline_rango(cimg_library::CImg<double>& orig){
	unsigned int ancho = orig.width();
	unsigned int alto = orig.height();

	std::vector<double> filR, filG, filB, colR, colG, colB;

	std::vector<double> spline_col_r, spline_col_g, spline_col_b, spline_fila_r, spline_fila_g, spline_fila_b;

	for (int i = 5; i < alto-5; ++i)
	{
		std::cout << "Fila " << i << std::endl;

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

	for (int i = 5; i < alto-5; ++i)
	{
		std::cout << "Fila " << i << std::endl;

		for (int j = 5; j < ancho-5; ++j)
		{
			if (i%2==0)
			{
				if (j%2==0) //pixel azul
				{
					filR.clear();
					colR.clear();

					colR.push_back(orig(j,i-5,0,0));
					colR.push_back(orig(j,i-3,0,0));
					colR.push_back(orig(j,i-1,0,0));
					colR.push_back(orig(j,i+1,0,0));
					colR.push_back(orig(j,i+3,0,0));
					colR.push_back(orig(j,i+5,0,0));

					filR.push_back(orig(j-5,i,0,0));
					filR.push_back(orig(j-3,i,0,0));
					filR.push_back(orig(j-1,i,0,0));
					filR.push_back(orig(j+1,i,0,0));
					filR.push_back(orig(j+3,i,0,0));
					filR.push_back(orig(j+5,i,0,0));

					spline_fila_r = generar_spline(filR);
					spline_col_r = generar_spline(colR);

					//orig(j,i,0,0) = fmin(255.0,fmax(0,(evaluar(spline_fila_r,5)+evaluar(spline_col_r,5))/2));

					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,5)));
					else
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)));
				}
				/*else //pixel verde
				{
					filR.clear();

					filR.push_back(orig(j-5,i,0,0));
					filR.push_back(orig(j-3,i,0,0));
					filR.push_back(orig(j-1,i,0,0));
					filR.push_back(orig(j+1,i,0,0));
					filR.push_back(orig(j+3,i,0,0));
					filR.push_back(orig(j+5,i,0,0));

					spline_fila_r = generar_spline(filR);

					orig(j,i,0,0) = fmin(255.0,fmax(0,(orig(j,i,0,0)+evaluar(spline_fila_r,5))/2));

					colB.clear();

					colB.push_back(orig(j,i-5,0,2));
					colB.push_back(orig(j,i-3,0,2));
					colB.push_back(orig(j,i-1,0,2));
					colB.push_back(orig(j,i+1,0,2));
					colB.push_back(orig(j,i+3,0,2));
					colB.push_back(orig(j,i+5,0,2));

					spline_col_b = generar_spline(colB);

					orig(j,i,0,2) = fmin(255.0,fmax(0,(orig(j,i,0,2)+evaluar(spline_col_b,5))/2));
				}*/
			}
			else
			{
				if (j%2==1) //pixel rojo
				{
					filB.clear();
					colB.clear();

					colB.push_back(orig(j,i-5,0,2));
					colB.push_back(orig(j,i-3,0,2));
					colB.push_back(orig(j,i-1,0,2));
					colB.push_back(orig(j,i+1,0,2));
					colB.push_back(orig(j,i+3,0,2));
					colB.push_back(orig(j,i+5,0,2));

					filB.push_back(orig(j-5,i,0,2));
					filB.push_back(orig(j-3,i,0,2));
					filB.push_back(orig(j-1,i,0,2));
					filB.push_back(orig(j+1,i,0,2));
					filB.push_back(orig(j+3,i,0,2));
					filB.push_back(orig(j+5,i,0,2));

					spline_fila_b = generar_spline(filB);
					spline_col_b = generar_spline(colB);

					//orig(j,i,0,2) = fmin(255.0,fmax(0,(evaluar(spline_fila_b,5)+evaluar(spline_col_b,5))/2));

					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,5)));
					else
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)));
				}
				/*else //pixel verde
				{
					filB.clear();

					filB.push_back(orig(j-5,i,0,2));
					filB.push_back(orig(j-3,i,0,2));
					filB.push_back(orig(j-1,i,0,2));
					filB.push_back(orig(j+1,i,0,2));
					filB.push_back(orig(j+3,i,0,2));
					filB.push_back(orig(j+5,i,0,2));

					spline_fila_b = generar_spline(filB);

					orig(j,i,0,2) = fmin(255.0,fmax(0,(orig(j,i,0,2)+evaluar(spline_fila_b,5))/2));

					colR.clear();

					colR.push_back(orig(j,i-5,0,0));
					colR.push_back(orig(j,i-3,0,0));
					colR.push_back(orig(j,i-1,0,0));
					colR.push_back(orig(j,i+1,0,0));
					colR.push_back(orig(j,i+3,0,0));
					colR.push_back(orig(j,i+5,0,0));

					spline_col_r = generar_spline(colR);

					orig(j,i,0,0) = fmin(255.0,fmax(0,(orig(j,i,0,0)+evaluar(spline_col_r,5))/2));
				}*/
			}
		}
	}

	//BETA:

	for (int i = 5; i < alto-5; ++i)
	{
		std::cout << "Fila " << i << std::endl;

		for (int j = 5; j < ancho-5; ++j)
		{
			if (i%2==0)
			{
				if (j%2==1) //pixel verde
				{
					filR.clear();

					filR.push_back(orig(j-5,i,0,0));
					filR.push_back(orig(j-3,i,0,0));
					filR.push_back(orig(j-1,i,0,0));
					filR.push_back(orig(j+1,i,0,0));
					filR.push_back(orig(j+3,i,0,0));
					filR.push_back(orig(j+5,i,0,0));

					spline_fila_r = generar_spline(filR);

					//orig(j,i,0,0) = fmin(255.0,fmax(0,(orig(j,i,0,0)+evaluar(spline_fila_r,5))/2));
					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) < fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_fila_r,5)));

					colB.clear();

					colB.push_back(orig(j,i-5,0,2));
					colB.push_back(orig(j,i-3,0,2));
					colB.push_back(orig(j,i-1,0,2));
					colB.push_back(orig(j,i+1,0,2));
					colB.push_back(orig(j,i+3,0,2));
					colB.push_back(orig(j,i+5,0,2));

					spline_col_b = generar_spline(colB);

					//orig(j,i,0,2) = fmin(255.0,fmax(0,(orig(j,i,0,2)+evaluar(spline_col_b,5))/2));
					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) > fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_col_b,5)));
				}
			}
			else
			{
				if (j%2==0) //pixel verde
				{
					filB.clear();

					filB.push_back(orig(j-5,i,0,2));
					filB.push_back(orig(j-3,i,0,2));
					filB.push_back(orig(j-1,i,0,2));
					filB.push_back(orig(j+1,i,0,2));
					filB.push_back(orig(j+3,i,0,2));
					filB.push_back(orig(j+5,i,0,2));

					spline_fila_b = generar_spline(filB);

					//orig(j,i,0,2) = fmin(255.0,fmax(0,(orig(j,i,0,2)+evaluar(spline_fila_b,5))/2));
					if( fabs(orig(j-1,i,0,2)-orig(j+1,i,0,2)) < fabs(orig(j,i-1,0,2)-orig(j,i+1,0,2)) )
						orig(j,i,0,2) = fmin(255.0,fmax(0,evaluar(spline_fila_b,5)));

					colR.clear();

					colR.push_back(orig(j,i-5,0,0));
					colR.push_back(orig(j,i-3,0,0));
					colR.push_back(orig(j,i-1,0,0));
					colR.push_back(orig(j,i+1,0,0));
					colR.push_back(orig(j,i+3,0,0));
					colR.push_back(orig(j,i+5,0,0));

					spline_col_r = generar_spline(colR);

					//orig(j,i,0,0) = fmin(255.0,fmax(0,(orig(j,i,0,0)+evaluar(spline_col_r,5))/2));
					if( fabs(orig(j-1,i,0,0)-orig(j+1,i,0,0)) > fabs(orig(j,i-1,0,0)-orig(j,i+1,0,0)) )
						orig(j,i,0,0) = fmin(255.0,fmax(0,evaluar(spline_col_r,5)));
				}
			}
		}
	}

	orig.crop(5,5,0,0,ancho-5,alto-5,0,2); //no se si dejarlo aca o meterlo en el main
}