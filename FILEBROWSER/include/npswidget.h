#ifndef NPSWIDGET_H
#define NPSWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/qpushbutton.h>
#include <QtCore/qthread.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlabel.h>
#include "nps.h"

QT_BEGIN_NAMESPACE

class NPSWidget : public QWidget {
public:
	NPSWidget(QWidget* parent);
	~NPSWidget();
	void open(QString);

private:
	QThread* thread;
};

#endif // !NPSWIDGET_H

