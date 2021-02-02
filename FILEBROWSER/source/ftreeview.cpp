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
	QModelIndex index = currentIndex();
	QFileInfo fileinfo = QFileInfo(model->filePath(index));
	qDebug()<<fileinfo.fileName();
	if (fileinfo.suffix() == QString("abf")) {
		QDockWidget* dockWidget = this->parent()->parent()->parent()->findChild<QDockWidget*>("fileWidget");
		if (dockWidget->windowTitle() != "ABF") {
			Ui::ABFDockWidget ui;
			ui.setupUi(dockWidget);
		}
		dockWidget->findChild<QWidget*>("dockWidgetContents")->findChild<AChartView*>("graphicsView")->open(fileinfo.filePath());
		
	}
	return;
}