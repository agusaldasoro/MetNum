#include "sparse.cpp"

int main(int argc, char const *argv[])
{
	MatrizEsparsa hola(2);
	hola.definirPos(1,1,5);
	hola.definirPos(1,0,1);
	hola.definirPos(0,1,2);
	hola.definirPos(0,0,3);
	hola.imprimir();

	hola.trasponer();

	std::vector<double> v;
	v.push_back(2);
	v.push_back(1);

	std::vector<double> sol = hola.multMatVec(v);

	std::cout << "Solucion: [";
	for (int i = 0; i < sol.size()-1; ++i)
	{
		std::cout << sol[i] << ", ";
	}
	std::cout << sol[sol.size()-1] << "]" << std::endl;
	return 0;
}