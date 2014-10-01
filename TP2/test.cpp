/*#include "sparse.cpp"

int main(int argc, char const *argv[])
{
	MatrizEsparsa hola(4);
	hola.definirPos(1,1,1);
	hola.definirPos(0,3,1);
	hola.definirPos(1,3,8);
	hola.definirPos(0,1,2);
	hola.definirPos(2,3,4);
	hola.definirPos(0,0,1);
	hola.definirPos(3,0,7);
	hola.definirPos(3,1,3);
	hola.imprimir();

	std::vector<double> v;
	v.push_back(4);
	v.push_back(3);
	v.push_back(2);
	v.push_back(6);

	std::vector<double> sol = hola.multMatVec(v);

	std::cout << "Solucion: [";
	for (int i = 0; i < sol.size()-1; ++i)
	{
		std::cout << sol[i] << ", ";
	}
	std::cout << sol[sol.size()-1] << "]" << std::endl;

	hola.trasponer();

	sol = hola.multMatVec(v);

	std::cout << "Solucion: [";
	for (int i = 0; i < sol.size()-1; ++i)
	{
		std::cout << sol[i] << ", ";
	}
	std::cout << sol[sol.size()-1] << "]" << std::endl;

	return 0;
}*/