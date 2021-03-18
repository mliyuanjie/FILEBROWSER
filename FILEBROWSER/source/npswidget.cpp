#include "npschartview.h"
#include "histchartview.h"
#include "npswidget.h"

NPSWidget::NPSWidget(QWidget* parent) :
	QWidget(parent) {
	qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");
	thread = new QThread();
}

NPSWidget::~NPSWidget() {
	thread->quit();
	thread = NULL;
}

void NPSWidget::open(QString filename) {
	NPS* nps = new NPS();
	nps->moveToThread(thread);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), nps, SLOT(deleteLater()));
	connect(this->findChild<QPushButton*>("pushButton_6"), SIGNAL(clicked()), nps, SLOT(nexttrace()));
	connect(this->findChild<QPushButton*>("pushButton_8"), SIGNAL(clicked()), nps, SLOT(pretrace()));
	connect(this->findChild<QPushButton*>("pushButton_3"), SIGNAL(clicked()), nps, SLOT(prehist()));
	connect(this->findChild<QPushButton*>("pushButton_4"), SIGNAL(clicked()), nps, SLOT(nexthist()));
	connect(nps, SIGNAL(sendindex(const QString&)), this->findChild<QLineEdit*>("lineEdit_7"), SLOT(setText(const QString&)));
	connect(this->findChild<HistChartView*>("graphicsView_2"), SIGNAL(indexedited(int)), nps, SLOT(setIndex(int)));
	connect(this->findChild<HistChartView*>("graphicsView_2"), SIGNAL(binedited(int)), nps, SLOT(setBin(int)));
	connect(nps, SIGNAL(sendtrace(QVector<QPointF>)), this->findChild<NPSChartView*>("graphicsView"), SLOT(update_d(QVector<QPointF>)));
	connect(nps, SIGNAL(sendsig(QVector<QPointF>)), this->findChild<NPSChartView*>("graphicsView"), SLOT(update_f(QVector<QPointF>)));
	connect(nps, SIGNAL(sendcursig(QVector<QPointF>)), this->findChild<NPSChartView*>("graphicsView"), SLOT(update_p(QVector<QPointF>)));
	connect(nps, SIGNAL(sendaxis(float, float, float, float)), this->findChild<NPSChartView*>("graphicsView"), SLOT(initaxis(float, float, float, float)));
	connect(nps, SIGNAL(sendhistaxis(float, float, float, float)), this->findChild<HistChartView*>("graphicsView_2"), SLOT(initaxis(float, float, float, float)));
	connect(nps, SIGNAL(sendhist(QVector<QPointF>)), this->findChild<HistChartView*>("graphicsView_2"), SLOT(update(QVector<QPointF>)));
	connect(nps, SIGNAL(sendtracenum(int)), this->findChild<QLabel*>("label_8"), SLOT(setNum(int)));
	connect(nps, SIGNAL(sendsignum(int)), this->findChild<QLabel*>("label_7"), SLOT(setNum(int)));
	connect(nps, SIGNAL(sendtracecur(int)), this->findChild<QLabel*>("label_6"), SLOT(setNum(int)));
	connect(this->findChild<NPSChartView*>("graphicsView"), SIGNAL(gettrace(float, float)), nps, SLOT(trace(float, float)));
	connect(this->findChild<NPSChartView*>("graphicsView"), SIGNAL(gethist(float)), nps, SLOT(sethist(float)));
	//connect(this->findChild<QPushButton*>("pushButton_5"), SIGNAL(clicked()), nps, SLOT(fit()));
	thread->start();
	nps->load(filename.toStdString());
}