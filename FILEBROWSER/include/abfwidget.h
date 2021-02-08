/********************************************************************************
** Form generated from reading UI file 'abfwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ABFWIDGET_H
#define ABFWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include "achartview.h"

QT_BEGIN_NAMESPACE

class Ui_ABFDockWidget
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    AChartView *graphicsView;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QRadioButton *radioButton;
    QTableWidget *tableWidget;
    QPushButton *pushButton_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_3;
    QPushButton *pushButton_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *label_3;
    QLabel *label_6;

    void setupUi(QDockWidget *ABFDockWidget)
    {
        if (ABFDockWidget->objectName().isEmpty())
            ABFDockWidget->setObjectName(QStringLiteral("ABFDockWidget"));
        ABFDockWidget->resize(825, 574);
        ABFDockWidget->setCursor(QCursor(Qt::ArrowCursor));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        graphicsView = new AChartView(dockWidgetContents);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CrossCursor)));

        gridLayout->addWidget(graphicsView, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(dockWidgetContents);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(dockWidgetContents);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        comboBox = new QComboBox(dockWidgetContents);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout->addWidget(comboBox);

        comboBox_2 = new QComboBox(dockWidgetContents);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));

        horizontalLayout->addWidget(comboBox_2);

        radioButton = new QRadioButton(dockWidgetContents);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        horizontalLayout->addWidget(radioButton);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        tableWidget = new QTableWidget(dockWidgetContents);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setMinimumSize(QSize(150, 0));
        tableWidget->setMaximumSize(QSize(150, 16777215));
        tableWidget->setLineWidth(1);
        tableWidget->setMidLineWidth(0);
        tableWidget->setGridStyle(Qt::SolidLine);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(2);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->horizontalHeader()->setDefaultSectionSize(75);
        tableWidget->horizontalHeader()->setHighlightSections(true);
        tableWidget->horizontalHeader()->setMinimumSectionSize(10);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setDefaultSectionSize(30);

        gridLayout->addWidget(tableWidget, 1, 1, 1, 1);

        pushButton_4 = new QPushButton(dockWidgetContents);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setMinimumSize(QSize(150, 0));
        pushButton_4->setMaximumSize(QSize(150, 16777215));

        gridLayout->addWidget(pushButton_4, 2, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButton_3 = new QPushButton(dockWidgetContents);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(60, 0));
        pushButton_3->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_3->addWidget(pushButton_3);

        pushButton_5 = new QPushButton(dockWidgetContents);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setMinimumSize(QSize(60, 0));
        pushButton_5->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_3->addWidget(pushButton_5);


        gridLayout->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(50, 0));
        label->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(label);

        label_4 = new QLabel(dockWidgetContents);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(50, 0));
        label_4->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(label_4);

        label_2 = new QLabel(dockWidgetContents);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(50, 0));
        label_2->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(label_2);

        label_5 = new QLabel(dockWidgetContents);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setMinimumSize(QSize(50, 0));
        label_5->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(label_5);

        label_3 = new QLabel(dockWidgetContents);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(50, 0));
        label_3->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(label_3);

        label_6 = new QLabel(dockWidgetContents);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMinimumSize(QSize(50, 0));
        label_6->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(label_6);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        ABFDockWidget->setWidget(dockWidgetContents);

        retranslateUi(ABFDockWidget);
        QObject::connect(pushButton, SIGNAL(clicked()), graphicsView, SLOT(back()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), graphicsView, SLOT(plot()));
        QObject::connect(comboBox, SIGNAL(currentTextChanged(QString)), graphicsView, SLOT(setchannel(QString)));
        QObject::connect(comboBox_2, SIGNAL(currentTextChanged(QString)), graphicsView, SLOT(setsweep(QString)));
        QObject::connect(radioButton, SIGNAL(toggled(bool)), graphicsView, SLOT(setmode(bool)));
        QObject::connect(graphicsView, SIGNAL(setstart(double)), label_4, SLOT(setNum(double)));
        QObject::connect(graphicsView, SIGNAL(setend(double)), label_5, SLOT(setNum(double)));
        QObject::connect(pushButton_3, SIGNAL(clicked()), graphicsView, SLOT(additem()));
        QObject::connect(pushButton_5, SIGNAL(clicked()), graphicsView, SLOT(delitem()));

        QMetaObject::connectSlotsByName(ABFDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *ABFDockWidget)
    {
        ABFDockWidget->setWindowTitle(QApplication::translate("ABFDockWidget", "ABF", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ABFDockWidget", "Back", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("ABFDockWidget", "Home", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("ABFDockWidget", "0", Q_NULLPTR)
        );
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("ABFDockWidget", "1", Q_NULLPTR)
        );
        radioButton->setText(QApplication::translate("ABFDockWidget", "Select Data Mode", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("ABFDockWidget", "Save", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("ABFDockWidget", "add", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("ABFDockWidget", "del", Q_NULLPTR));
        label->setText(QApplication::translate("ABFDockWidget", "start time:", Q_NULLPTR));
        label_4->setText(QString());
        label_2->setText(QApplication::translate("ABFDockWidget", "end time:", Q_NULLPTR));
        label_5->setText(QString());
        label_3->setText(QApplication::translate("ABFDockWidget", "voltage:", Q_NULLPTR));
        label_6->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ABFDockWidget: public Ui_ABFDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ABFWIDGET_H
