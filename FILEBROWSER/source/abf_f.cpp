#include "abf_f.h" 
#include "algorithm.h"
#include <QtCore/qdebug.h>

ABF_F::ABF_F(ABF* a, float s, float f) {
	input = a;
	sigma = s;
	freq = f;
	return;
}

void ABF_F::draw(int s, int e) {
	int n = e - s;
	int skip = (n / 3000 == 0) ? 1 : n / 3000;
	float interval = input->Interval;
	QVector<QPointF> interVariables;
	int i = s;
	for (; i < e; i += skip) {
		int j = i;
		int max = j;
		int min = j;
		for (; j < i + skip && j < e; j++) {
			max = (data[max] > data[j]) ? max : j;
			min = (data[min] < data[j]) ? min : j;
		}
		interVariables.append(QPointF(min * interval / 1000, data[min]));
		interVariables.append(QPointF(max * interval / 1000, data[max]));
	}
	emit update_f(interVariables);
	QVector<QPointF> interFilter;
	for (int i = 0; i < sig.size(); i++) {
		interFilter.append(QPointF(sig[i].first * interval / 1000, data[sig[i].first]));
		interFilter.append(QPointF(sig[i].second * interval / 1000, data[sig[i].second]));
	}
	emit update_s(interFilter);
	return;
}

void ABF_F::process() {
	data = meanSmooth(input->data, sigma);
	sig = findPeak(data, 2000, 100);
	draw(0, data.size());
	qDebug() << "done";
	return;
}
