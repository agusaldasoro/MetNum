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
	double TempPtoCritico();
	void MatarSanguijuelasDEA1(char const *salidaSanguis);
	void MatarSanguijuelasPorcentaje(char const *salidaSanguis);
	void MatarSanguijuelasProximas(char const *salidaSanguis);

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

	if (*tipo == '0') //Crear un nuevo parabrisas con la estructura convencional de matrices
	{
		sist.Redimensionar(((a/h)+1)*((b/h)+1), ((a/h)+1)*((b/h)+1)+1); //creo la matriz. Se inicializa con ceros, y como una matriz ampliada donde la ultima columna representa al vector independiente.

		Nat mod = a/h; //mod = ancho "discretizado"
		for (int i = 0; i < ((a/h)+1)*((b/h)+1); ++i) //para cada fila...
		{
			if (i<(mod+1) || i>=(((a/h)+1)*((b/h)+1) - (mod+1)) || i%((mod)+1)==0 || i%((mod)+1)==mod) //si está en una posición que representa al borde del parabrisas
			{
				sist.Elem(i,i) = 1.0; //pongo un 1 en la diagonal
				sist.Elem(i,((a/h)+1)*((b/h)+1)) = -100.0; //y establezco como -100 su temperatura
			}
		}

		for (std::vector<Posicion>::iterator i = sanguijuelas.begin(); i != sanguijuelas.end(); ++i) //para cada posición en la cual hay sanguijuelas
		{
			Ataque(*i, tipo); //ubico las sanguijuelas
		}

		for (int i = 0; i < ((a/h)+1)*((b/h)+1); ++i) //nuevamente, para cada fila
		{
			if (sist.Elem(i,i) == 0) //si la fila no representa a una posición borde ni tiene una sanguijuela (es decir, el elemento de la diagonal sigue en cero)...
			{
				sist.Elem(i,i) = -4.0;
				sist.Elem(i,i-1) = 1.0;
				sist.Elem(i,i+1) = 1.0;
				sist.Elem(i,i-(mod+1)) = 1.0;
				sist.Elem(i,i+(mod+1)) = 1.0; //armo el sistema que corresponde (detalles en el informe)
			}
		}
	}
	else if (*tipo == '1') //Crear un nuevo parabrisas con la estructura banda para matrices
	{
		Nat mod = a/h; //mod = ancho "discretizado"
		sist.Redimensionar(((a/h)+1)*((b/h)+1), (2*(mod+1))+2); //creo la matriz. Se inicializa con ceros, y como una matriz ampliada donde la ultima columna representa al vector independiente.

		for (int i = 0; i < ((a/h)+1)*((b/h)+1); ++i) //para cada fila...
		{
			if (i<(mod+1) || i>=((mod+1)*((b/h)+1) - (mod+1)) || i%((mod)+1)==0 || i%((mod)+1)==mod) //si está en una posición que representa al borde del parabrisas
			{
				sist.Elem(i,mod+1) = 1.0; //pongo un 1 en la diagonal
				sist.Elem(i,2*(mod)+3) = -100.0; //y establezco como -100 su temperatura
			}
		}

		for (std::vector<Posicion>::iterator i = sanguijuelas.begin(); i != sanguijuelas.end(); ++i) //para cada posición en la cual hay sanguijuelas
		{
			Ataque(*i, tipo); //ubico las sanguijuelas
		}

		for (int i = 0; i < (mod+1)*((b/h)+1); ++i) //nuevamente, para cada fila
		{
			if (sist.Elem(i,mod+1) == 0) //si la fila no representa a una posición borde ni tiene una sanguijuela (es decir, el elemento de la diagonal sigue en cero)...
			{
				sist.Elem(i,mod+1) = -4.0;
				sist.Elem(i,0) = 1.0;
				sist.Elem(i,mod) = 1.0;
				sist.Elem(i,mod+2) = 1.0;
				sist.Elem(i,2*(mod)+2) = 1.0; //armo el sistema que corresponde (detalles en el informe)
			}
		}
	}
};

void Parabrisas::CalcularTemperaturas(char const *salida, char const *tipo)
{
	const Nat k = ((ancho/h_disc)); //cantidad de columnas del parabrisas discretizado
	sist.ResolverSistema(tipo, k); //resuelvo el sistema

	int f = 0;
	int c = 0;

	std::ofstream ofs(salida);

	ofs.setf( std::ios::fixed, std::ios::floatfield );
	ofs.precision(5);

	for (int i = 0; i < sist.Filas(); ++i) //escribo al archivo de salida
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
	if ((p.x)>radio && (p.y)>radio && (ancho-(p.x))>radio && (b_altura-(p.y))>radio) //si la sanguijuela no esta tocando el borde del parabrisas:
	{
		double posx = (p.x)/h_disc;
		double posy = (p.y)/h_disc; //guardo su posicion discretizada

		double radio_h = radio/h_disc;
		double radioe = pow(radio_h,2);

		Intervalo intervalo_x;
		Intervalo intervalo_y;

		intervalo_x.izq = posx-radio_h;
		intervalo_x.der = posx+radio_h;
		intervalo_y.izq = posy-radio_h;
		intervalo_y.der = posy+radio_h; //y armo "el cuadrado" dentro del cual puede caer la sanguijuela discretizada

		int i = ceil(intervalo_x.izq);
		int j = ceil(intervalo_y.izq);
		bool esDiscret = false;

		if (*tipo == '0') //para la estructura convencional de matriz:
		{
			while(i <= floor(intervalo_x.der)) //voy recorriendo de izquierda a derecha el cuadrado
			{
				int c = j;
				while(c <= floor(intervalo_y.der)) //y para cada punto del ancho del cuadrado, recorro todos los puntos que estan en la columna que cae dentro del mismo
				{//en definitiva, lo que hago es recorrer cada punto del cuadrado
					double norma2 = (pow(i-posx,2) + pow(c-posy,2)); //calculo la distancia entre el punto discretizado que estoy viendo y el centro de la sanguijuela
					if (norma2 <= (radioe+0.00000000001)) //si el punto es tocado por la sanguijuela (+ una tolerancia)
					{
						sist.Elem(c*((ancho/h_disc)+1)+i, ((ancho/h_disc)+1)*((b_altura/h_disc)+1)) = temp; //entonces seteo su temperatura = a la temp. de la sanguijuela
						sist.Elem(c*((ancho/h_disc)+1)+i, c*((ancho/h_disc)+1)+i) = 1; //y pongo un 1 en su diagonal
						if(!esDiscret)
							esDiscret = true; //ademas, me "guardo" que esta sanguijuela tiene al menos un punto discretizado
					}
					c++;
				}
				i++;
				c = j;
			}
		}else if (*tipo == '1') //lo mismo, pero para la estructura banda, cambiando los indices pertinentemente.
		{
			while(i <= floor(intervalo_x.der))
			{
				int c = j;
				while(c <= floor(intervalo_y.der))
				{
					double norma2 = (pow(i-posx,2) + pow(c-posy,2));
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
		 	sanguijuelasDiscretizadas.push_back(p); //agrego la posición de la sanguijuela al vector de las sanguijuelas discretizadas
	}
}

bool Parabrisas::EsEstable()
{
	if (TempPtoCritico() < (235 + 0.000001))
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

	if(ceil(x)==x && ceil(y)==y) //si el punto critico esta discretizado
		return sist.Elem((k+1)*y+x, sist.Columnas()-1); //devolver su valor
	else
	{//sino, me armo un cuadrado con las filas/columnas que encierran a ese punto
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
			else if((filArri+(0.5))<y)
				return ((sist.Elem((k+1)*filAba+colDer,sist.Columnas()-1)+sist.Elem((k+1)*filAba+colIzq,sist.Columnas()-1))/2);
			else
				return ((sist.Elem((k+1)*filArri+colDer,sist.Columnas()-1)+sist.Elem((k+1)*filArri+colIzq,sist.Columnas()-1))/2);
		}
		else if(fabs(filArri+(0.5)-y)<0.00001) //si esta en el "centro vertical" del cuadrado
		{
			if((colIzq+(0.5))<x)
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
	int cant = sanguijuelasDiscretizadas.size();

	while(!(this->EsEstable())){
		//busco la sanguijuela con la minima norma y la ubico al final del vector...
		double norma1 = pow(sanguijuelasDiscretizadas[0].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[0].y-(b_altura/2),2) ; //No hace falta hacer la raiz cuadrada, porque no lo hago en ninguno de los dos
		for (int i = 0; i < sanguijuelasDiscretizadas.size()-1; ++i)
		{//POR INVARIANTE el norma1 siempre va a tener la norma del elemento i
			double norma2 = pow(sanguijuelasDiscretizadas[i+1].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[i+1].y-(b_altura/2),2);
			if(norma1 < (norma2+0.00000000001)){
				Posicion posI = sanguijuelasDiscretizadas[i];
				sanguijuelasDiscretizadas[i] = sanguijuelasDiscretizadas[i+1];
				sanguijuelasDiscretizadas[i+1] = posI;
			}else{
					norma1 = norma2;
			}
		}
		sanguijuelasDiscretizadas.pop_back(); //una vez que esta al final, la borro
		Parabrisas p = Parabrisas(ancho, b_altura, h_disc, radio, temp, sanguijuelasDiscretizadas, tipoMatriz); //y recalculo el parabrisas
		sist = p.sist;
		CalcularTemperaturas(salidaSanguis, tipoMatriz);
	}

	std::cout << "Se mataron: " << cant-sanguijuelasDiscretizadas.size() << " sanguijuela(s). El parabrisas esta estable." << std::endl;
}

void Parabrisas::MatarSanguijuelasPorcentaje(char const *salidaSanguis) //mata el 25% de las sanguijuelas mas cercanas al PC
{
	int cant = sanguijuelasDiscretizadas.size();

	while(!(this->EsEstable())){
		float z = sanguijuelasDiscretizadas.size();
		z *= 0.25;
		z = ceil(z);
		//borra el 25% de las sanguijuelas, en base a que tan cerca estan del punto critico
		for (int j = 0; j < z; ++j)
		{
			double norma1 = pow(sanguijuelasDiscretizadas[0].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[0].y-(b_altura/2),2) ; //No hace falta hacer la raiz cuadrada, porque no lo hago en ninguno de los dos
			for (int i = 0; i < sanguijuelasDiscretizadas.size()-1; ++i)
			{
				double norma2 = pow(sanguijuelasDiscretizadas[i+1].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[i+1].y-(b_altura/2),2) ;
				if(norma1 < (norma2+0.00000000001)){
					Posicion posI = sanguijuelasDiscretizadas[i];
					sanguijuelasDiscretizadas[i] = sanguijuelasDiscretizadas[i+1];
					sanguijuelasDiscretizadas[i+1] = posI;
				}else{
						norma1 = norma2;
				}
			}
			sanguijuelasDiscretizadas.pop_back(); //las elimino
		}

		Parabrisas p = Parabrisas(ancho, b_altura, h_disc, radio, temp, sanguijuelasDiscretizadas, tipoMatriz); //y recalculo
		sist = p.sist;
		CalcularTemperaturas(salidaSanguis, tipoMatriz);

	}

	std::cout << "Se mataron: " << cant-sanguijuelasDiscretizadas.size() << " sanguijuela(s). El parabrisas esta estable." << std::endl;

}

void Parabrisas::MatarSanguijuelasProximas(char const *salidaSanguis) //mata la mas cercana al PC y el 25% mas cercano a esa
{
	int cant = sanguijuelasDiscretizadas.size();

	while(!(this->EsEstable())){
		float z = sanguijuelasDiscretizadas.size();
		z *= 0.25;
		z = ceil(z);
		//busco la sanguijuela mas cercana al PC
		double norma1 = pow(sanguijuelasDiscretizadas[0].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[0].y-(b_altura/2),2) ; //No hace falta hacer la raiz cuadrada, porque no lo hago en ninguno de los dos
		for (int i = 0; i < sanguijuelasDiscretizadas.size()-1; ++i)
		{
			double norma2 = pow(sanguijuelasDiscretizadas[i+1].x-(ancho/2),2)+pow(sanguijuelasDiscretizadas[i+1].y-(b_altura/2),2) ;
			if(norma1 < (norma2+0.00000000001)){
				Posicion posI = sanguijuelasDiscretizadas[i];
				sanguijuelasDiscretizadas[i] = sanguijuelasDiscretizadas[i+1];
				sanguijuelasDiscretizadas[i+1] = posI;
			}else{
					norma1 = norma2;
			}
		}
		Posicion aBorrar = sanguijuelasDiscretizadas[sanguijuelasDiscretizadas.size()-1];
		int j = 0;
		while(j < z-1) //y ahora busco el 25% de las restantes que esten mas cerca de la mas cercana
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

		for (int p = 0; p < j+1; ++p) //las elimino
		{
			sanguijuelasDiscretizadas.pop_back();
		}

		Parabrisas p = Parabrisas(ancho, b_altura, h_disc, radio, temp, sanguijuelasDiscretizadas, tipoMatriz); //y recalculo
		sist = p.sist;
		CalcularTemperaturas(salidaSanguis, tipoMatriz);
	}

	std::cout << "Se mataron: " << cant-sanguijuelasDiscretizadas.size() << " sanguijuela(s). El parabrisas esta estable." << std::endl;
}

#endif
