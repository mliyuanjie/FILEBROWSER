/********************************************************************************
** Form generated from reading UI file 'txtwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TXTWIDGET_H
#define TXTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DockWidget
{
public:
    QWidget *dockWidgetContents;
    QHBoxLayout *horizontalLayout;
    QTextBrowser *textBrowser;

    void setupUi(QDockWidget *DockWidget)
    {
        if (DockWidget->objectName().isEmpty())
            DockWidget->setObjectName(QStringLiteral("DockWidget"));
        DockWidget->resize(1290, 525);
        DockWidget->setCursor(QCursor(Qt::ArrowCursor));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        horizontalLayout = new QHBoxLayout(dockWidgetContents);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        textBrowser = new QTextBrowser(dockWidgetContents);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));

        horizontalLayout->addWidget(textBrowser);

        DockWidget->setWidget(dockWidgetContents);

        retranslateUi(DockWidget);

        QMetaObject::connectSlotsByName(DockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *DockWidget)
    {
        DockWidget->setWindowTitle(QApplication::translate("DockWidget", "TXTWidget", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DockWidget: public Ui_DockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TXTWIDGET_H
