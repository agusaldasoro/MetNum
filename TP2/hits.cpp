void normalizar(std::vector<double> v)
{
	double suma = 0.0;

	for (int i = 0; i < v.size(); ++i)
	{
		s += pow(v[i],2);
	}

	double norma = sqrt(suma);

	for (int i = 0; i < v.size(); ++i)
	{
		v[i] = v[i]/norma;
	}
}


HITS
{
	double x[n], y[n];

	std::vector<double> x(n,1);
	std::vector<double> y(n,1);

	// for (int i = 0; i < n; ++i)
	// {
	// 	x[i] = 1.0;
	// 	y[i] = 1.0;
	// }

	CICLO
	{
		A.trasponer(); //A = At
		x = A.multMatVec(x);
		x.normalizar();

		A.trasponer(); //A = A
		y = A.multMatVec(y);
		y.normalizar();
	}
}