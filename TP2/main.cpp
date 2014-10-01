#include <fstream>
#include <string>
#include <iostream>
#include "sparse.h"
#include "hits.h"

using namespace std;

int main(int argc, char const *argv[])
{
  ifstream entrada(argv[4]);

  if (!entrada.is_open())
    cout << "Path incorrecto." << endl;
  else
  {
    if (*argv[3]=='0') //SNAP
    {
      int dim,k;
      string line;
      getline(entrada,line);
      //getline(entrada,line);
      entrada.ignore(9);
      entrada >> dim;
      entrada.ignore(8);
      entrada >> k;
      getline(entrada,line);
      getline(entrada,line);
      cout << "Dim: " << dim << " Iteraciones: " << k << endl;
      MatrizEsparsa a(dim);

      if (*argv[1]=='0')
      {
        //PageRank
      }
      else if (*argv[1]=='1') //HITS
      {
        int iteraciones = k;
        while (k > 0){
          int i, j;
          entrada >> i >> j;
          cout << "Fila: " << i << endl << "Columna: " << j << endl;
          a.definirPos(i-1,j-1,1);
          k--;
        }
        a.imprimir();
        //string nombresalida(argv[4]);
        ofstream salida("probando.out");
        salida.setf( std::ios::fixed, std::ios::floatfield );
        salida.precision(6);
        //salida.open(nombresalida.append(".out"), ofstream::out);
        cout << "Entre..." <<endl;
        hits(a, *argv[5], iteraciones, salida);
        cout << "Sali!!!" <<endl;
        salida.close();
      }
      else if (*argv[1]=='2')
      {
        //In-Deg
      }
      else
        cout << "Primer parámetro no válido: escribir 0 para PageRank, 1 para HITS, 2 para In-Deg." << endl;
    }
    else if (*argv[3]=='1') //No SNAP
    {
      /* code */
    }
  }

	entrada.close();

	return 0;
}