#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_filter.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_fft_complex.h>

#define REAL(z,i) ((z)[2*(i)])   
#define IMAG(z,i) ((z)[2*(i)+1])

const double kt = 298 * 1.3806488e-23;

struct npfdata {
    size_t n;
    double* t;
    double* y;
};

gsl_vector* gaussSmooth(gsl_vector* x, float s, int w);
gsl_vector* meanSmooth(gsl_vector* x, int w);
std::vector<std::pair<int, int>> findPeak(double* x, double* b, size_t size, int t, float&, float&);
//int npfit(npfdata* data, gsl_vector* para, double& chisq);