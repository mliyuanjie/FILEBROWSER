#include "achartview.h"
#include <QtGui/QMouseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableWidget>
#include <QtCore/QtMath>
#include <QtCore/qdebug.h>


AChartView::AChartView(QWidget* parent) :
    QChartView(parent)
{
    axisx = new QtCharts::QValueAxis();
    axisy = new QtCharts::QValueAxis();
    series = new QtCharts::QLineSeries();
    series->setPen(QPen(Qt::darkBlue, 1)); 
    //series->setUseOpenGL(true);
    charts = new QtCharts::QChart();
    charts->addSeries(series);
    charts->setAxisX(axisx, series);
    charts->setAxisY(axisy, series);
    charts->legend()->hide();
    setChart(charts);
    //setRubberBand(QChartView::RectangleRubberBand);

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
        if (pf.x() * 1000 / interval < stx.back().first || pf.y() > sty.back().second)
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
        if (pf.x() * 1000 / interval < stx.back().second && pf.y() > sty.back().first) {
            QPair<int, int> x;
            QPair<float, float> y;
            x.second = pf.x() * 1000 / interval;
            y.first = pf.y();
            pf = mapToScene(rubberBand->pos());
            pf = charts->mapFromScene(pf);
            pf = charts->mapToValue(pf);
            x.first = pf.x() * 1000 / interval;
            y.second = pf.y();
            stx.push_back(x);
            sty.push_back(y);
            update();
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
    update();
}

void AChartView::changex1() {
    float range = stx.back().second - stx.back().first;
    stx.back().first = (stx.back().first - 0.2 * range <= 0) ? 0 : stx.back().first - 0.2 * range;
    stx.back().second = (stx.back().second + 0.2 * range >= abf->data.size()) ? abf->data.size() : stx.back().second + 0.2 * range;
    update();
}

void AChartView::changex2() {
    float range = stx.back().second - stx.back().first;
    stx.back().first = stx.back().first + 0.2 * range;
    stx.back().second = stx.back().second - 0.2 * range;
    update();
}

void AChartView::changey1() {
    float range = sty.back().second - sty.back().first;
    sty.back().first = sty.back().first - 0.2 * range;
    sty.back().second = sty.back().second + 0.2 * range;
    update();
}

void AChartView::changey2() {
    float range = sty.back().second - sty.back().first;
    sty.back().first = sty.back().first + 0.2 * range;
    sty.back().second = sty.back().second - 0.2 * range;
    update();
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

void AChartView::update() {
    axisy->setRange(sty.back().first, sty.back().second);
    axisx->setRange(stx.back().first * interval / 1000, stx.back().second * interval / 1000);
    qDebug() << sty.back().first;
    abf->draw(stx.back().first, stx.back().second);
}

void AChartView::plot() {
    axisx->setTitleText(QString("ms"));
    axisy->setTitleText(unit);
    if (abf == NULL) {
        return;
    }
    abf->readData(channel, sweep, true);
    std::pair<float, float> t = abf->getLimit();
    
    stx.clear();
    sty.clear();
    stx.push_back(QPair<int, int>(0, abf->data.size()));
    sty.push_back(QPair<float, float>(t.first - 100, t.second + 100));
    update();
}

void AChartView::open(QString fn) {
    if (abf != NULL) {
        delete abf;
    }
    abf = new ABF(fn.toStdString());
    for (int i = 1; i < abf->Channel; i++) 
        this->parent()->findChild<QComboBox*>("comboBox")->addItem(QString::number(i));
    for (int i = 2; i <= abf->Sweep; i++) 
        this->parent()->findChild<QComboBox*>("comboBox_2")->addItem(QString::number(i));
    interval = abf->Interval;
    abf->setSeries(series);
    plot(); 
    
}

void AChartView::save() {
    QTableWidget* pt = this->parent()->findChild<QTabWidget*>("tabWidget")->findChild<QTableWidget*>("tableWidget");
    std::vector<unsigned int> start;
    std::vector<unsigned int> end;
    for (int i = 0; i < pt->rowCount(); i++) {
        start.push_back(pt->item(i, 0)->text().toFloat() * 1000 / interval);
        end.push_back(pt->item(i, 1)->text().toFloat() * 1000 / interval);
    }
    abf->save(start, end);
    pt->clear();
}
