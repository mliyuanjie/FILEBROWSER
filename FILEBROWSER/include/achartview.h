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
    void save();
    void setmode(bool);
    void clear();
    
signals:
    void startchanged(QString s);
    void endchanged(QString s);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);


private:
    QtCharts::QLineSeries* series;
    QtCharts::QChart* charts;
    QtCharts::QValueAxis* axisx;
    QtCharts::QValueAxis* axisy;
    QPair<float, float> xrange;
    QPair<float, float> yrange;
    int channel = 0;
    int sweep = 1;
    QString unit;
    QString filename;
    std::vector<float> data;
    float interval;
    ABF* abf = NULL;
    std::vector<QPair<float, float>> stx;
    std::vector<QPair<float, float>> sty;
    bool mode = false;
    
    void update();
    
};

#endif //ACHARTVIEW_H 