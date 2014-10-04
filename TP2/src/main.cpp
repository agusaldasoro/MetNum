#include <fstream>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "sparse.h"
#include "hits.h"
#include "pagerank.h"
#include "indeg.h"
#include <limits>

using namespace std;

int main(int argc, char const *argv[])
{
  if (*argv[3]=='0') //SNAP
  {
    ifstream entrada(argv[4]);
    if (!entrada.is_open())
      cout << "Path incorrecto." << endl;
    else
    {
      int dim,k;
      string line;
      getline(entrada,line);
      entrada.ignore(2);
      entrada >> line;
      while(line != "Nodes:"){
        getline(entrada,line);
        entrada.ignore(2);
        entrada >> line;
      }
      entrada >> dim;
      entrada.ignore(8);
      entrada >> k;
      getline(entrada,line);
      getline(entrada,line);
      //cout << "Dim: " << dim << " Iteraciones: " << k << endl;

      MatrizEsparsa a(dim);

      if (*argv[1]=='0') //PageRank
      {
        a.trasponer();
        int i, j, col;
        double contador;
        entrada >> j;                       //leo la primer coordenada

        while (k > 0){
          std::vector<int> filas;
          entrada >> i;                     //leo la segunda
          filas.push_back(i);               //y la guardo en mi arreglo de filas
          contador = 1;                     //seteo en 1 el contador
          entrada >> col;                   //leo la primer coordenada de la siguiente linea

          while(entrada.good() && j==col){  //mientras la primer coordenada de la linea de abajo sea igual
            entrada >> i;                   //extraigo la segunda coordenada,
            filas.push_back(i);             //la guardo en el arreglo
            contador++;                     //incremento el contador
            entrada >> col;                 //extraigo la primer coordenada de la linea de abajo y repito
          }

          double pij = 1/contador;          //cuando termine, calculo el coeficiente que va a ir en cada columna (1/nj)
          for (int i = 0; i < filas.size(); ++i)
            a.definirPos(j-1,filas[i]-1,pij);

          j = col;                          //como a esta altura j != col, los igualo para no saltearme ninguna linea
          k -= contador;                    //acabo de definir "contador" coordenadas, asi que le resto esa cant. a k
        }

        string nombre(argv[4]);
        string nombreSalida(nombre.begin(),nombre.end()-4);
        nombreSalida.append(".out");
        const char *nsalida = nombreSalida.c_str();
        ofstream salida(nsalida);

        salida.setf( std::ios::fixed, std::ios::floatfield );
        salida.precision(6);
        double tol = strtod(argv[5], NULL);
        double c = strtod(argv[2], NULL);
        pagerank(a, c, tol, salida);

        salida.close();
      }
      else if (*argv[1]=='1') //HITS
      {
        int i, j;
        while (k > 0){
          entrada >> i >> j;
          //cout << "Fila: " << i << endl << "Columna: " << j << endl;
          a.definirPos(i-1,j-1,1);
          k--;
        }
        //a.imprimir();
        string nombre(argv[4]);
        string nombreSalida(nombre.begin(),nombre.end()-4);
        nombreSalida.append(".out");
        const char *nsalida = nombreSalida.c_str();
        ofstream salida(nsalida);

        salida.setf( std::ios::fixed, std::ios::floatfield );
        salida.precision(6);
        double tol = strtod(argv[5], NULL);
        hits(a, tol, salida);

        salida.close();
      }
      else if (*argv[1]=='2') //In-Deg
      {
        int i, j;
        while (k > 0){
          entrada >> i >> j;
          a.definirPos(i-1,j-1,1);
          k--;
        }
        //a.imprimir();
        string nombre(argv[4]);
        string nombreSalida(nombre.begin(),nombre.end()-4);
        nombreSalida.append(".out");
        const char *nsalida = nombreSalida.c_str();
        ofstream salida(nsalida);
        indeg(a, salida);
        salida.close();
      }
      else
        cout << "Primer parámetro no válido: escribir 0 para PageRank, 1 para HITS, 2 para In-Deg." << endl;
    }
    entrada.close();
  }
  else if (*argv[3]=='1') //No SNAP
  {
    string nodes(argv[4]);
    string adjlist(argv[4]);
    nodes.append("/nodes");
    adjlist.append("/adj_list");
    const char *entrada1 = nodes.c_str();
    const char *entrada2 = adjlist.c_str();
    ifstream nodos(entrada1);
    ifstream ady(entrada2);

    if (!(nodos.is_open() && ady.is_open()))
      cout << "Path incorrecto." << endl;
    else
    {
      int dim, k;
      string line;

      nodos >> dim;

      k = 0;
      MatrizEsparsa a(dim);

      if (*argv[1]=='0') //PageRank
      {
        int j;
        double od, pij;

        getline(nodos,line);
        getline(nodos,line);

        while(k<dim){
          getline(nodos,line);
          //cout << line << endl;
          getline(nodos,line);
          //cout << line << endl;
          getline(nodos,line);
          //cout << line << endl;

          nodos >> od >> od;
          //cout << od << endl;

          pij = 1/od;

          getline(nodos,line);
          getline(nodos,line);

          ady.ignore(numeric_limits<streamsize>::max(),' ');

          for (int i = 0; i < od; ++i){
            ady >> j;
            //cout << j << endl;
            a.definirPos(j,k,pij);
          }

          k++;
          getline(ady,line);
        }

        //a.imprimir();
        string nombreSalida(argv[4]);
        nombreSalida.append("/pagerank.out");
        const char *nsalida = nombreSalida.c_str();
        ofstream salida(nsalida);

        salida.setf( std::ios::fixed, std::ios::floatfield );
        salida.precision(6);
        double tol = strtod(argv[5], NULL);
        double c = strtod(argv[2], NULL);
        //cout << "Tele: " << c << "Tolerancia: " << tol << endl;
        pagerank(a, c, tol, salida);

        salida.close();

      }
      else if (*argv[1]=='1') //HITS
      {
        int j;
        while(k<dim)
        {
          ady.ignore(numeric_limits<streamsize>::max(), ' ');
          ady >> j;
          while(j != -1){
            a.definirPos(k,j,1);
            ady >> j;
          }
          k++;
          getline(ady,line);
        }

        //a.imprimir();
        string nombreSalida(argv[4]);
        nombreSalida.append("/hits.out");
        const char *nsalida = nombreSalida.c_str();
        ofstream salida(nsalida);

        salida.setf( std::ios::fixed, std::ios::floatfield );
        salida.precision(6);
        double tol = strtod(argv[5], NULL);
        hits(a, tol, salida);

        salida.close();

      }
      else if (*argv[1]=='2') //In-Deg
      {
        int j;
        while(k<dim)
        {
          ady.ignore(numeric_limits<streamsize>::max(), ' ');
          ady >> j;
          while(j != -1){
            //cout << "Fila: " << k << "Col: " << j << endl;
            a.definirPos(k,j,1);
            ady >> j;
          }
          k++;
          getline(ady,line);
        }
        //cout << "SALI" << endl;
        //a.imprimir();
        string nombreSalida(argv[4]);
        nombreSalida.append("/indeg.out");
        const char *nsalida = nombreSalida.c_str();
        ofstream salida(nsalida);
        indeg(a, salida);
        salida.close();
      }
      else
        cout << "Primer parámetro no válido: escribir 0 para PageRank, 1 para HITS, 2 para In-Deg." << endl;
    }

    nodos.close();
    ady.close();
  }

  return 0;
}
