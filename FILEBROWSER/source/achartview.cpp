#include "achartview.h"
#include <QtGui/QMouseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QLineEdit>
#include <QtCore/QtMath>
#include <QtCore/qdebug.h>
#include <QtCore/qthread.h>
#include <QtCore/qmetatype.h>


AChartView::AChartView(QWidget* parent) :
    QChartView(parent)
{
    axisx = new QValueAxis();
    axisy = new QValueAxis();
    series = new QLineSeries();
    series_f = new QLineSeries();
    series_s = new QScatterSeries();
    series->setPen(QPen(Qt::darkBlue, 2)); 
    series_f->setPen(QPen(Qt::green, 1));
    series_s->setMarkerSize(10);
    //series->setUseOpenGL(true);
    charts = new QtCharts::QChart();
    charts->addSeries(series);
    charts->addSeries(series_f);
    charts->addSeries(series_s);
    charts->setAxisX(axisx, series);
    charts->setAxisY(axisy, series);
    charts->setAxisX(axisx, series_f);
    charts->setAxisY(axisy, series_f);
    charts->setAxisX(axisx, series_s);
    charts->setAxisY(axisy, series_s);
    charts->legend()->hide();
    setChart(charts);
    qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");
    //setRubberBand(QChartView::RectangleRubberBand);

}

AChartView::~AChartView() {
    thread->quit();
}

void AChartView::setchannel(QString s) {
    channel = s.toInt();
}

void AChartView::setsweep(QString s) {
    sweep = s.toInt();
}

void AChartView::mousePressEvent(QMouseEvent* event) {
    QPoint p = event->pos();
    QPointF pf = mapToScene(p);
    pf = charts->mapFromScene(pf);
    pf = charts->mapToValue(pf);
    if (mode) {
        start = pf.x();
        emit setstart(start);
    }
    else {
        if (pf.x() < stx.back().first || pf.y() > sty.back().second)
            return;
    }
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(p, QSize()).normalized());
    rubberBand->show();
}

void AChartView::mouseMoveEvent(QMouseEvent* event) {
    if (rubberBand == NULL)
        return;
    rubberBand->setGeometry(QRect(rubberBand->pos(), event->pos()).normalized());
}

void AChartView::mouseReleaseEvent(QMouseEvent* event) {
    QPoint p = event->pos();
    QPointF pf = mapToScene(p);
    pf = charts->mapFromScene(pf);
    pf = charts->mapToValue(pf);
    if (mode) {
        end = pf.x();
        emit setend(end);
    }
    else {
        if (pf.x() < stx.back().second && pf.y() > sty.back().first) {
            QPair<float, float> x;
            QPair<float, float> y;
            x.second = pf.x();
            y.first = pf.y();
            pf = mapToScene(rubberBand->pos());
            pf = charts->mapFromScene(pf);
            pf = charts->mapToValue(pf);
            x.first = pf.x();
            y.second = pf.y();
            stx.push_back(x);
            sty.push_back(y);
            emit getdata(stx.back().first, stx.back().second);
        }
    }
    delete rubberBand;
    rubberBand = NULL;
}

void AChartView::back() {
    if (stx.size() <= 1)
        return;
    stx.pop_back();
    sty.pop_back();
    getdata(stx.back().first, stx.back().second);
}

void AChartView::changex1() {
    float range = stx.back().second - stx.back().first;
    stx.back().first = (stx.back().first - 0.2 * range <= 0) ? 0 : stx.back().first - 0.2 * range;
    stx.back().second = (stx.back().second + 0.2 * range >= stx.front().second) ? stx.front().second : stx.back().second + 0.2 * range;
    emit getdata(stx.back().first, stx.back().second);
}

void AChartView::changex2() {
    float range = stx.back().second - stx.back().first;
    stx.back().first = stx.back().first + 0.2 * range;
    stx.back().second = stx.back().second - 0.2 * range;
    emit getdata(stx.back().first, stx.back().second);
}

void AChartView::changey1() {
    float range = sty.back().second - sty.back().first;
    sty.back().first = sty.back().first - 0.2 * range;
    sty.back().second = sty.back().second + 0.2 * range;
    emit getdata(sty.back().first, sty.back().second);
}

void AChartView::changey2() {
    float range = sty.back().second - sty.back().first;
    sty.back().first = sty.back().first + 0.2 * range;
    sty.back().second = sty.back().second - 0.2 * range;
    emit getdata(sty.back().first, sty.back().second);
}

void AChartView::additem() {
    QTableWidget* pt = this->parent()->findChild<QTabWidget*>("tabWidget")->findChild<QTableWidget*>("tableWidget");
    int index = pt->rowCount();
    pt->insertRow(index);
    pt->setItem(index, 0, new QTableWidgetItem(QString::number(start)));
    pt->setItem(index, 1, new QTableWidgetItem(QString::number(end)));
    index++;  
}

void AChartView::delitem() {
    QTableWidget* pt = this->parent()->findChild<QTabWidget*>("tabWidget")->findChild<QTableWidget*>("tableWidget");
    int index = pt->rowCount();
    pt->removeRow(index-1);
}

void AChartView::setmode(bool i) {
    mode = i;
}

void AChartView::update_d(QVector<QPointF> data) {
    axisy->setRange(sty.back().first, sty.back().second);
    axisx->setRange(stx.back().first, stx.back().second);
    series->replace(data);
    return;
}

void AChartView::update_f(QVector<QPointF> data) {
    series_f->replace(data);
    return;
}

void AChartView::update_s(QVector<QPointF> data) {
    series_s->replace(data);
    return;
}

void AChartView::initui(float x1, float x2, float y1, float y2) {
    axisx->setTitleText(QString("ms"));
    axisy->setTitleText(unit);
    stx.push_back(QPair<float, float>(x1, x2));
    sty.push_back(QPair<float, float>(y1, y2));
    axisx->setRange(x1, x2);
    axisy->setRange(y1, y2);
    emit getdata(x1, x2);

}

void AChartView::open(QString fn) {
    if (thread != NULL) {
        thread->quit();
        thread = NULL;
    }
    thread = new QThread;
    ABF* abf = new ABF(fn.toStdString());
    for (int i = 1; i < abf->Channel; i++)
        this->parent()->findChild<QComboBox*>("comboBox")->addItem(QString::number(i));
    for (int i = 2; i <= abf->Sweep; i++)
        this->parent()->findChild<QComboBox*>("comboBox_2")->addItem(QString::number(i));
    abf->moveToThread(thread);
    connect(thread, SIGNAL(finished()), abf, SLOT(deleteLater()));
    connect(this, SIGNAL(loaddata(int, int, bool)), abf, SLOT(readData(int, int, bool)));
    connect(this, SIGNAL(getdata(float, float)), abf, SLOT(draw(float, float)));
    connect(abf, SIGNAL(sendAxis(float, float, float, float)), this, SLOT(initui(float, float, float, float)));
    connect(abf, SIGNAL(sendData(QVector<QPointF>)), this, SLOT(update_d(QVector<QPointF>)));
    connect(this, SIGNAL(loadprocess(float, float)), abf, SLOT(readSignal(float, float)));
    connect(abf, SIGNAL(sendData_f(QVector<QPointF>)), this, SLOT(update_f(QVector<QPointF>)));
    connect(abf, SIGNAL(sendSig(QVector<QPointF>)), this, SLOT(update_s(QVector<QPointF>)));
    connect(this, SIGNAL(sendsave(QVector<QPointF>)), abf, SLOT(save(QVector<QPointF>)));
    thread->start();
    emit loaddata(0, 1, true);
}

void AChartView::save() {
    QTableWidget* pt = this->parent()->findChild<QTabWidget*>("tabWidget")->findChild<QTableWidget*>("tableWidget");
    QVector<QPointF> range;
    for (int i = 0; i < pt->rowCount(); i++) {
        range.push_back(QPointF(pt->item(i, 0)->text().toFloat(), pt->item(i, 1)->text().toFloat()));
    }
    emit sendsave(range);
    pt->clear();
}

void AChartView::startprocess() {
    QLineEdit* pt_sigma = this->parent()->findChild<QTabWidget*>("tabWidget")->findChild<QWidget*>("tab_2")->findChild<QLineEdit*>("lineEdit");
    QLineEdit* pt_freq = this->parent()->findChild<QTabWidget*>("tabWidget")->findChild<QWidget*>("tab_2")->findChild<QLineEdit*>("lineEdit_2");
    emit  loadprocess(pt_sigma->text().toFloat(), pt_freq->text().toFloat());
}

void AChartView::home() {
    emit loaddata(channel, sweep, true);
}