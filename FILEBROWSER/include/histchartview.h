#ifndef HISTCHARTVIEW_H
#define HISTCHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QtCharts/qsplineseries.h>
#include <QtCharts/qscatterseries.h>
#include <QtCharts/qvalueaxis.h>
#include "npswidget.h"

QT_CHARTS_USE_NAMESPACE

class HistChartView : public QChartView
{
    Q_OBJECT;
public:
    HistChartView(NPSWidget* parent);

public slots:
    //void setFilename(QString s);
    void initaxis(float, float, float, float);
    void update(QVector<QPointF>);

private:
    QLineSeries* series;
    QChart* charts;
    QValueAxis* axisx;
    QValueAxis* axisy;
};
#endif // !HISTCHARTVIEW_H