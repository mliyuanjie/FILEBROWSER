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
	nps->load(filename.toStdString());
	nps->moveToThread(thread);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), nps, SLOT(deleteLater()));
	connect(this->findChild<QPushButton*>("pushButton_6"), SIGNAL(clicked()), nps, SLOT(pretrace()));
	connect(this->findChild<QPushButton*>("pushButton_8"), SIGNAL(clicked()), nps, SLOT(nexttrace()));
	connect(nps, SIGNAL(sendindex(const QString&)), this->findChild<QLineEdit*>("lineEdit_7"), SIGNAL(setText(const QString&)));
	connect(this->findChild<QLineEdit*>("lineEdit_7"), SIGNAL(textChanged(const QString&)), nps, SLOT(setIndex(const QString&)));
	connect(this->findChild<QLineEdit*>("lineEdit_8"), SIGNAL(textChanged(const QString&)), nps, SLOT(setBin(const QString&)));
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
	thread->start();
}