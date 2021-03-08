#ifndef NPSCHARTVIEW_H
#define NPSCHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QtCharts/qsplineseries.h>
#include <QtCharts/qscatterseries.h>
#include <QtCharts/qvalueaxis.h>


QT_CHARTS_USE_NAMESPACE

class NPSChartView : public QChartView
{
    Q_OBJECT;
public:
    NPSChartView(QWidget* parent);
    ~NPSChartView();

public slots:
    //void setFilename(QString s);
    void back();
    void home();
    void initaxis(float x1, float x2, float y1, float y2);
    void update_d(QVector<QPointF>);
    void update_f(QVector<QPointF>);
    void update_p(QPointF);

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
#endif // !NPSCHARTVIEW_H

