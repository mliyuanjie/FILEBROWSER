#pragma once
#include <gsl/gsl_vector.h> 
#include <QtCore/qobject.h>
#include <QtCharts/qlineseries.h>
#include <QtCharts/qscatterseries.h>

QT_CHARTS_USE_NAMESPACE
class ABF_F : public QObject {
	Q_OBJECT
public:
	ABF_F(std::vector<float>* d, QLineSeries* s1, QScatterSeries* s2);
	~ABF_F();
private:
	gsl_vector* input;
	gsl_vector* output;
	std::vector<float>* data;
	std::vector<std::pair<int, int>> sig;
	QLineSeries* lineseries;
	QScatterSeries* scatterseries;

public slots:
	void draw(int s, int e);
	void findpeak(float sigma);
	void destory();


};