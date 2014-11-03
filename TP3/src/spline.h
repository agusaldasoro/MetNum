#ifndef SPLINE_H
#define SPLINE_H

#include <vector>

std::vector<double> generar_spline(std::vector<double> fx);

double evaluar(std::vector<double> spline, unsigned int x);

void spline(cimg_library::CImg<double>& orig);

void spline_der(cimg_library::CImg<double>& orig);

#endif