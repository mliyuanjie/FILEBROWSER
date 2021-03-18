#ifndef DATMODEL_H
#define DATMODEL_H

#include <iostream>
#include <fstream>
#include <vector>
#include <QtCore/qobject.h>
#include <QtCore/qvector.h>
#include <gsl/gsl_vector.h>
#include "datio.h"

struct NPSig {
	float start;
	float end;
	float current;
	float baseline;
	float Imax;
	float Imin;
	float volume;
	float shape;
};

struct Paras {
	float sigma;
	float window;
	float threshold;
};

class DATModel:public QObject {
	Q_OBJECT 
public slots:
	void load(std::string&);
	void signal(float sigma, float freq, float thres);
	void save(QVector<QPointF>);
	void draw(float xmin, float xmax);
	void savenps(float, float, float);
	void removenps();

signals:
	void sendData(QVector<QPointF>);
	void sendAxis(float xmin, float xmax, float ymin, float ymax);
	void sendData_f(QVector<QPointF>, float, float);
	void sendProcess(int);

private:
	DATIO dat;
	std::string fn;
	float interval;
	std::vector<NPSig> sigs;
	size_t n;
};



#endif //DATMODEL_H