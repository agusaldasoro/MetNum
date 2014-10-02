#include <fstream>
#include <string>
#include <iostream>
#include "sparse.h"
#include "hits.h"
#include "pagerank.h"
#include "indeg.h"

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
        int i, j, col;
        double contador;
        entrada >> j >> i;
        col = j;
        while (k > 0){
          contador = 0;
          std::vector<int> filas;
          //cout << "HOLA1" << endl;
          while(entrada.good() && j==col)
          {
            contador++;
            filas.push_back(i);
            entrada >> col >> i;
            //cout << "Fila: " << col << endl << "Columna: " << i << endl;
          }
          //cout << "HOLA2" << endl;
          double pij = 1/contador;
          // cout << "Pij: " << pij << endl;
          // cout << "Contador: " << contador << endl;
          for (int i = 0; i < filas.size(); ++i)
            a.definirPos(filas[i]-1,j-1,pij);

          j=col;

          k -= contador;
        }
        //a.imprimir();

        string nombre(argv[4]);
        string nombreSalida(nombre.begin(),nombre.end()-4);
        nombreSalida.append(".out");
        ofstream salida(nombreSalida);

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
        ofstream salida(nombreSalida);

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
        ofstream salida(nombreSalida);
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
    ifstream nodos(nodes);
    ifstream ady(adjlist);

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
        ofstream salida(nombreSalida);

        salida.setf( std::ios::fixed, std::ios::floatfield );
        salida.precision(6);
        double tol = strtod(argv[5], NULL);
        double c = strtod(argv[2], NULL);
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
        ofstream salida(nombreSalida);

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
        ofstream salida(nombreSalida);
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