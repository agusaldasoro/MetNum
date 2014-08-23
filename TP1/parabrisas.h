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
	//void VerMatriz(); //BORRAR

private:
	Matriz sist;
	double ancho;
	double b_altura;
	double radio;
	double h_disc;
	double temp;
	std::vector<Posicion> sanguijuelas;

	void Ataque(Posicion p);
};

Parabrisas::Parabrisas(const double& a, const double& b, const double& h, const double& r, const double& t, const std::vector<Posicion>& v, char const *tipo) : ancho(a), b_altura(b), radio(r), h_disc(h), temp(t), sanguijuelas(v)
{
	//ACA HAY QUE PONER LOS DOS CASOS PARA ARMAR LA MATRIZ: SI BANDA O NO.

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
			Ataque(*i);
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
	}
};

/*void Parabrisas::VerMatriz()
{
	sist.Ver();
	std::cout << std::endl << "Filas: " << sist.Filas() << std::endl;
	std::cout << "Columnas: " << sist.Columnas() << std::endl;
}*/

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

void Parabrisas::Ataque(Posicion p)
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

#endif