#include "algorithm.h"

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


std::vector<std::pair<int, int>> findPeak(double* x, double* b, size_t size, int t) {
    std::vector<std::pair<int, int>> out;
    float sd = gsl_stats_sd(x, 1, size);
    unsigned int j;
    bool flag = false;
    for (int i = 0; i < size; i++) {
        if (!flag && abs(b[i]) - abs(x[i]) > t * sd) {
            j = i;
            while (!(abs(x[j] - b[j]) < sd && j > 1 && x[j] >= x[j - 1] && x[j] >= x[j + 1])) 
                j--;               
            flag = true;
        }
        if (flag && abs(b[j] - b[i]) < 0.5 * sd && abs(b[i]) - abs(x[i]) < sd  && i < size - 1 && x[i] >= x[i - 1] && x[i] >= x[i + 1]) {
            flag = false;
            out.push_back(std::pair<int, int>(j, i));
        }
    }
    return out;
}
