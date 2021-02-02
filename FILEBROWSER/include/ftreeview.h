#ifndef FTREEVIEW_H
#define FTREEVIEW_H

#include<QtWidgets/qtreeview.h>
#include<QtWidgets/qfilesystemmodel.h>
#include<QtGui/QMouseEvent>
#include<QtGui/qstandarditemmodel.h>
#include "abfwidget.h" 

class FTreeView : public QTreeView {
	Q_OBJECT;
public:
	FTreeView(QWidget* parent);
public slots:
	void open();

signals:
	void rootpath(QString fn);
	void showabf(QString fn);

private:
	QFileSystemModel* model;

protected:
	void mouseDoubleClickEvent(QMouseEvent* event);
};
#endif // !FTREEVIEW_H
