#include "algorithm.h"

struct npfdata {
    size_t n;
    double* t;
    double* y;
};

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

int npf_f(gsl_vector* x, npfdata data, gsl_vector* f) {
    size_t n = data.n;
    double* t = data.t;
    double* y = data.y;

    double A1 = gsl_vector_get(x, 0);
    double A2 = gsl_vector_get(x, 1);
    double B1 = gsl_vector_get(x, 2);
    double B2 = gsl_vector_get(x, 3);
    double C1 = gsl_vector_get(x, 4);
    double C2 = gsl_vector_get(x, 5);

    for (size_t i = 0; i < n; i++)
    {
        double Y1 = A1  * exp(-1 * gsl_pow_2(t[i] - B1) / (2 * gsl_pow_2(C1)));
        double Y2 = A2  * exp(-1 * gsl_pow_2(t[i] - B2) / (2 * gsl_pow_2(C2)));
        gsl_vector_set(f, i, y[i] - Y1 - Y2);
    }

    return GSL_SUCCESS;
}

int npf_df(gsl_vector* x, npfdata data, gsl_matrix* J) {
    size_t n = data.n;
    double* t = data.t;

    double A1 = gsl_vector_get(x, 0);
    double A2 = gsl_vector_get(x, 1);
    double B1 = gsl_vector_get(x, 2);
    double B2 = gsl_vector_get(x, 3);
    double C1 = gsl_vector_get(x, 4);
    double C2 = gsl_vector_get(x, 5);

    for (size_t i = 0; i < n; i++)
    {
        double ti = t[i];
        double zi1 = (ti - B1) / gsl_pow_2(C1);
        double zi2 = (ti - B2) / gsl_pow_2(C2);
        double ei1 = exp(-0.5 * gsl_pow_2(zi1));
        double ei2 = exp(-0.5 * gsl_pow_2(zi2));

        gsl_matrix_set(J, i, 0, -ei1);
        gsl_matrix_set(J, i, 1, -ei2);
        gsl_matrix_set(J, i, 2, -A1 * ei1 * zi1);
        gsl_matrix_set(J, i, 3, -A2 * ei2 * zi2);
        gsl_matrix_set(J, i, 4, -A1 * C1 * ei1 * gsl_pow_2(zi1));
        gsl_matrix_set(J, i, 5, -A2 * C2 * ei2 * gsl_pow_2(zi2));
    }

    return GSL_SUCCESS;
}