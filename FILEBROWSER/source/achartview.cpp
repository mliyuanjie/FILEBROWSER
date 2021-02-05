#include "achartview.h"
#include <QtGui/QMouseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/QComboBox>
#include <QtCore/QtMath>


AChartView::AChartView(QWidget* parent) :
    QChartView(parent)
{
    axisx = new QtCharts::QValueAxis();
    axisy = new QtCharts::QValueAxis();
    series = new QtCharts::QLineSeries();

    charts = new QtCharts::QChart();
    charts->addSeries(series);
    charts->setAxisX(axisx, series);
    charts->setAxisY(axisy, series);
    charts->setAnimationOptions(QChart::SeriesAnimations);
    setChart(charts);

    setRenderHint(QPainter::Antialiasing);
    setRubberBand(QChartView::RectangleRubberBand);

}

AChartView::~AChartView() {
    delete abf;
}

void AChartView::setchannel(QString s) {
    channel = s.toInt();
}

void AChartView::setsweep(QString s) {
    sweep = s.toInt();
}

void AChartView::mousePressEvent(QMouseEvent* event)
{
    QPoint p = event->pos();
    QPointF pf = mapToScene(p);
    pf = charts->mapFromScene(pf);
    pf = charts->mapToValue(pf);
    if (mode) {
        
    }
    else {
        xrange.first = pf.x();
        yrange.second = pf.y();
    }
    
    QChartView::mousePressEvent(event);
}

void AChartView::mouseMoveEvent(QMouseEvent* event)
{
    QChartView::mouseMoveEvent(event);
}

void AChartView::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint p = event->pos();
    QPointF pf = mapToScene(p);
    pf = charts->mapFromScene(pf);
    pf = charts->mapToValue(pf);
    xrange.second = pf.x();
    yrange.first = pf.y();
    QChartView::mouseReleaseEvent(event);
    stx.push_back(xrange);
    sty.push_back(yrange);
    update();
}

void AChartView::back() {
    if (stx.size() == 1)
        return;
    stx.pop_back();
    sty.pop_back();
    update();
}

void AChartView::changex1() {
    if (xrange.first == 0 || xrange.second == data.size() * interval / 1000)
        return;
    xrange.first = xrange.first - 0.1 * (xrange.second - xrange.first);
    xrange.second = xrange.first + 0.1 * (xrange.second - xrange.first);
    stx.push_back(xrange);
    sty.push_back(yrange);
    update();
}

void AChartView::changex2() {
    xrange.first = xrange.first + 0.1 * (xrange.second - xrange.first);
    xrange.second = xrange.first - 0.1 * (xrange.second - xrange.first);
    stx.push_back(xrange);
    sty.push_back(yrange);
    update();
}

void AChartView::changey1() {
    yrange.first = yrange.first - 0.1 * (yrange.second - yrange.first);
    yrange.second = yrange.second + 0.1 * (yrange.second - yrange.first);
    stx.push_back(xrange);
    sty.push_back(yrange);
    update();
}

void AChartView::changey2() {
    yrange.first = yrange.first + 0.1 * (yrange.second - yrange.first);
    xrange.second = xrange.second - 0.1 * (yrange.second - yrange.first);
    stx.push_back(xrange);
    sty.push_back(yrange);
    update();
}

void AChartView::update() {
    axisy->setRange(sty.back().first, sty.back().second);
    axisx->setRange(stx.back().first, stx.back().second);
    int n = (stx.back().second - stx.back().first) * 1000 / interval;
    int skip = (n / 2048 == 0) ? 1 : n / 1024;
    QVector<QPointF> interVariables;
    clock_t t1 = clock();
    for (int i = 0; i < n; i += skip) {
        QPointF p(i * interval / 1000 + stx.back().first, data[i + stx.back().first * 1000 / interval]);
        interVariables.append(p);
    }

    series->replace(interVariables);
    clock_t t2 = clock() - t1;
}

void AChartView::plot() {
    //axisx->setTitleText(QString("ms"));
    //axisy->setTitleText(unit);
    if (abf == NULL) {
        return;
    }
    data = abf->data(channel, sweep, true);
    xrange.first = 0;
    xrange.second = data.size() * interval / 1000;
    float min = 0;
    float max = data[0];
    for (int i = 1; i < data.size(); i++) {
        if (data[i] < min) {
            min = data[i];
        }
        else if (data[i] > max) {
            max = data[i];
        }
    }
    yrange.first = min - 10;
    yrange.second = max + 10;
    stx.clear();
    sty.clear();
    stx.push_back(xrange);
    sty.push_back(yrange);
    update();
}

void AChartView::open(QString fn) {
    if (abf != NULL) {
        delete abf;
    }
    abf = new ABF(fn.toStdString());
    for (int i = 1; i < abf->Channel; i++) {
        this->parent()->findChild<QComboBox*>("comboBox")->addItem(QString::number(i));
    }
    for (int i = 2; i <= abf->Sweep; i++) {
        this->parent()->findChild<QComboBox*>("comboBox_2")->addItem(QString::number(i));
    }
    interval = abf->Interval;
    data = abf->data(channel, sweep, true);
    xrange.first = 0;
    xrange.second = data.size() * interval / 1000;
    float min = 0;
    float max = data[0];
    for (int i = 1; i < data.size(); i++) {
        if (data[i] < min) {
            min = data[i];
        }
        else if (data[i] > max) {
            max = data[i];
        }
    }
    yrange.first = min - 10;
    yrange.second = max + 10;
    stx.push_back(xrange);
    sty.push_back(yrange);
    update();


}
