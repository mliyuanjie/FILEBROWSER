#include <QtWidgets/QFileDialog>
#include <QtCore/qdebug.h>
#include <QtCore/qfileinfo.h>
#include <QtWidgets/QDockWidget>
#include <ftreeview.h>

FTreeView::FTreeView(QWidget* parent) :
	QTreeView(parent) {
	model = new QFileSystemModel();
	QTreeView::setModel(model);
} 

void FTreeView::open() {
	QString fn = QFileDialog::getExistingDirectory(this);
	QTreeView::setRootIndex(model->setRootPath(fn));
	emit rootpath(fn);
}

void FTreeView::mouseDoubleClickEvent(QMouseEvent* event) {
	emit doubleClicked();
	QModelIndex index = currentIndex();
	QFileInfo fileinfo = QFileInfo(model->filePath(index));
	QDockWidget* dockWidget = this->parent()->parent()->parent()->findChild<QDockWidget*>("fileWidget");
	delete dockWidget->findChild<QWidget*>("dockWidgetContents");
	//qDebug()<<fileinfo.fileName();
	if (fileinfo.suffix() == QString("abf") || fileinfo.suffix() == QString("dat")) {
		Ui::ABFDockWidget ui;
		ui.setupUi(dockWidget);
		dockWidget->findChild<QWidget*>("dockWidgetContents")->findChild<AChartView*>("graphicsView")->open(fileinfo.filePath());
	}
	else {
		QFile file(fileinfo.filePath());
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return;
		Ui::DockWidget ui;
		ui.setupUi(dockWidget);
		QString str;
		while (!file.atEnd())
		{
			QByteArray line = file.readLine();
			str.append(QString(line));
		}
		dockWidget->findChild<QWidget*>("dockWidgetContents")->findChild<QTextBrowser*>("textBrowser")->setText(str);
	}
	
	return;
}