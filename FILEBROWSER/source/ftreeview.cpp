#include <QtWidgets/QFileDialog>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qmenu.h>
#include <QtCore/qdebug.h>
#include <QtCore/qfileinfo.h>
#include <QtWidgets/QDockWidget>
#include <ftreeview.h>

FTreeView::FTreeView(QWidget* parent) :
	QTreeView(parent) {
	model = new QFileSystemModel();
	model->setReadOnly(false);
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
	else if (fileinfo.suffix() == QString("nps")) {
		Ui::NPSDockWidget ui;
		ui.setupUi(dockWidget);
		dockWidget->findChild<QWidget*>("dockWidgetContents")->findChild<NPSWidget*>("widget")->open(fileinfo.filePath());

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

void FTreeView::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu* popMenu = new QMenu(this);
	QModelIndex index = currentIndex();
	QFileInfo fileinfo = QFileInfo(model->filePath(index));
	if (fileinfo.isDir())
		return;
	// qDebug()<<"right:"<<i;
	QAction* ren = new QAction(QObject::tr("Rename"), this);
	QAction* del = new QAction(QObject::tr("Delete"), this);
	popMenu->addAction(ren);
	popMenu->addAction(del);
	connect(ren, SIGNAL(triggered()), this, SLOT(renamefile()));
	connect(del, SIGNAL(triggered()), this, SLOT(deletefile()));
	popMenu->exec(QCursor::pos());
}

void FTreeView::renamefile() {
	QModelIndex index = currentIndex();
	edit(index);
}

void FTreeView::deletefile() {
	QMessageBox msgBox;
	msgBox.setText(model->filePath(currentIndex()));
	msgBox.setInformativeText("Are you sure to delete it?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	int ret = msgBox.exec();
	switch (ret) {
	case QMessageBox::Yes:
		model->remove(currentIndex());
	case QMessageBox::No:
		break;
	default:
		break;
	}
	return;
}