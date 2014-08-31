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
	Parabrisas(const double& a, const double& b, const double& h, const double& r, const double& t, const std::vector<Posicion>& v, 
	char const *tipoMatriz); //sistema de ecuaciones
	void CalcularTemperaturas(char const *salida, char const *tipoMatriz); //resuelve el sistema
	bool EsEstable(); //punto critico < 235
	double TempPtoCritico();
	void MatarSanguijuelas(); //VER SI DEVOLVEMOS LAS MUERTAS
	void MatarSanguijuelasDEA1();
	void MatarSanguijuelasDEA3();
	void MatarSanguijuelasProximas();

	//void VerMatriz(); //BORRAR

private:
	Matriz sist;
	double ancho;
	double b_altura;
	double radio;
	double h_disc;
	double temp;
	std::vector<Posicion> sanguijuelas; //aca van a estar todas, incluso las borradas. Es decir van a estar las que entraron como parametros
	std::vector<Posicion> sanguijuelasDiscretizadas; //aca van a estar las actuales, pero solo las que esten discretizadas
	char const *tipoMatriz;

	void Ataque(Posicion p, char const *tipoMatriz);
};

Parabrisas::Parabrisas(const double& a, const double& b, const double& h, const double& r, const double& t, const std::vector<Posicion>& v, 
char const *tipo) : ancho(a), b_altura(b), radio(r), h_disc(h), temp(t), sanguijuelas(v), sanguijuelasDiscretizadas(){
	char const *tip = tipo;
	tipoMatriz = tip;
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
			Ataque(*i, tipo);
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
	else if (*tipo == '1')
	{
		Nat mod = a/h;
		sist.Redimensionar(((a/h)+1)*((b/h)+1), (2*(mod+1))+2); //

		for (int i = 0; i < ((a/h)+1)*((b/h)+1); ++i)
		{
			if (i<(mod+1) || i>=((mod+1)*((b/h)+1) - (mod+1)) || i%((mod)+1)==0 || i%((mod)+1)==mod)
			{
				sist.Elem(i,mod+1) = 1.0;
				sist.Elem(i,2*(mod)+3) = -100.0;
			}
		}

		for (std::vector<Posicion>::iterator i = sanguijuelas.begin(); i != sanguijuelas.end(); ++i)
		{
			Ataque(*i, tipo);
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

/*void Parabrisas::VerMatriz()
{
	sist.Ver();
	std::cout << std::endl << "Filas: " << sist.Filas() << std::endl;
	std::cout << "Columnas: " << sist.Columnas() << std::endl;
}*/

void Parabrisas::CalcularTemperaturas(char const *salida, char const *tipo)
{
	const Nat k = ((ancho/h_disc));
	sist.ResolverSistema(tipo, k);

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

void Parabrisas::Ataque(Posicion p, char const *tipo)
{
	p.x = (p.x)/h_disc;
	p.y = (p.y)/h_disc;

	double radio_h = radio/h_disc;
	double radioe = pow(radio_h,2);

	Intervalo intervalo_x;
	Intervalo intervalo_y;

	intervalo_x.izq = (p.x)-radio_h;
	intervalo_x.der = (p.x)+radio_h;
	intervalo_y.izq = (p.y)-radio_h;
	intervalo_y.der = (p.y)+radio_h;

	int i = ceil(intervalo_x.izq);
	int j = ceil(intervalo_y.izq);
	bool esDiscret = false;

	if (*tipo == '0')
	{
		while(i <= floor(intervalo_x.der))
		{
			int c = j;
			while(c <= floor(intervalo_y.der))
			{
				double norma2 = (pow(i-(p.x),2) + pow(c-(p.y),2));
				if (norma2 <= radioe)
				{ // +error ¿?
					sist.Elem(c*((ancho/h_disc)+1)+i, ((ancho/h_disc)+1)*((b_altura/h_disc)+1)) = temp;
					sist.Elem(c*((ancho/h_disc)+1)+i, c*((ancho/h_disc)+1)+i) = 1;
					if(!esDiscret)
						esDiscret = true;
				}
				c++;
			}
			i++;
			c = j;
		}
	}else if (*tipo == '1')
	{
		while(i <= floor(intervalo_x.der))
		{
			int c = j;
			while(c <= floor(intervalo_y.der))
			{
				double norma2 = (pow(i-(p.x),2) + pow(c-(p.y),2));
				if (norma2 <= radioe)
				{
					sist.Elem(c*((ancho/h_disc)+1)+i, 2*(ancho/h_disc)+3) = temp;
					sist.Elem(c*((ancho/h_disc)+1)+i, (ancho/h_disc)+1) = 1;
					if(!esDiscret)
						esDiscret = true;
				}
			c++;
			}
			i++;
			c = j;
		}
	}
	if(esDiscret)
		sanguijuelasDiscretizadas.push_back(p); //Agrega al final
}

bool Parabrisas::EsEstable(){
	if((*this).TempPtoCritico() < 235)
		return true;
	else
		return false;
}

double Parabrisas::TempPtoCritico(){
	double x = ancho/(2*h_disc);
	double y = b_altura/(2*h_disc);
	if(floor(x)==x && floor(y)==y)
	{
		return sist.Elem(2*x*y+x,sist.Columnas()-1);
	}
	else
	{
		int colDer = ceil(x);
		int colIzq = floor(x);
		int filArri = floor(y);
		int filAba = ceil(y);

		if(fabs(colIzq+(h_disc/2)-x)<0.00001)
		{
			if(fabs(colDer+(h_disc/2)-x)<0.00001)
				return ((sist.Elem(filArri,colDer)+sist.Elem(filAba,colDer)+sist.Elem(filArri,colIzq)+sist.Elem(filAba,colIzq))/4);
			else if((filArri+(h_disc/2))<y) //PENSAR SI PONER TOLERANCIA
				return ((sist.Elem(filAba,colDer)+sist.Elem(filAba,colIzq))/2);
			else
				return ((sist.Elem(filArri,colDer)+sist.Elem(filArri,colIzq))/2);
		}
		else if(fabs(filArri+(h_disc/2)-y)<0.00001)
		{
			if((colIzq+(h_disc/2))<x) //PENSAR SI PONER TOLERANCIA
				return ((sist.Elem(filAba,colDer)+sist.Elem(filArri,colDer))/2);
			else
				return ((sist.Elem(filAba,colIzq)+sist.Elem(filArri,colIzq))/2);
		}
		else
		{
			if(x<colIzq+(h_disc/2))
			{
				if(y<filArri+(h_disc/2))
					return sist.Elem(filArri,colIzq);
				else
					return sist.Elem(filAba,colIzq);
			}
			else
			{
				if(y<filArri+(h_disc/2))
					return sist.Elem(filArri,colDer);
				else
					return sist.Elem(filAba,colDer);
			}
		}
	}
}

void Parabrisas::MatarSanguijuelasDEA1(){
	while(!(this->EsEstable())){
		//Se hace la cuenta con los puntos reales
		double norma1 = pow(sanguijuelasDiscretizadas[0].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[0].y-(b_altura/2),2) ; //No hace falta hacer la raiz cuadrada, porque no lo hago en ninguno de los dos
		for (int i = 0; i < sanguijuelasDiscretizadas.size()-1; ++i)
		{//POR INVARIANTE el norma1 siempre va a tener la norma del elemento i
			double norma2 = pow(sanguijuelasDiscretizadas[i+1].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[i+1].y-(b_altura/2),2) ;
			if(norma1 < norma2){
				//VER QUE ONDA
				//sanguijuelasDiscretizadas[i].swap(sanguijuelasDiscretizadas[i+1]);
				Posicion posI = sanguijuelasDiscretizadas[i];
				sanguijuelasDiscretizadas[i] = sanguijuelasDiscretizadas[i+1];
				sanguijuelasDiscretizadas[i+1] = posI;
			}else{
					norma1 = norma2;
			}
		}
		sanguijuelasDiscretizadas.pop_back();
		char const *t = tipoMatriz;
		Parabrisas p = Parabrisas(ancho, b_altura, h_disc, radio, temp, sanguijuelasDiscretizadas, t);
		sist = p.sist;
	}
}

void Parabrisas::MatarSanguijuelasDEA3(){
	while(!(this->EsEstable())){
		for (int j = 0; j < 3; ++j)
		{
			double norma1 = pow(sanguijuelasDiscretizadas[0].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[0].y-(b_altura/2),2) ; //No hace falta hacer la raiz cuadrada, porque no lo hago en ninguno de los dos
			for (int i = 0; i < sanguijuelasDiscretizadas.size()-1; ++i)
			{
				double norma2 = pow(sanguijuelasDiscretizadas[i+1].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[i+1].y-(b_altura/2),2) ;
				if(norma1 < norma2){
					Posicion posI = sanguijuelasDiscretizadas[i];
					sanguijuelasDiscretizadas[i] = sanguijuelasDiscretizadas[i+1];
					sanguijuelasDiscretizadas[i+1] = posI;
				}else{
						norma1 = norma2;
				}
			}
			sanguijuelasDiscretizadas.pop_back();
		}
		char const *t = tipoMatriz;
		Parabrisas p = Parabrisas(ancho, b_altura, h_disc, radio, temp, sanguijuelasDiscretizadas, t);
		sist = p.sist;
	}

}

void Parabrisas::MatarSanguijuelasProximas(){ //borra la mas cerca y las 3 mas proximas a esa
	while(!(this->EsEstable())){
		double norma1 = pow(sanguijuelasDiscretizadas[0].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[0].y-(b_altura/2),2) ; //No hace falta hacer la raiz cuadrada, porque no lo hago en ninguno de los dos
		for (int i = 0; i < sanguijuelasDiscretizadas.size()-1; ++i)
		{
			double norma2 = pow(sanguijuelasDiscretizadas[i+1].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[i+1].y-(b_altura/2),2) ;
			if(norma1 < norma2){
				Posicion posI = sanguijuelasDiscretizadas[i];
				sanguijuelasDiscretizadas[i] = sanguijuelasDiscretizadas[i+1];
				sanguijuelasDiscretizadas[i+1] = posI;
			}else{
					norma1 = norma2;
			}
		}
		Posicion aBorrar = sanguijuelasDiscretizadas[sanguijuelasDiscretizadas.size()-1];
		sanguijuelasDiscretizadas.pop_back();
		for (int j = 0; j < 3; ++j)
		{
			double dif = pow(sanguijuelasDiscretizadas[0].x - aBorrar.x,2)+pow(sanguijuelasDiscretizadas[0].y - aBorrar.y,2) ;
			for (int w = 0; w < sanguijuelasDiscretizadas.size()-1-j; ++w)
			{
				double dif2 = pow(sanguijuelasDiscretizadas[w+1].x - aBorrar.x,2)+pow(sanguijuelasDiscretizadas[w+1].y - aBorrar.y,2);
				if(dif < dif2){
					Posicion posI = sanguijuelasDiscretizadas[w];
					sanguijuelasDiscretizadas[w] = sanguijuelasDiscretizadas[w+1];
					sanguijuelasDiscretizadas[w+1] = posI;
				}else{
					dif = dif2;
				}
			}
		}
		sanguijuelasDiscretizadas.pop_back();
		sanguijuelasDiscretizadas.pop_back();
		sanguijuelasDiscretizadas.pop_back();
		char const *t = tipoMatriz;
		Parabrisas p = Parabrisas(ancho, b_altura, h_disc, radio, temp, sanguijuelasDiscretizadas, t);
		sist = p.sist;
	}
}

#endif
