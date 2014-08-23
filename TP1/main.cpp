#include <iostream>
#include "parabrisas.h"

using namespace std;

int main(int argc, char const *argv[])
{
	ifstream entrada(argv[1]);

	double a, b, h, r, t, k;
	entrada >> a >> b >> h >> r >> t >> k;

	std::vector<Posicion> pos;

	while (k > 0)
  	{
  		double x, y;
  		entrada >> x >> y;

  		Posicion s;
  		s.x = x;
  		s.y = y;
		pos.push_back(s);

    	k--;
  	}

  	entrada.close();

  	Parabrisas nuevo(a,b,h,r,t,pos);

  	nuevo.CalcularTemperaturas(argv[2]); //AGREGAR OTRO PARAMETRO QUE SEA SI ES MATRIZ BANDA O NO

	return 0;
}