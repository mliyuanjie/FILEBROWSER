#include "npswidget.h"
#include "npschartview.h"
#include "histchartview.h"
#include "nps.h"

NPSWidget::NPSWidget(QWidget* parent) :QWidget(parent) {
	thread = new QThread;
}

NPSWidget::~NPSWidget() {
	thread->quit();
	thread = NULL;
	deleteLater();
}

void NPSWidget::open(QString filename) {
	NPS* nps = new NPS();
	nps->moveToThread(thread);
	connect(this->findChild<QPushButton*>("pushButton"), SIGNAL(clicked()), nps, SLOT(back()));
	connect(this->findChild<QPushButton*>("pushButton_2"), SIGNAL(clicked()), nps, SLOT(home()));
	connect(this->findChild<QPushButton*>("pushButton_8"), SIGNAL(clicked()), nps, SLOT(pretrace()));
	connect(this->findChild<QPushButton*>("pushButton_6"), SIGNAL(clicked()), nps, SLOT(nexttrace()));
	connect(nps, SIGNAL(sendtrace(QVector<QPointF>)), this->findChild<NPSChartView*>("graphicsView"), SLOT(update_d(QVector<QPointF>)));
	connect(nps, SIGNAL(sendsig(QVector<QPointF>)), this->findChild<NPSChartView*>("graphicsView"), SLOT(update_f(QVector<QPointF>)));
	connect(nps, SIGNAL(sendcursig(QVector<QPointF>)), this->findChild<NPSChartView*>("graphicsView"), SLOT(update_p(QVector<QPointF>)));
	connect(nps, SIGNAL(sendaxis(float, float, float, float)), this->findChild<NPSChartView*>("graphicsView"), SLOT(initaxis(float, float, float, float)));
	connect(nps, SIGNAL(sendhistaxis(float, float, float, float)), this->findChild<HistChartView*>("graphicsView_2"), SLOT(initaxis(float, float, float, float)));
	connect(nps, SIGNAL(sendhist(QVector<QPointF>)), this->findChild<HistChartView*>("graphicsView_2"), SLOT(update(QVector<QPointF>)));
	nps->load(filename.toStdString());

}