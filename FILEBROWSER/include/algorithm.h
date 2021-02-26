#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_filter.h>
#include <gsl/gsl_statistics.h>

gsl_vector* gaussSmooth(gsl_vector* x, float s, int w);
gsl_vector* meanSmooth(gsl_vector* x, int w);
std::vector<std::pair<int, int>> findPeak(double* x, double* b, size_t size, int t);