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

    cout.setf( std::ios::fixed, std::ios::floatfield );
    cout.precision(5);

    cout << "La temperatura en el punto crítico es: " << nuevo.TempPtoCritico() << endl;

    if (argc==6)
    {
      if (*argv[5]=='1') //se llama al programa con MatarSanguijuelasDEA1
      {
        nuevo.MatarSanguijuelasDEA1(argv[4]);
      }else if (*argv[5]=='2') //se llama al programa con MatarSanguijuelasProximas
      {
        nuevo.MatarSanguijuelasProximas(argv[4]);
      }else if (*argv[5]=='3') //se llama al programa con MatarSanguijuelasPorcentaje
      {
        nuevo.MatarSanguijuelasPorcentaje(argv[4]);
      }

      cout << "La nueva temperatura en el punto crítico es: " << nuevo.TempPtoCritico() << endl;
    }

	return 0;
}