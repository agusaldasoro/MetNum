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

  	Parabrisas nuevo(a,b,h,r,t,pos,argv[3]);

  	nuevo.CalcularTemperaturas(argv[2],argv[3]);

    cout << nuevo.EsEstable() << endl;
    cout << nuevo.TempPtoCritico() << endl;

    if (argc==5)
    {
      nuevo.MatarSanguijuelasDEA1(argv[4]);
      //nuevo.MatarSanguijuelasProximas(argv[4]);
      //nuevo.MatarSanguijuelasPorcentaje(argv[4]);

      cout << nuevo.EsEstable() << endl;
      cout << nuevo.TempPtoCritico() << endl;
    }

	return 0;
}