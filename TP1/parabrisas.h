#ifndef PARABRISAS_H
#define PARABRISAS_H

#include "matriz.h"
#include <fstream>

struct Posicion
{
	double x;
	double y;
};

struct Intervalo
{
	double izq;
	double der;
};

class Parabrisas
{
public:
	Parabrisas(const double& a, const double& b, const double& h, const double& r, const double& t, const std::vector<Posicion>& v, char const *tipoMatriz); //sistema de ecuaciones
	void CalcularTemperaturas(char const *salida, char const *tipoMatriz); //resuelve el sistema
	bool EsEstable(); //punto critico < 235
	void MatarSanguijuelas(); //VER SI DEVOLVEMOS LAS MUERTAS
	void VerMatriz(); //BORRAR

private:
	Matriz sist;
	double ancho;
	double b_altura;
	double radio;
	double h_disc;
	double temp;
	std::vector<Posicion> sanguijuelas;

	void AtaqueB(Posicion p);
	void AtaqueC(Posicion p);
};

Parabrisas::Parabrisas(const double& a, const double& b, const double& h, const double& r, const double& t, const std::vector<Posicion>& v, char const *tipo) : ancho(a), b_altura(b), radio(r), h_disc(h), temp(t), sanguijuelas(v)
{
	if (*tipo == '0')
	{
		sist.Redimensionar(((a/h)+1)*((b/h)+1), ((a/h)+1)*((b/h)+1)+1);

		Nat mod = a/h;
		for (int i = 0; i < ((a/h)+1)*((b/h)+1); ++i)
		{
			if (i<(mod+1) || i>=(((a/h)+1)*((b/h)+1) - (mod+1)) || i%((mod)+1)==0 || i%((mod)+1)==mod)
			{
				sist.Elem(i,i) = 1.0;
				sist.Elem(i,((a/h)+1)*((b/h)+1)) = -100.0;
			}
		}

		for (std::vector<Posicion>::iterator i = sanguijuelas.begin(); i != sanguijuelas.end(); ++i)
		{
			AtaqueC(*i);
		}

		for (int i = 0; i < ((a/h)+1)*((b/h)+1); ++i)
		{
			if (sist.Elem(i,i) == 0)
			{
				sist.Elem(i,i) = -4.0;
				sist.Elem(i,i-1) = 1.0;
				sist.Elem(i,i+1) = 1.0;
				sist.Elem(i,i-(mod+1)) = 1.0;
				sist.Elem(i,i+(mod+1)) = 1.0;
			}
		}
	}else if (*tipo == '1')
	{
		//Le agregue los ceros de la izquierda
		Nat mod = a/h;
		sist.Redimensionar(((a/h)+1)*((b/h)+1), 2*(mod)+4);

		for (int i = 0; i < ((a/h)+1)*((b/h)+1); ++i)
		{
			if (i<(mod+1) || i>=(((mod)+1)*((b/h)+1) - (mod+1)) || i%((mod)+1)==0 || i%((mod)+1)==mod)
			{
				sist.Elem(i,mod+1) = 1.0;
				sist.Elem(i,2*(mod)+3) = -100.0;
			}
		}

		for (std::vector<Posicion>::iterator i = sanguijuelas.begin(); i != sanguijuelas.end(); ++i)
		{
			AtaqueB(*i);
		}

		for (int i = 0; i < (mod+1)*((b/h)+1); ++i)
		{
			if (sist.Elem(i,mod+1) == 0)
			{
				sist.Elem(i,mod+1) = -4.0;
				sist.Elem(i,0) = 1.0;
				sist.Elem(i,mod) = 1.0;
				sist.Elem(i,mod+2) = 1.0;
				sist.Elem(i,2*(mod)+2) = 1.0;
			}
		}
	}
};

void Parabrisas::VerMatriz(){

	std::cout<< "A: " << ancho << "|| B: " << b_altura << "| H: " << h_disc << std::endl;
	this->sist.Ver();
}

void Parabrisas::CalcularTemperaturas(char const *salida, char const *tipo)
{
	sist.ResolverSistema(tipo);

	int f = 0;
	int c = 0;

	std::ofstream ofs(salida);

	ofs.setf( std::ios::fixed, std::ios::floatfield );
	ofs.precision(5);

	for (int i = 0; i < sist.Filas(); ++i)
	{
		ofs << f << "\t" << c << "\t" << sist.Elem(i, (sist.Columnas()-1)) << std::endl;

		if ( c == ancho/h_disc )
		{
			c=0;
			f++;
		}
		else
			c++;
	}
}

void Parabrisas::AtaqueC(Posicion p)
{
	p.x = (p.x)/h_disc;
	p.y = (p.y)/h_disc;

	double radio_h = radio/h_disc;

	Intervalo intervalo_x;
	Intervalo intervalo_y;

	intervalo_x.izq = (p.x)-radio_h;
	intervalo_x.der = (p.x)+radio_h;
	intervalo_y.izq = (p.y)-radio_h;
	intervalo_y.der = (p.y)+radio_h;

	int i = ceil(intervalo_x.izq);
	int j = ceil(intervalo_y.izq);

	while(i <= floor(intervalo_x.der))
	{

		int c = j;
		while(c <= floor(intervalo_y.der))
		{
			double norma2 = (pow(i-(p.x),2) + pow(c-(p.y),2));

			if (norma2 <= pow(radio_h,2))
			{
				sist.Elem(c*((ancho/h_disc)+1)+i, ((ancho/h_disc)+1)*((b_altura/h_disc)+1)) = temp;
				sist.Elem(c*((ancho/h_disc)+1)+i, c*((ancho/h_disc)+1)+i) = 1;
			}

			c++;
		}

		i++;
		c = j;
	}

}

void Parabrisas::AtaqueB(Posicion p){
	p.x = (p.x)/h_disc;
	p.y = (p.y)/h_disc;

	double radio_h = radio/h_disc;

	Intervalo intervalo_x;
	Intervalo intervalo_y;

	intervalo_x.izq = (p.x)-radio_h;
	intervalo_x.der = (p.x)+radio_h;
	intervalo_y.izq = (p.y)-radio_h;
	intervalo_y.der = (p.y)+radio_h;

	int i = ceil(intervalo_x.izq);
	int j = ceil(intervalo_y.izq);

	while(i <= floor(intervalo_x.der))
		{
			int c = j;
			while(c <= floor(intervalo_y.der))
			{
				double norma2 = (pow(i-(p.x),2) + pow(c-(p.y),2));

				if (norma2 <= pow(radio_h,2))
				{
					sist.Elem(c*((ancho/h_disc)+1)+i, 2*(ancho/h_disc)+3) = temp;
					sist.Elem(c*((ancho/h_disc)+1)+i, (ancho/h_disc)+1) = 1;
				}
			c++;
			}
			i++;
			c = j;
		}
}

#endif
