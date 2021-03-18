#include "tools.h"

gsl_vector* gaussSmooth(gsl_vector* x, float s, int w) {
    gsl_vector* y = gsl_vector_alloc(x->size);
    gsl_vector* k = gsl_vector_alloc(w);
    gsl_filter_gaussian_workspace* gauss_p = gsl_filter_gaussian_alloc(w);
    gsl_filter_gaussian_kernel(s, 0, 1, k);
    gsl_filter_gaussian(GSL_FILTER_END_PADVALUE, s, 0, x, y, gauss_p);
    gsl_vector_free(k);
    gsl_filter_gaussian_free(gauss_p);
    return y;
}   

gsl_vector* meanSmooth(gsl_vector* x, int w) {
    gsl_movstat_workspace* mean_p = gsl_movstat_alloc(w);
    gsl_vector* y = gsl_vector_alloc(x->size);
    gsl_movstat_mean(GSL_MOVSTAT_END_PADVALUE, x, y, mean_p);
    gsl_movstat_free(mean_p);
    return y;
}


std::vector<std::pair<int, int>> findPeak(double* x, double* b, size_t size, int t, float& sd, float& mean) {
    std::vector<std::pair<int, int>> out;
    sd = gsl_stats_sd(x, 1, size);
    mean = gsl_stats_mean(x, 1, size);
    unsigned int j;
    bool flag = false;
    for (int i = 0; i < size; i++) {
        if (mean > 0) {
            if (!flag && b[i] - x[i] > t * sd && abs(b[i] - mean) < 1.5 * sd) {
                j = i;
                while (!(j < 1 || abs(b[j] - x[j]) < sd))
                    j--;
                flag = true;
            }
            if (flag && abs(b[j] - b[i]) < 0.8 * sd && abs(b[i] - x[i]) < 1.5 * sd) {
                flag = false;
                out.push_back(std::pair<int, int>(j, i));
            }       
        }
        else {
            if (!flag && x[i] - b[i] > t * sd && abs(b[i] - mean) < 1.5 * sd) {
                j = i;
                while (!(j < 1 || abs(b[j] - x[j]) < sd))
                    j--;
                flag = true;
            }
            if (flag && abs(b[j] - b[i]) < 0.8 * sd && abs(b[i] - x[i]) < 1.5 * sd) {
                flag = false;
                out.push_back(std::pair<int, int>(j, i));
            }
        }
        
    }
    return out;
}
/*
void fftconv(double* x, double* y, size_t n) {
    size_t N = pow(2, 14);
    double* x_f = new double[2*N];
    double* y_f = new double[2*N];
    for (int i = 0; i < n; i++) {
        REAL(x_f, i) = x_f[i];
        REAL(y_f, i) = y_f[i];
        IMAG(x_f, i) = 0;
        IMAG(y_f, i) = 0;
    }
    for (int i = n; i < N; i++) {
        REAL(x_f, i) = 0;
        REAL(y_f, i) = 0;
        IMAG(x_f, i) = 0;
        IMAG(y_f, i) = 0;
    }
    gsl_fft_complex_radix2_forward(x_f, 1, N);
    gsl_fft_complex_radix2_forward(y_f, 1, N);
    for (int i = 0; i < 2 * N; i++) 
        y_f[i] = x_f[i] * y_f[i];
    gsl_fft_complex_radix2_inverse(y_f, 1, N);
    for (int i = 0; i < n; i++) 
        y[i] = REAL(y_f, i);
    delete[] y_f;
    delete[] x_f;
    return;
}

double func1(int x, double sigma) {
    return exp(-0.5 * gsl_pow_2(x / sigma))/(M_SQRT2 * M_SQRTPI *sigma);
}

double func2(int x, double xmin, double xmax, double u) {
    if (x <= xmin || x >= xmax)
        return 0;
    return cosh(u * 3.33356E-30 * sqrt((x - xmin) / (xmax - xmin)) / kt) / sqrt((x - xmax) * (xmin - x));
}

int npf_f(const gsl_vector* x, void* data, gsl_vector* f) {
    size_t n = ((struct npfdata*)data)->n;
    double* t = ((struct npfdata*)data)->t;
    double* y = ((struct npfdata*)data)->y;
    size_t N = pow(2, 13);
    double* xt = new double[N];
    double* yg = new double[N];
    double* yb = new double[N];
    for (int i = 0; i < N; i++) {
        xt[i] = -4095 + i;
        yb[i] = func2(xt[i], gsl_vector_get(x,1), gsl_vector_get(x, 2), gsl_vector_get(x, 3));
        yg[i] = func1(xt[i], gsl_vector_get(x, 2));
    }
    fftconv(yg, yb, N);
    gsl_interp_accel* acc = gsl_interp_accel_alloc();
    gsl_spline* spline = gsl_spline_alloc(gsl_interp_cspline, N);
    gsl_spline_init(spline, xt, yb, N);
    for (int i = 0; i < n; i++) 
        gsl_vector_set(f, i, y[i] - gsl_spline_eval(spline, t[i], acc));

    delete[] xt;
    delete[] yb;
    delete[] yg;
    gsl_spline_free(spline);
    gsl_interp_accel_free(acc);
}
*/
int npf_f(const gsl_vector* x, void* data, gsl_vector* f) {
    size_t n = ((struct npfdata*)data)->n;
    double* t = ((struct npfdata*)data)->t;
    double* y = ((struct npfdata*)data)->y;

    double A1 = gsl_vector_get(x, 0);
    double A2 = gsl_vector_get(x, 1);
    double B1 = gsl_vector_get(x, 2);
    double B2 = gsl_vector_get(x, 3);
    double C1 = gsl_vector_get(x, 4);
    double C2 = gsl_vector_get(x, 5);

    for (size_t i = 0; i < n; i++)
    {
        double Y1 = A1  * exp(-0.5 * gsl_pow_2((t[i] - B1) / C1));
        double Y2 = A2  * exp(-0.5 * gsl_pow_2((t[i] - B2) / C2));
        gsl_vector_set(f, i, y[i] - Y1 - Y2);
    }

    return GSL_SUCCESS;
}

int npf_df(const gsl_vector* x, void* data, gsl_matrix* J) {
    size_t n = ((struct npfdata*)data)->n;
    double* t = ((struct npfdata*)data)->t;

    double A1 = gsl_vector_get(x, 0);
    double A2 = gsl_vector_get(x, 1);
    double B1 = gsl_vector_get(x, 2);
    double B2 = gsl_vector_get(x, 3);
    double C1 = gsl_vector_get(x, 4);
    double C2 = gsl_vector_get(x, 5);

    for (size_t i = 0; i < n; i++)
    {
        double ti = t[i];
        double zi1 = (ti - B1) / C1;
        double zi2 = (ti - B2) / C2;
        double ei1 = exp(-0.5 * gsl_pow_2(zi1));
        double ei2 = exp(-0.5 * gsl_pow_2(zi2));

        gsl_matrix_set(J, i, 0, -ei1);
        gsl_matrix_set(J, i, 1, -ei2);
        gsl_matrix_set(J, i, 2, -(A1 / C1) * ei1 * zi1);
        gsl_matrix_set(J, i, 3, -(A2 / C2) * ei2 * zi2);
        gsl_matrix_set(J, i, 4, -(A1 / C1) * ei1 * gsl_pow_2(zi1));
        gsl_matrix_set(J, i, 5, -(A2 / C2) * ei2 * gsl_pow_2(zi2));
    }

    return GSL_SUCCESS;
}
/*
int npfit(npfdata* data, gsl_vector* para, double &chisq) {
    const gsl_multifit_nlinear_type* T = gsl_multifit_nlinear_trust;
    gsl_multifit_nlinear_workspace* w;
    gsl_multifit_nlinear_fdf fdf;
    gsl_multifit_nlinear_parameters fdf_params = gsl_multifit_nlinear_default_parameters();
    size_t n = data->n;
    size_t p = 6;
    gsl_vector* f;
    int status, info;

    const double xtol = 1e-8;
    const double gtol = 1e-8;
    const double ftol = 1e-8;
    fdf.f = npf_f;
    fdf.df = npf_df;   
    fdf.fvv = NULL;     
    fdf.n = n;
    fdf.p = p;
    fdf.params = data;
    
    w = gsl_multifit_nlinear_alloc(T, &fdf_params, n, p);
   
    gsl_multifit_nlinear_init(para, &fdf, w);
    
    status = gsl_multifit_nlinear_driver(2000, xtol, gtol, ftol, NULL, NULL, &info, w);
    
    f = gsl_multifit_nlinear_residual(w);
    gsl_blas_ddot(f, f, &chisq);
    gsl_vector* x = gsl_multifit_nlinear_position(w);
    for (int i = 0; i < 6; i++) 
        gsl_vector_set(para, i, gsl_vector_get(x, i));
    gsl_multifit_nlinear_free(w);
    return status;
}
*/