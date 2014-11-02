#ifndef SPLINE_H
#define SPLINE_H

#include <vector>

std::vector<double> generar_spline(std::vector<double> fx);

void spline(cimg_library::CImg<double>& orig);

#endif