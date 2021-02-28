#ifndef ACHARTVIEW_H
#define ACHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QtCharts/qsplineseries.h>
#include <QtCharts/qscatterseries.h>
#include <QtCharts/qvalueaxis.h>
#include "abf.h"
#include "abf_f.h"

QT_CHARTS_USE_NAMESPACE

class AChartView : public QChartView
{
    Q_OBJECT;
public:
    AChartView(QWidget* parent);
    ~AChartView();

    void open(QString fn);

public slots:
    //void setFilename(QString s);
    void changex1();
    void changex2();
    void changey2();
    void changey1();
    void back();
    void home();
    void setchannel(QString s);
    void setsweep(QString s);
    void initui(float x1, float x2, float y1, float y2);
    void additem();
    void delitem();
    void save();
    void startprocess();
    void update_d(QVector<QPointF>);
    void update_f(QVector<QPointF>);
    void currentprocess(int);

signals:
    void setstart(double num);
    void setend(double num);
    void getdata(float s, float e);
    void loaddata(int c, int s, bool m);
    void loadprocess(float sigma, float freq, float thres);
    void sendsave(QVector<QPointF>);
    void setsignum(QString);
    void setprocess(int);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    QtCharts::QLineSeries* series;
    QtCharts::QLineSeries* series_f;
    QtCharts::QChart* charts;
    QtCharts::QValueAxis* axisx;
    QtCharts::QValueAxis* axisy;
    QRubberBand* rubberBand = NULL;
    int channel = 0;
    int sweep = 1;
    QThread* thread = NULL;
    std::vector<QPair<float, float>> stx;
    std::vector<QPair<float, float>> sty;
    double start = 0;
    double end = 0;
    
};
#endif //ACHARTVIEW_H 