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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "achartview.h"

QT_BEGIN_NAMESPACE

class Ui_ABFDockWidget
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_8;
    QPushButton *pushButton_7;
    AChartView *graphicsView;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QRadioButton *radioButton;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_3;
    QPushButton *pushButton_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *label_4;
    QPushButton *pushButton_9;
    QLabel *label_2;
    QLabel *label_5;
    QPushButton *pushButton_10;
    QLabel *label_3;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_4;
    QPushButton *pushButton_6;

    void setupUi(QDockWidget *ABFDockWidget)
    {
        if (ABFDockWidget->objectName().isEmpty())
            ABFDockWidget->setObjectName(QStringLiteral("ABFDockWidget"));
        ABFDockWidget->resize(1118, 573);
        ABFDockWidget->setCursor(QCursor(Qt::ArrowCursor));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_8 = new QPushButton(dockWidgetContents);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setMinimumSize(QSize(20, 20));
        pushButton_8->setMaximumSize(QSize(20, 20));

        verticalLayout->addWidget(pushButton_8);

        pushButton_7 = new QPushButton(dockWidgetContents);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_7->sizePolicy().hasHeightForWidth());
        pushButton_7->setSizePolicy(sizePolicy);
        pushButton_7->setMinimumSize(QSize(20, 20));
        pushButton_7->setMaximumSize(QSize(20, 20));

        verticalLayout->addWidget(pushButton_7);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 2);

        graphicsView = new AChartView(dockWidgetContents);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CrossCursor)));

        gridLayout->addWidget(graphicsView, 1, 5, 1, 1);

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


        gridLayout->addLayout(horizontalLayout, 0, 5, 1, 1);

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

        gridLayout->addWidget(tableWidget, 1, 6, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButton_3 = new QPushButton(dockWidgetContents);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(70, 0));
        pushButton_3->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_3->addWidget(pushButton_3);

        pushButton_5 = new QPushButton(dockWidgetContents);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setMinimumSize(QSize(70, 0));
        pushButton_5->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_3->addWidget(pushButton_5);


        gridLayout->addLayout(horizontalLayout_3, 0, 6, 1, 1);

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

        pushButton_9 = new QPushButton(dockWidgetContents);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setMinimumSize(QSize(20, 20));
        pushButton_9->setMaximumSize(QSize(20, 20));

        horizontalLayout_2->addWidget(pushButton_9);

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

        pushButton_10 = new QPushButton(dockWidgetContents);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setMinimumSize(QSize(20, 20));
        pushButton_10->setMaximumSize(QSize(20, 20));

        horizontalLayout_2->addWidget(pushButton_10);

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


        gridLayout->addLayout(horizontalLayout_2, 2, 5, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushButton_4 = new QPushButton(dockWidgetContents);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setMinimumSize(QSize(70, 0));
        pushButton_4->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_4->addWidget(pushButton_4);

        pushButton_6 = new QPushButton(dockWidgetContents);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setMinimumSize(QSize(70, 0));
        pushButton_6->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_4->addWidget(pushButton_6);


        gridLayout->addLayout(horizontalLayout_4, 2, 6, 1, 1);

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
        QObject::connect(pushButton_9, SIGNAL(clicked()), graphicsView, SLOT(changex1()));
        QObject::connect(pushButton_10, SIGNAL(clicked()), graphicsView, SLOT(changex2()));
        QObject::connect(pushButton_8, SIGNAL(clicked()), graphicsView, SLOT(changey1()));
        QObject::connect(pushButton_7, SIGNAL(clicked()), graphicsView, SLOT(changey2()));

        QMetaObject::connectSlotsByName(ABFDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *ABFDockWidget)
    {
        ABFDockWidget->setWindowTitle(QApplication::translate("ABFDockWidget", "ABF", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("ABFDockWidget", "+", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("ABFDockWidget", "-", Q_NULLPTR));
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
        pushButton_3->setText(QApplication::translate("ABFDockWidget", "add", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("ABFDockWidget", "del", Q_NULLPTR));
        label->setText(QApplication::translate("ABFDockWidget", "start time:", Q_NULLPTR));
        label_4->setText(QString());
        pushButton_9->setText(QApplication::translate("ABFDockWidget", "+", Q_NULLPTR));
        label_2->setText(QApplication::translate("ABFDockWidget", "end time:", Q_NULLPTR));
        label_5->setText(QString());
        pushButton_10->setText(QApplication::translate("ABFDockWidget", "-", Q_NULLPTR));
        label_3->setText(QApplication::translate("ABFDockWidget", "voltage:", Q_NULLPTR));
        label_6->setText(QString());
        pushButton_4->setText(QApplication::translate("ABFDockWidget", "show", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("ABFDockWidget", "save", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ABFDockWidget: public Ui_ABFDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ABFWIDGET_H
