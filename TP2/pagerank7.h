#include "sparse.h"
#include "vectores.h"

void pagerank(MatrizEsparsa a, double c, double tol, std::ofstream& salida)
{
	double n = a.dimension();

	int maxIt = 1000; //NOSOTROS TENEMOS QUE ELEGIR LA CANT MAX DE ITERACIONES

	c = 0.05;

	std::vector<double> dif(n);
	std::vector<double> x(n,1), xp;
	double factor = ((1-c)/n);

	std::ofstream normaFile05("normasPageRank05.out");
	std::ofstream salida05("stanford_pagerank05.out");
	normaFile05.setf( std::ios::fixed, std::ios::floatfield );
	normaFile05.precision(6);
	salida05.setf( std::ios::fixed, std::ios::floatfield );
	salida05.precision(6);

	int j = 0;
	while(j < maxIt)
	{

		xp = a.multMatVec(x);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]*c;

		double norma1p = 0.0;
		for (int i = 0; i < xp.size(); ++i)
			norma1p += fabs(xp[i]);

		double norma1 = 0.0;
		for (int i = 0; i < x.size(); ++i)
			norma1 += fabs(x[i]);

		double w = norma1 - norma1p;
		w = w/n;

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]+w;

		if (vecIguales(x,xp,tol))
			j=maxIt;
		else
			j++;


		for (int i = 0; i < x.size(); ++i)
			dif[i] = x[i]-xp[i];

		x = xp;

		double normaprint = 0;
		for (int i = 0; i < x.size(); ++i)
			normaprint += fabs(dif[i]);
		normaFile05 /*<< "Norma iteracion " << j << ": "*/ << normaprint << std::endl;
	}

	double norma = 0.0;
	for (int i = 0; i < x.size(); ++i)
		norma += fabs(x[i]);

	for (int i = 0; i < x.size(); ++i)
		x[i] = x[i]/norma;

	for (int i = 0; i < x.size(); ++i)
		salida05 << x[i] << std::endl;

	normaFile05.close();
	salida05.close();

// AHORA CON 0.15

	c = 0.15;

	for (int i = 0; i < n; ++i)
	{
		x[i] = 1;
	}

	factor = ((1-c)/n);

	std::ofstream normaFile15("normasPageRank15.out");
	std::ofstream salida15("stanford_pagerank15.out");
	normaFile15.setf( std::ios::fixed, std::ios::floatfield );
	normaFile15.precision(6);
	salida15.setf( std::ios::fixed, std::ios::floatfield );
	salida15.precision(6);

	j = 0;
	while(j < maxIt)
	{

		xp = a.multMatVec(x);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]*c;

		double norma1p = 0.0;
		for (int i = 0; i < xp.size(); ++i)
			norma1p += fabs(xp[i]);

		double norma1 = 0.0;
		for (int i = 0; i < x.size(); ++i)
			norma1 += fabs(x[i]);

		double w = norma1 - norma1p;
		w = w/n;

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]+w;

		if (vecIguales(x,xp,tol))
			j=maxIt;
		else
			j++;


		for (int i = 0; i < x.size(); ++i)
			dif[i] = x[i]-xp[i];

		x = xp;

		double normaprint = 0;
		for (int i = 0; i < x.size(); ++i)
			normaprint += fabs(dif[i]);
		normaFile15 /*<< "Norma iteracion " << j << ": "*/ << normaprint << std::endl;
	}

	norma = 0.0;
	for (int i = 0; i < x.size(); ++i)
		norma += fabs(x[i]);

	for (int i = 0; i < x.size(); ++i)
		x[i] = x[i]/norma;

	for (int i = 0; i < x.size(); ++i)
		salida15 << x[i] << std::endl;

	normaFile15.close();
	salida15.close();

// ahora con 0.30


	c = 0.30;

	for (int i = 0; i < n; ++i)
	{
		x[i] = 1;
	}

	factor = ((1-c)/n);

	std::ofstream normaFile30("normasPageRank30.out");
	std::ofstream salida30("stanford_pagerank30.out");
	normaFile30.setf( std::ios::fixed, std::ios::floatfield );
	normaFile30.precision(6);
	salida30.setf( std::ios::fixed, std::ios::floatfield );
	salida30.precision(6);

	j = 0;
	while(j < maxIt)
	{

		xp = a.multMatVec(x);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]*c;

		double norma1p = 0.0;
		for (int i = 0; i < xp.size(); ++i)
			norma1p += fabs(xp[i]);

		double norma1 = 0.0;
		for (int i = 0; i < x.size(); ++i)
			norma1 += fabs(x[i]);

		double w = norma1 - norma1p;
		w = w/n;

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]+w;

		if (vecIguales(x,xp,tol))
			j=maxIt;
		else
			j++;


		for (int i = 0; i < x.size(); ++i)
			dif[i] = x[i]-xp[i];

		x = xp;

		double normaprint = 0;
		for (int i = 0; i < x.size(); ++i)
			normaprint += fabs(dif[i]);
		normaFile30 /*<< "Norma iteracion " << j << ": "*/ << normaprint << std::endl;
	}

	norma = 0.0;
	for (int i = 0; i < x.size(); ++i)
		norma += fabs(x[i]);

	for (int i = 0; i < x.size(); ++i)
		x[i] = x[i]/norma;

	for (int i = 0; i < x.size(); ++i)
		salida30 << x[i] << std::endl;

	normaFile30.close();
	salida30.close();

// ahora con 50


	c = 0.50;

	for (int i = 0; i < n; ++i)
	{
		x[i] = 1;
	}

	factor = ((1-c)/n);

	std::ofstream normaFile50("normasPageRank50.out");
	std::ofstream salida50("stanford_pagerank50.out");
	normaFile50.setf( std::ios::fixed, std::ios::floatfield );
	normaFile50.precision(6);
	salida50.setf( std::ios::fixed, std::ios::floatfield );
	salida50.precision(6);

	j = 0;
	while(j < maxIt)
	{

		xp = a.multMatVec(x);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]*c;

		double norma1p = 0.0;
		for (int i = 0; i < xp.size(); ++i)
			norma1p += fabs(xp[i]);

		double norma1 = 0.0;
		for (int i = 0; i < x.size(); ++i)
			norma1 += fabs(x[i]);

		double w = norma1 - norma1p;
		w = w/n;

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]+w;

		if (vecIguales(x,xp,tol))
			j=maxIt;
		else
			j++;


		for (int i = 0; i < x.size(); ++i)
			dif[i] = x[i]-xp[i];

		x = xp;

		double normaprint = 0;
		for (int i = 0; i < x.size(); ++i)
			normaprint += fabs(dif[i]);
		normaFile50 /*<< "Norma iteracion " << j << ": "*/ << normaprint << std::endl;
	}

	norma = 0.0;
	for (int i = 0; i < x.size(); ++i)
		norma += fabs(x[i]);

	for (int i = 0; i < x.size(); ++i)
		x[i] = x[i]/norma;

	for (int i = 0; i < x.size(); ++i)
		salida50 << x[i] << std::endl;

	normaFile50.close();
	salida50.close();

// ahora con 0.75


	c = 0.75;

	for (int i = 0; i < n; ++i)
	{
		x[i] = 1;
	}

	factor = ((1-c)/n);

	std::ofstream normaFile75("normasPageRank75.out");
	std::ofstream salida75("stanford_pagerank75.out");
	normaFile75.setf( std::ios::fixed, std::ios::floatfield );
	normaFile75.precision(6);
	salida75.setf( std::ios::fixed, std::ios::floatfield );
	salida75.precision(6);

	j = 0;
	while(j < maxIt)
	{

		xp = a.multMatVec(x);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]*c;

		double norma1p = 0.0;
		for (int i = 0; i < xp.size(); ++i)
			norma1p += fabs(xp[i]);

		double norma1 = 0.0;
		for (int i = 0; i < x.size(); ++i)
			norma1 += fabs(x[i]);

		double w = norma1 - norma1p;
		w = w/n;

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]+w;

		if (vecIguales(x,xp,tol))
			j=maxIt;
		else
			j++;


		for (int i = 0; i < x.size(); ++i)
			dif[i] = x[i]-xp[i];

		x = xp;

		double normaprint = 0;
		for (int i = 0; i < x.size(); ++i)
			normaprint += fabs(dif[i]);
		normaFile75 /*<< "Norma iteracion " << j << ": "*/ << normaprint << std::endl;
	}

	norma = 0.0;
	for (int i = 0; i < x.size(); ++i)
		norma += fabs(x[i]);

	for (int i = 0; i < x.size(); ++i)
		x[i] = x[i]/norma;

	for (int i = 0; i < x.size(); ++i)
		salida75 << x[i] << std::endl;

	normaFile75.close();
	salida75.close();

// ahora con 0.85

	c = 0.85;

	for (int i = 0; i < n; ++i)
	{
		x[i] = 1;
	}

	factor = ((1-c)/n);

	std::ofstream normaFile85("normasPageRank85.out");
	std::ofstream salida85("stanford_pagerank85.out");
	normaFile85.setf( std::ios::fixed, std::ios::floatfield );
	normaFile85.precision(6);
	salida85.setf( std::ios::fixed, std::ios::floatfield );
	salida85.precision(6);

	j = 0;
	while(j < maxIt)
	{

		xp = a.multMatVec(x);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]*c;

		double norma1p = 0.0;
		for (int i = 0; i < xp.size(); ++i)
			norma1p += fabs(xp[i]);

		double norma1 = 0.0;
		for (int i = 0; i < x.size(); ++i)
			norma1 += fabs(x[i]);

		double w = norma1 - norma1p;
		w = w/n;

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]+w;

		if (vecIguales(x,xp,tol))
			j=maxIt;
		else
			j++;


		for (int i = 0; i < x.size(); ++i)
			dif[i] = x[i]-xp[i];

		x = xp;

		double normaprint = 0;
		for (int i = 0; i < x.size(); ++i)
			normaprint += fabs(dif[i]);
		normaFile85 /*<< "Norma iteracion " << j << ": "*/ << normaprint << std::endl;
	}

	norma = 0.0;
	for (int i = 0; i < x.size(); ++i)
		norma += fabs(x[i]);

	for (int i = 0; i < x.size(); ++i)
		x[i] = x[i]/norma;

	for (int i = 0; i < x.size(); ++i)
		salida85 << x[i] << std::endl;

	normaFile85.close();
	salida85.close();

// ahora con 0.95


	c = 0.95;

	for (int i = 0; i < n; ++i)
	{
		x[i] = 1;
	}

	factor = ((1-c)/n);

	std::ofstream normaFile95("normasPageRank95.out");
	std::ofstream salida95("stanford_pagerank95.out");
	normaFile95.setf( std::ios::fixed, std::ios::floatfield );
	normaFile95.precision(6);
	salida95.setf( std::ios::fixed, std::ios::floatfield );
	salida95.precision(6);


	j = 0;
	while(j < maxIt)
	{

		xp = a.multMatVec(x);

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]*c;

		double norma1p = 0.0;
		for (int i = 0; i < xp.size(); ++i)
			norma1p += fabs(xp[i]);

		double norma1 = 0.0;
		for (int i = 0; i < x.size(); ++i)
			norma1 += fabs(x[i]);

		double w = norma1 - norma1p;
		w = w/n;

		for (int i = 0; i < xp.size(); ++i)
			xp[i] = xp[i]+w;

		if (vecIguales(x,xp,tol))
			j=maxIt;
		else
			j++;


		for (int i = 0; i < x.size(); ++i)
			dif[i] = x[i]-xp[i];

		x = xp;

		double normaprint = 0;
		for (int i = 0; i < x.size(); ++i)
			normaprint += fabs(dif[i]);
		normaFile95 /*<< "Norma iteracion " << j << ": "*/ << normaprint << std::endl;
	}

	norma = 0.0;
	for (int i = 0; i < x.size(); ++i)
		norma += fabs(x[i]);

	for (int i = 0; i < x.size(); ++i)
		x[i] = x[i]/norma;

	for (int i = 0; i < x.size(); ++i)
		salida95 << x[i] << std::endl;

	normaFile95.close();
	salida95.close();

}
