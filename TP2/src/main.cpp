#include <fstream>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "sparse.h"
#include "hits.h"
#include "pagerank.h"
#include "indeg.h"
#include <limits>
#include <chrono>

using namespace std;

int main(int argc, char const *argv[])
{
	std::chrono::time_point<std::chrono::system_clock> start, end;

	ifstream main(argv[1]);
	int alg, inst;
	double c, tol;
	string path;
	const char* nombrePath;
	main >> alg >> c >> inst >> path >> tol;
	//cout << "Algoritmo: " << alg << "C: " << c << "Instancia: " << inst << "Path: " << path << "Tolerancia: " << tol;

	nombrePath =  path.c_str();

	if (inst==0) //SNAP
	{
		ifstream entrada(nombrePath);
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

			if (alg==0) //PageRank
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
					cout << "Fila: " << col << endl;
					double pij = 1/contador;          //cuando termine, calculo el coeficiente que va a ir en cada columna (1/nj)
					for (int i = 0; i < filas.size(); ++i)
						a.definirPos(j-1,filas[i]-1,pij);

					j = col;                          //como a esta altura j != col, los igualo para no saltearme ninguna linea
					k -= contador;                    //acabo de definir "contador" coordenadas, asi que le resto esa cant. a k
			    }

			    ofstream salida(argv[2]);
			    salida.setf( std::ios::fixed, std::ios::floatfield );
			    salida.precision(6);

			    start = std::chrono::system_clock::now();
			    pagerank(a, c, tol, salida);
			    end = std::chrono::system_clock::now();

			    salida.close();
			}
			else if (alg==1) //HITS
			{
				int i, j;
				while (k > 0){
					entrada >> i >> j;
					cout << "Fila: " << i << endl << "Columna: " << j << endl;
					a.definirPos(i-1,j-1,1);
					k--;
				}
				//a.imprimir();
				ofstream salida(argv[2]);
				salida.setf( std::ios::fixed, std::ios::floatfield );
				salida.precision(6);

				start = std::chrono::system_clock::now();
				hits(a, tol, salida);
				end = std::chrono::system_clock::now();

				salida.close();
			}
			else if (alg==2) //In-Deg
			{
				int i, j;
				while (k > 0){
					entrada >> i >> j;
					a.definirPos(i-1,j-1,1);
					k--;
				}
				//a.imprimir();
				ofstream salida(argv[2]);
				salida.setf( std::ios::fixed, std::ios::floatfield );
				salida.precision(6);

				start = std::chrono::system_clock::now();
				indeg(a, salida);
				end = std::chrono::system_clock::now();

				salida.close();
			}
			else
				cout << "Primer parámetro no válido: escribir 0 para PageRank, 1 para HITS, 2 para In-Deg." << endl;
		}
	entrada.close();
	}
	else if (inst==1) //No SNAP
	{
		string nodes(path);
		string adjlist(path);
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

			if (alg==0) //PageRank
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
				ofstream salida(argv[2]);
				salida.setf( std::ios::fixed, std::ios::floatfield );
				salida.precision(6);
				//cout << "Tele: " << c << "Tolerancia: " << tol << endl;
				start = std::chrono::system_clock::now();
				pagerank(a, c, tol, salida);
				end = std::chrono::system_clock::now();

				salida.close();
			}
			else if (alg==1) //HITS
			{
				int j;
				while(k<dim){
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
				ofstream salida(argv[2]);
				salida.setf( std::ios::fixed, std::ios::floatfield );
				salida.precision(6);

				start = std::chrono::system_clock::now();
				hits(a, tol, salida);
				end = std::chrono::system_clock::now();

				salida.close();

			}
			else if (alg==2) //In-Deg
			{
				int j;
				while(k<dim){
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
				ofstream salida(argv[2]);
				salida.setf( std::ios::fixed, std::ios::floatfield );
				salida.precision(6);

				start = std::chrono::system_clock::now();
				indeg(a, salida);
				end = std::chrono::system_clock::now();

				salida.close();
			}
			else
				cout << "Primer parámetro no válido: escribir 0 para PageRank, 1 para HITS, 2 para In-Deg." << endl;
		}
	nodos.close();
	ady.close();
	}

	std::chrono::duration<double> elapsed_seconds = end-start;
	cout << "Tiempo: " << elapsed_seconds.count() << endl;

	return 0;
}
