#include "histchartview.h" 


HistChartView::HistChartView(NPSWidget* parent) :
    QChartView(parent) {
    axisx = new QValueAxis();
    axisx->setGridLineVisible(false);
    axisy = new QValueAxis();
    axisy->setGridLineVisible(false);
    series = new QLineSeries();
    series->setPen(QPen(Qt::black, 3));
    charts = new QChart();
    charts->addSeries(series);
    charts->setAxisX(axisx, series);
    charts->setAxisY(axisy, series);
    charts->legend()->hide();
    setChart(charts);
}

void HistChartView::initaxis(float x1, float x2, float y1, float y2) {
    axisx->setRange(x1, x2);
    axisy->setRange(y1, y2);
}

void HistChartView::update(QVector<QPointF> data) {
    series->replace(data);
}