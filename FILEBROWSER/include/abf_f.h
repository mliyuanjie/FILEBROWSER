#pragma once
#include <gsl/gsl_vector.h> 
#include <QtCore/qobject.h>

class ABF_F : public QObject {
public:
	ABF_F(std::vector<float> &data);


};