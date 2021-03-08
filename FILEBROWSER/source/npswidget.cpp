#include "npswidget.h"

NPSWidget::NPSWidget(QWidget* parent) :QWidget(parent) {
	thread = new QThread;
}