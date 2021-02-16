#ifndef ACHARTVIEW_H
#define ACHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QtCharts/qsplineseries.h>
#include <QtCharts/qvalueaxis.h>
#include "abf.h"

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
    //void home();
    void setchannel(QString s);
    void setsweep(QString s);
    void plot();
    void setmode(bool);
    void additem();
    void delitem();
    void save();

signals:
    void setstart(double num);
    void setend(double num);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    QtCharts::QLineSeries* series;
    QtCharts::QChart* charts;
    QtCharts::QValueAxis* axisx;
    QtCharts::QValueAxis* axisy;
    QRubberBand* rubberBand = NULL;
    int channel = 0;
    int sweep = 1;
    QString unit;
    QString filename;
    float interval;
    ABF* abf = NULL;
    std::vector<QPair<int, int>> stx;
    std::vector<QPair<float, float>> sty;
    bool mode = false;
    double start = 0;
    double end = 0;
    void update();
};
#endif //ACHARTVIEW_H 