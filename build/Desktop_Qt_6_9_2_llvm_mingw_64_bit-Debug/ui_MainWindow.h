/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnSettings;
    QLabel *labOpenPort;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_4;
    QPushButton *btnConnect;
    QPushButton *btnDisconnect;
    QVBoxLayout *verticalLayout_3;
    QCustomPlot *grafica;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnGraphOptions;
    QPushButton *btnClear;
    QCheckBox *checkCursors;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btnExport;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(802, 372);
        MainWindow->setMinimumSize(QSize(773, 367));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        btnSettings = new QPushButton(centralwidget);
        btnSettings->setObjectName("btnSettings");
        btnSettings->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(btnSettings);


        verticalLayout->addLayout(horizontalLayout_2);

        labOpenPort = new QLabel(centralwidget);
        labOpenPort->setObjectName("labOpenPort");
        labOpenPort->setMinimumSize(QSize(181, 51));
        labOpenPort->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labOpenPort);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        btnConnect = new QPushButton(centralwidget);
        btnConnect->setObjectName("btnConnect");
        btnConnect->setStyleSheet(QString::fromUtf8("color: green; font-weight: bold;"));

        verticalLayout_4->addWidget(btnConnect);

        btnDisconnect = new QPushButton(centralwidget);
        btnDisconnect->setObjectName("btnDisconnect");
        btnDisconnect->setEnabled(false);
        btnDisconnect->setStyleSheet(QString::fromUtf8("color: red;"));

        verticalLayout_4->addWidget(btnDisconnect);


        horizontalLayout_4->addLayout(verticalLayout_4);


        verticalLayout_2->addLayout(horizontalLayout_4);


        horizontalLayout_5->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        grafica = new QCustomPlot(centralwidget);
        grafica->setObjectName("grafica");
        grafica->setMinimumSize(QSize(511, 301));

        verticalLayout_3->addWidget(grafica);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        btnGraphOptions = new QPushButton(centralwidget);
        btnGraphOptions->setObjectName("btnGraphOptions");

        horizontalLayout_3->addWidget(btnGraphOptions);

        btnClear = new QPushButton(centralwidget);
        btnClear->setObjectName("btnClear");

        horizontalLayout_3->addWidget(btnClear);

        checkCursors = new QCheckBox(centralwidget);
        checkCursors->setObjectName("checkCursors");

        horizontalLayout_3->addWidget(checkCursors);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        btnExport = new QPushButton(centralwidget);
        btnExport->setObjectName("btnExport");

        horizontalLayout_3->addWidget(btnExport);


        verticalLayout_3->addLayout(horizontalLayout_3);


        horizontalLayout_5->addLayout(verticalLayout_3);


        gridLayout->addLayout(horizontalLayout_5, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "SerialScope", nullptr));
        btnSettings->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        labOpenPort->setText(QCoreApplication::translate("MainWindow", "Not Connected", nullptr));
        btnConnect->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        btnDisconnect->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
        btnGraphOptions->setText(QCoreApplication::translate("MainWindow", "Graph Options", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        checkCursors->setText(QCoreApplication::translate("MainWindow", "Cursors", nullptr));
        btnExport->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
