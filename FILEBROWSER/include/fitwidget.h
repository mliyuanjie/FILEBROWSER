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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
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
    QGroupBox *groupBox_3;
    QPushButton *pushButton_5;
    QLineEdit *lineEdit_7;
    QPushButton *pushButton;
    QGroupBox *groupBox;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *comboBox;
    QPushButton *pushButton_7;
    QToolButton *toolButton;
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
        NPSDockWidget->resize(1108, 592);
        NPSDockWidget->setMinimumSize(QSize(102, 311));
        NPSDockWidget->setMaximumSize(QSize(1200, 524287));
        NPSDockWidget->setCursor(QCursor(Qt::ArrowCursor));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        widget = new NPSWidget(dockWidgetContents);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, -10, 1111, 581));
        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(90, 10, 75, 23));
        graphicsView = new NPSChartView(widget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(10, 40, 1081, 201));
        groupBox_3 = new QGroupBox(widget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(300, 250, 261, 301));
        pushButton_5 = new QPushButton(widget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(1040, 10, 51, 23));
        lineEdit_7 = new QLineEdit(widget);
        lineEdit_7->setObjectName(QStringLiteral("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(10, 250, 51, 25));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 75, 23));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(890, 250, 201, 301));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(100, 30, 71, 25));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(100, 70, 71, 25));
        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(100, 110, 71, 25));
        lineEdit_4 = new QLineEdit(groupBox);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(100, 150, 71, 25));
        lineEdit_5 = new QLineEdit(groupBox);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(100, 190, 71, 25));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 81, 18));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 70, 81, 18));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 110, 81, 18));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 150, 81, 18));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 190, 81, 18));
        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(10, 230, 81, 31));
        pushButton_7 = new QPushButton(groupBox);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(100, 230, 71, 34));
        toolButton = new QToolButton(widget);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(980, 10, 52, 24));
        graphicsView_2 = new HistChartView(widget);
        graphicsView_2->setObjectName(QStringLiteral("graphicsView_2"));
        graphicsView_2->setGeometry(QRect(10, 280, 271, 271));
        label_7 = new QLabel(widget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(90, 240, 41, 41));
        label_8 = new QLabel(widget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(550, 10, 41, 31));
        label_6 = new QLabel(widget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(500, 10, 31, 31));
        lineEdit_8 = new QLineEdit(widget);
        lineEdit_8->setObjectName(QStringLiteral("lineEdit_8"));
        lineEdit_8->setGeometry(QRect(230, 250, 51, 25));
        pushButton_4 = new QPushButton(widget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(140, 550, 31, 23));
        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(110, 550, 31, 23));
        label_9 = new QLabel(widget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(190, 250, 81, 21));
        label_10 = new QLabel(widget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(530, 10, 21, 31));
        label_11 = new QLabel(widget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(80, 240, 21, 41));
        pushButton_6 = new QPushButton(widget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(640, 10, 31, 23));
        pushButton_8 = new QPushButton(widget);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(610, 10, 31, 23));
        NPSDockWidget->setWidget(dockWidgetContents);

        retranslateUi(NPSDockWidget);
        QObject::connect(pushButton, SIGNAL(clicked()), graphicsView, SLOT(back()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), graphicsView, SLOT(home()));

        QMetaObject::connectSlotsByName(NPSDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *NPSDockWidget)
    {
        NPSDockWidget->setWindowTitle(QApplication::translate("NPSDockWidget", "NPS", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("NPSDockWidget", "Home", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("NPSDockWidget", "Result", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("NPSDockWidget", "Save", Q_NULLPTR));
        pushButton->setText(QApplication::translate("NPSDockWidget", "Back", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("NPSDockWidget", "fit:", Q_NULLPTR));
        label->setText(QApplication::translate("NPSDockWidget", "Diameter:", Q_NULLPTR));
        label_2->setText(QApplication::translate("NPSDockWidget", "Diameter:", Q_NULLPTR));
        label_3->setText(QApplication::translate("NPSDockWidget", "Diameter:", Q_NULLPTR));
        label_4->setText(QApplication::translate("NPSDockWidget", "Diameter:", Q_NULLPTR));
        label_5->setText(QApplication::translate("NPSDockWidget", "Diameter:", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("NPSDockWidget", "Oblate", Q_NULLPTR)
         << QApplication::translate("NPSDockWidget", "Prolate", Q_NULLPTR)
        );
        pushButton_7->setText(QApplication::translate("NPSDockWidget", "fit", Q_NULLPTR));
        toolButton->setText(QApplication::translate("NPSDockWidget", "...", Q_NULLPTR));
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
