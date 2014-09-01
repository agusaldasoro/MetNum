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
	void MatarSanguijuelasDEA1(char const *salidaSanguis);
	void MatarSanguijuelasPorcentaje(char const *salidaSanguis);
	void MatarSanguijuelasProximas(char const *salidaSanguis);

	void VerMatriz(); //BORRAR
	//void VerSanguis(); //BORRAR

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

Parabrisas::Parabrisas(const double& a, const double& b, const double& h, const double& r, const double& t, const std::vector<Posicion>& v, char const *tipo) : ancho(a), b_altura(b), radio(r), h_disc(h), temp(t), sanguijuelas(v), tipoMatriz(tipo) {

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

void Parabrisas::VerMatriz()
{
	sist.Ver();
	std::cout << std::endl << "Filas: " << sist.Filas() << std::endl;
	std::cout << "Columnas: " << sist.Columnas() << std::endl;
}

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
	if ((p.x)>radio && (p.y)>radio && (ancho-(p.x))>radio && (b_altura-(p.y))>radio)
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
		{
			p.x = (p.x)*h_disc;
		 	p.y = (p.y)*h_disc;
		 	sanguijuelasDiscretizadas.push_back(p);
		} //Agrega al final
	}
}

bool Parabrisas::EsEstable()
{
	if (TempPtoCritico() < 235)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double Parabrisas::TempPtoCritico()
{
	double x = ancho/(2*h_disc);
	double y = b_altura/(2*h_disc);
	double k = ancho/h_disc;

	if(ceil(x)==x && ceil(y)==y)
		return sist.Elem((k+1)*y+x, sist.Columnas()-1);
	else
	{
		int colIzq = floor(x);
		int colDer;
		if (floor(x)==x)
			colDer = colIzq-1;
		else
			colDer = colIzq+1;
		int filAba = floor(y);
		int filArri;
		if (floor(y)==y)
			filArri = filAba-1;
		else
			filArri = filAba+1;

		//Si estoy parado en el punto (f,c), entonces voy a la columna k*f+c

		if(fabs(colIzq+(0.5)-x)<0.00001) //si esta en el "centro horizontal" del cuadrado
		{
			if(fabs(filArri+(0.5)-y)<0.00001) //y en la mitad verticalmente -> es el promedio entre los 4
				return ((sist.Elem((k+1)*filArri+colDer,sist.Columnas()-1)+sist.Elem((k+1)*filAba+colDer,sist.Columnas()-1)+sist.Elem((k+1)*filArri+colIzq,sist.Columnas()-1)+sist.Elem((k+1)*filAba+colIzq,sist.Columnas()-1))/4);
			else if((filArri+(0.5))<y) //PENSAR SI PONER TOLERANCIA
				return ((sist.Elem((k+1)*filAba+colDer,sist.Columnas()-1)+sist.Elem((k+1)*filAba+colIzq,sist.Columnas()-1))/2);
			else
				return ((sist.Elem((k+1)*filArri+colDer,sist.Columnas()-1)+sist.Elem((k+1)*filArri+colIzq,sist.Columnas()-1))/2);
		}
		else if(fabs(filArri+(0.5)-y)<0.00001) //si esta en el "centro vertical" del cuadrado
		{
			if((colIzq+(0.5))<x) //PENSAR SI PONER TOLERANCIA
				return ((sist.Elem((k+1)*filAba+colDer,sist.Columnas()-1)+sist.Elem((k+1)*filArri+colDer,sist.Columnas()-1))/2);
			else
				return ((sist.Elem((k+1)*filAba+colIzq,sist.Columnas()-1)+sist.Elem((k+1)*filArri+colIzq,sist.Columnas()-1))/2);
		}
		else
		{
			if(x<colIzq+(0.5)) //si esta en la mitad izquierda del cuadrado
			{
				if(y<filArri+(0.5)) //y en la parte superior -> segundo cuadrante
					return sist.Elem((k+1)*filArri+colIzq,sist.Columnas()-1);
				else //y en la parte inferior -> tercer cuadrante
					return sist.Elem((k+1)*filAba+colIzq,sist.Columnas()-1);
			}
			else //mitad derecha del cuadrado
			{
				if(y<filArri+(0.5)) // parte superior -> primer cuadrante
					return sist.Elem((k+1)*filArri+colDer,sist.Columnas()-1);
				else // parte inferior -> cuarto cuadrante
					return sist.Elem((k+1)*filAba+colDer,sist.Columnas()-1);
			}
		}
	}
}

void Parabrisas::MatarSanguijuelasDEA1(char const *salidaSanguis){
	std::cout << "Antes: " << sanguijuelasDiscretizadas.size() << std::endl;
	/*for (int e = 0; e < sanguijuelasDiscretizadas.size(); ++e)
		{
			double x = sanguijuelasDiscretizadas[e].x;
			double y = sanguijuelasDiscretizadas[e].y;
			std::cout << "(" << x << "," << y << ")" << std::endl;
		}*/
	while(!(this->EsEstable())){
		//Se hace la cuenta con los puntos reales
		double norma1 = pow(sanguijuelasDiscretizadas[0].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[0].y-(b_altura/2),2) ; //No hace falta hacer la raiz cuadrada, porque no lo hago en ninguno de los dos
		for (int i = 0; i < sanguijuelasDiscretizadas.size()-1; ++i)
		{//POR INVARIANTE el norma1 siempre va a tener la norma del elemento i
			double norma2 = pow(sanguijuelasDiscretizadas[i+1].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[i+1].y-(b_altura/2),2);
			if(norma1 < norma2){
				//VER QUE ONDA
				//(sanguijuelasDiscretizadas[i]).swap(sanguijuelasDiscretizadas[i+1]);
				Posicion posI = sanguijuelasDiscretizadas[i];
				sanguijuelasDiscretizadas[i] = sanguijuelasDiscretizadas[i+1];
				sanguijuelasDiscretizadas[i+1] = posI;
			}else{
					norma1 = norma2;
			}
		}
		sanguijuelasDiscretizadas.pop_back();
		Parabrisas p = Parabrisas(ancho, b_altura, h_disc, radio, temp, sanguijuelasDiscretizadas, tipoMatriz);
		sist = p.sist;

		// std::cout << "OTRA VEZ: " << std::endl;
		// p.VerMatriz();
		CalcularTemperaturas(salidaSanguis, tipoMatriz);
		// std::cout << "OTRA VEZ: " << std::endl;
		// VerMatriz();
	}
	std::cout << "Despues: " << sanguijuelasDiscretizadas.size() << std::endl;
	/*for (int e = 0; e < sanguijuelasDiscretizadas.size(); ++e)
		{
			double x = sanguijuelasDiscretizadas[e].x;
			double y = sanguijuelasDiscretizadas[e].y;
			std::cout << "(" << x << "," << y << ")" << std::endl;
		}*/
}

void Parabrisas::MatarSanguijuelasPorcentaje(char const *salidaSanguis){ //CAMBIAR, ESTO NO PUEDE SER 3 FIJO, SI CAPAZ NI HAY 3 SANGUIJUELAS
	std::cout << "Antes: " << sanguijuelasDiscretizadas.size() << std::endl;
	/*for (int e = 0; e < sanguijuelasDiscretizadas.size(); ++e)
		{
			double x = sanguijuelasDiscretizadas[e].x;
			double y = sanguijuelasDiscretizadas[e].y;
			std::cout << "(" << x << "," << y << ")" << std::endl;
		}*/

	while(!(this->EsEstable())){
		float z = sanguijuelasDiscretizadas.size();
		z *= 0.25;
		z = ceil(z);
		//borra el 25% de sanguijuelas
		for (int j = 0; j < z; ++j)
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
		Parabrisas p = Parabrisas(ancho, b_altura, h_disc, radio, temp, sanguijuelasDiscretizadas, tipoMatriz);
		sist = p.sist;
		CalcularTemperaturas(salidaSanguis, tipoMatriz);
	}
	std::cout << "Despues: " << sanguijuelasDiscretizadas.size() << std::endl;
	for (int e = 0; e < sanguijuelasDiscretizadas.size(); ++e)
		{
			double x = sanguijuelasDiscretizadas[e].x;
			double y = sanguijuelasDiscretizadas[e].y;
			std::cout << "(" << x << "," << y << ")" << std::endl;
		}
}

void Parabrisas::MatarSanguijuelasProximas(char const *salidaSanguis)//borra la mas cerca y las 3 mas proximas a esa --> MISMO PROBLEMA QUE ARRIBA
{
	std::cout << "Antes: " << sanguijuelasDiscretizadas.size() << std::endl;
	/*for (int e = 0; e < sanguijuelasDiscretizadas.size(); ++e)
		{
			double x = sanguijuelasDiscretizadas[e].x;
			double y = sanguijuelasDiscretizadas[e].y;
			std::cout << "(" << x << "," << y << ")" << std::endl;
		}*/

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
		int j = 0;
		while(j < 3 && j < sanguijuelasDiscretizadas.size()-1)
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
			j++;
		}
		for (int p = 0; p < j+1; ++p)
		{
			sanguijuelasDiscretizadas.pop_back();
		}

		Parabrisas p = Parabrisas(ancho, b_altura, h_disc, radio, temp, sanguijuelasDiscretizadas, tipoMatriz);
		sist = p.sist;
		CalcularTemperaturas(salidaSanguis, tipoMatriz);
	}
	std::cout << "Despues: " << sanguijuelasDiscretizadas.size() << std::endl;
	for (int e = 0; e < sanguijuelasDiscretizadas.size(); ++e)
		{
			double x = sanguijuelasDiscretizadas[e].x;
			double y = sanguijuelasDiscretizadas[e].y;
			std::cout << "(" << x << "," << y << ")" << std::endl;
		}
}

#endif
