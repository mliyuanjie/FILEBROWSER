/********************************************************************************
** Form generated from reading UI file 'fitwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef FITWIDGET_H
#define FITWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "histchartview.h"
#include "npschartview.h"
#include "npswidget.h"

QT_BEGIN_NAMESPACE

class Ui_NPSDockWidget
{
public:
    QWidget *dockWidgetContents;
    NPSWidget *widget;
    QPushButton *pushButton_2;
    NPSChartView *graphicsView;
    QLineEdit *lineEdit_7;
    QPushButton *pushButton;
    HistChartView *graphicsView_2;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_6;
    QLineEdit *lineEdit_8;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QPushButton *pushButton_6;
    QPushButton *pushButton_8;

    void setupUi(QDockWidget *NPSDockWidget)
    {
        if (NPSDockWidget->objectName().isEmpty())
            NPSDockWidget->setObjectName(QStringLiteral("NPSDockWidget"));
        NPSDockWidget->resize(1200, 543);
        NPSDockWidget->setMinimumSize(QSize(1200, 470));
        NPSDockWidget->setMaximumSize(QSize(1200, 524287));
        NPSDockWidget->setCursor(QCursor(Qt::ArrowCursor));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        widget = new NPSWidget(dockWidgetContents);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 0, 1201, 701));
        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(90, 0, 75, 23));
        graphicsView = new NPSChartView(widget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(10, 30, 761, 441));
        lineEdit_7 = new QLineEdit(widget);
        lineEdit_7->setObjectName(QStringLiteral("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(770, 0, 51, 25));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 0, 75, 23));
        graphicsView_2 = new HistChartView(widget);
        graphicsView_2->setObjectName(QStringLiteral("graphicsView_2"));
        graphicsView_2->setGeometry(QRect(770, 30, 411, 441));
        label_7 = new QLabel(widget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(860, 0, 41, 31));
        label_8 = new QLabel(widget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(220, 0, 41, 31));
        label_6 = new QLabel(widget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(180, 0, 31, 31));
        lineEdit_8 = new QLineEdit(widget);
        lineEdit_8->setObjectName(QStringLiteral("lineEdit_8"));
        lineEdit_8->setGeometry(QRect(1130, 0, 51, 25));
        pushButton_4 = new QPushButton(widget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(990, 470, 31, 23));
        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(960, 470, 31, 23));
        label_9 = new QLabel(widget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(1070, 0, 51, 21));
        label_10 = new QLabel(widget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(200, 0, 21, 31));
        label_11 = new QLabel(widget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(840, 0, 21, 31));
        pushButton_6 = new QPushButton(widget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(370, 470, 31, 23));
        pushButton_8 = new QPushButton(widget);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(340, 470, 31, 23));
        NPSDockWidget->setWidget(dockWidgetContents);

        retranslateUi(NPSDockWidget);
        QObject::connect(pushButton, SIGNAL(clicked()), graphicsView, SLOT(back()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), graphicsView, SLOT(home()));
        QObject::connect(lineEdit_7, SIGNAL(returnPressed()), graphicsView_2, SLOT(setindex()));
        QObject::connect(lineEdit_8, SIGNAL(returnPressed()), graphicsView_2, SLOT(setbin()));

        QMetaObject::connectSlotsByName(NPSDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *NPSDockWidget)
    {
        NPSDockWidget->setWindowTitle(QApplication::translate("NPSDockWidget", "NPS", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("NPSDockWidget", "Home", Q_NULLPTR));
        lineEdit_7->setText(QApplication::translate("NPSDockWidget", "0", Q_NULLPTR));
        pushButton->setText(QApplication::translate("NPSDockWidget", "Back", Q_NULLPTR));
        label_7->setText(QApplication::translate("NPSDockWidget", "0", Q_NULLPTR));
        label_8->setText(QApplication::translate("NPSDockWidget", "0", Q_NULLPTR));
        label_6->setText(QApplication::translate("NPSDockWidget", "0", Q_NULLPTR));
        lineEdit_8->setText(QApplication::translate("NPSDockWidget", "50", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("NPSDockWidget", ">>", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("NPSDockWidget", "<<", Q_NULLPTR));
        label_9->setText(QApplication::translate("NPSDockWidget", "bin:", Q_NULLPTR));
        label_10->setText(QApplication::translate("NPSDockWidget", "/", Q_NULLPTR));
        label_11->setText(QApplication::translate("NPSDockWidget", "/", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("NPSDockWidget", ">>", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("NPSDockWidget", "<<", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class NPSDockWidget: public Ui_NPSDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // FITWIDGET_H
