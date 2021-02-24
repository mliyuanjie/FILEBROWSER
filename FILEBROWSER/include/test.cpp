
#include <QtWidgets/qapplication.h>
#include <QtCharts/qscatterseries.h>
#include <QtWidgets/QMainWindow> 
#include "abf.h"
#include "algorithm.h"
#include <QtCharts/QChartView>
#include <QtWidgets/qboxlayout.h>
//#include <QtCharts/QChart>


QT_CHARTS_USE_NAMESPACE

void show(std::vector<float>& data, std::vector<float>& filter, std::vector<std::pair<int, int>>& sig, QMainWindow& window) {
	int s = 0;
	int e = data.size();
	int n = e - s;
	int skip = (n / 2000 == 0) ? 1 : n / 2000;
	QList<QPointF> interVariables;
	QList<QPointF> interFilter;
	int i = s;
	for (; i < e; i += skip) {
		int j = i;
		int max = j;
		int min = j;
		for (; j < i + skip && j < e; j++) {
			max = (data[max] > data[j]) ? max : j;
			min = (data[min] < data[j]) ? min : j;
		}
		interVariables.append(QPointF(min * 10 / 1000, data[min]));
		interVariables.append(QPointF(max * 10 / 1000, data[max]));
	}
	QLineSeries* series = new QLineSeries();
	series->append(interVariables);
	QChart* charts = new QChart();
	charts->legend()->hide();
	charts->addSeries(series);
	if (filter.size() != 0) {
		QList<QPointF> interFilter;
		int i = s;
		for (; i < e; i += skip) {
			int j = i;
			int max = j;
			int min = j;
			for (; j < i + skip && j < e; j++) {
				max = (filter[max] > filter[j]) ? max : j;
				min = (filter[min] < filter[j]) ? min : j;
			}
			interFilter.append(QPointF(min * 10 / 1000, filter[min]));
			interFilter.append(QPointF(max * 10 / 1000, filter[max]));
		}
		QLineSeries* series = new QLineSeries();
		series->append(interFilter);
		charts->addSeries(series);
	}
	if (sig.size() != 0) {
		QList<QPointF> interFilter;
		QScatterSeries* series = new QScatterSeries();
		for (int i = 0; i < sig.size(); i++) {
			series->append(QPointF(sig[i].first * 10 / 1000, data[sig[i].first]));
			series->append(QPointF(sig[i].second * 10 / 1000, data[sig[i].second]));
		}
		charts->addSeries(series);
	}
	charts->legend()->hide();
	charts->createDefaultAxes();
	QChartView* chartView = new QChartView(charts);
	chartView->setRubberBand(QChartView::HorizontalRubberBand);
	chartView->setRenderHint(QPainter::Antialiasing);
	window.setCentralWidget(chartView);
	return;
}

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	QMainWindow window;
	window.resize(500, 500);
	window.show();
	ABF* abf = new ABF("U:/test/test8.dat");
	abf->readData();
	std::vector<float> filter = meanSmooth(abf->data, 10);
	std::vector<std::pair<int, int>> sig = findPeak(filter, 2000, 500);
	show(abf->data, filter, sig, window);
	
	return app.exec();
}