#pragma once
#include <QtCore/qobject.h>
#include <QtCharts/qlineseries.h>
#include <QtCharts/qscatterseries.h>
#include "abf.h"

QT_CHARTS_USE_NAMESPACE
class ABF_F : public QObject {
	Q_OBJECT
public:
	ABF_F(ABF* a, QLineSeries* s1, QScatterSeries* s2, float s, float f);
private:
	std::vector<float> data;
	ABF* input;
	std::vector<std::pair<int, int>> sig;
	QLineSeries* lineseries;
	QScatterSeries* scatterseries;
	float sigma;
	float freq;

public slots:
	void draw(int s, int e);
	void process();
};