#include "abf_f.h" 

ABF_F::ABF_F(std::vector<float>* d, QLineSeries* s1, QScatterSeries* s2) {
	data = data;
	lineseries = s1;
	scatterseries = s2;
	input = gsl_vector_alloc(data->size());
	output = gsl_vector_alloc(data->size());
	return;
}