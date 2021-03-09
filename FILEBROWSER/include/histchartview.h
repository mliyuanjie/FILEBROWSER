#ifndef HISTCHARTVIEW_H
#define HISTCHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QtCharts/qsplineseries.h>
#include <QtCharts/qscatterseries.h>
#include <QtCharts/qvalueaxis.h>


QT_CHARTS_USE_NAMESPACE

class HistChartView : public QChartView
{
    Q_OBJECT;
public:
    HistChartView(QWidget* parent);
    ~HistChartView();

public slots:
    //void setFilename(QString s);
    void initaxis(float, float, float, float);
    void update(QVector<QPointF>);

signals:

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    QLineSeries* series_d;
    QLineSeries* series_f;
    QScatterSeries* series_p;
    QChart* charts;
    QRubberBand* rubberBand = NULL;
    std::vector<QPair<float, float>> stx;
    std::vector<QPair<float, float>> sty;
    double start = 0;
    double end = 0;
};
#endif // !HISTCHARTVIEW_H
