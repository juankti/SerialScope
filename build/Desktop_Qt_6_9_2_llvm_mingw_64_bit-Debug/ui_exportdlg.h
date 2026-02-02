/********************************************************************************
** Form generated from reading UI file 'exportdlg.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTDLG_H
#define UI_EXPORTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_exportdlg
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *T;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_15;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboFormat;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *leFilePath;
    QPushButton *pushButton_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *leFileName;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QSlider *qualitySlider;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_13;
    QComboBox *comboDimensions;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QComboBox *comboRange;
    QSpacerItem *horizontalSpacer_4;
    QWidget *tab_2;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_6;
    QComboBox *comboFormat_2;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QLineEdit *leFilePath_2;
    QPushButton *pushButton_3;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_9;
    QLineEdit *leFileName_2;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_12;
    QComboBox *comboRange_2;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnExport;

    void setupUi(QDialog *exportdlg)
    {
        if (exportdlg->objectName().isEmpty())
            exportdlg->setObjectName("exportdlg");
        exportdlg->resize(347, 353);
        exportdlg->setMinimumSize(QSize(347, 353));
        exportdlg->setMaximumSize(QSize(347, 353));
        gridLayout = new QGridLayout(exportdlg);
        gridLayout->setObjectName("gridLayout");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        tabWidget = new QTabWidget(exportdlg);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setMinimumSize(QSize(321, 241));
        T = new QWidget();
        T->setObjectName("T");
        layoutWidget = new QWidget(T);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 12, 293, 231));
        horizontalLayout_15 = new QHBoxLayout(layoutWidget);
        horizontalLayout_15->setObjectName("horizontalLayout_15");
        horizontalLayout_15->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(layoutWidget);
        label->setObjectName("label");
        label->setMaximumSize(QSize(54, 30));

        horizontalLayout_2->addWidget(label);

        comboFormat = new QComboBox(layoutWidget);
        comboFormat->addItem(QString());
        comboFormat->addItem(QString());
        comboFormat->addItem(QString());
        comboFormat->setObjectName("comboFormat");
        comboFormat->setMaximumSize(QSize(91, 30));

        horizontalLayout_2->addWidget(comboFormat);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName("label_3");
        label_3->setMaximumSize(QSize(65, 32));

        horizontalLayout_5->addWidget(label_3);

        leFilePath = new QLineEdit(layoutWidget);
        leFilePath->setObjectName("leFilePath");
        leFilePath->setReadOnly(true);

        horizontalLayout_5->addWidget(leFilePath);

        pushButton_2 = new QPushButton(layoutWidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setMinimumSize(QSize(16, 32));
        pushButton_2->setMaximumSize(QSize(16, 32));

        horizontalLayout_5->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");
        label_2->setMaximumSize(QSize(73, 30));

        horizontalLayout_3->addWidget(label_2);

        leFileName = new QLineEdit(layoutWidget);
        leFileName->setObjectName("leFileName");
        leFileName->setMaximumSize(QSize(142, 30));

        horizontalLayout_3->addWidget(leFileName);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName("label_5");

        horizontalLayout_6->addWidget(label_5);

        qualitySlider = new QSlider(layoutWidget);
        qualitySlider->setObjectName("qualitySlider");
        qualitySlider->setOrientation(Qt::Orientation::Horizontal);

        horizontalLayout_6->addWidget(qualitySlider);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName("label_4");

        horizontalLayout_6->addWidget(label_4);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        label_13 = new QLabel(layoutWidget);
        label_13->setObjectName("label_13");

        horizontalLayout_9->addWidget(label_13);

        comboDimensions = new QComboBox(layoutWidget);
        comboDimensions->addItem(QString());
        comboDimensions->addItem(QString());
        comboDimensions->addItem(QString());
        comboDimensions->setObjectName("comboDimensions");
        comboDimensions->setMinimumSize(QSize(131, 30));
        comboDimensions->setMaximumSize(QSize(131, 30));

        horizontalLayout_9->addWidget(comboDimensions);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName("label_8");

        horizontalLayout_8->addWidget(label_8);

        comboRange = new QComboBox(layoutWidget);
        comboRange->addItem(QString());
        comboRange->addItem(QString());
        comboRange->setObjectName("comboRange");
        comboRange->setMinimumSize(QSize(131, 30));
        comboRange->setMaximumSize(QSize(131, 30));

        horizontalLayout_8->addWidget(comboRange);


        verticalLayout->addLayout(horizontalLayout_8);


        horizontalLayout_15->addLayout(verticalLayout);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_4);

        tabWidget->addTab(T, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        layoutWidget1 = new QWidget(tab_2);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(10, 10, 301, 231));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName("label_6");
        label_6->setMaximumSize(QSize(54, 30));

        horizontalLayout_4->addWidget(label_6);

        comboFormat_2 = new QComboBox(layoutWidget1);
        comboFormat_2->addItem(QString());
        comboFormat_2->addItem(QString());
        comboFormat_2->setObjectName("comboFormat_2");
        comboFormat_2->setMaximumSize(QSize(91, 30));

        horizontalLayout_4->addWidget(comboFormat_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName("label_7");
        label_7->setMaximumSize(QSize(65, 32));

        horizontalLayout_7->addWidget(label_7);

        leFilePath_2 = new QLineEdit(layoutWidget1);
        leFilePath_2->setObjectName("leFilePath_2");
        leFilePath_2->setReadOnly(true);

        horizontalLayout_7->addWidget(leFilePath_2);

        pushButton_3 = new QPushButton(layoutWidget1);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setMinimumSize(QSize(16, 32));
        pushButton_3->setMaximumSize(QSize(16, 32));

        horizontalLayout_7->addWidget(pushButton_3);


        verticalLayout_3->addLayout(horizontalLayout_7);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        label_9 = new QLabel(layoutWidget1);
        label_9->setObjectName("label_9");
        label_9->setMaximumSize(QSize(73, 30));

        horizontalLayout_10->addWidget(label_9);

        leFileName_2 = new QLineEdit(layoutWidget1);
        leFileName_2->setObjectName("leFileName_2");
        leFileName_2->setMaximumSize(QSize(142, 30));

        horizontalLayout_10->addWidget(leFileName_2);


        verticalLayout_3->addLayout(horizontalLayout_10);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName("horizontalLayout_13");
        label_12 = new QLabel(layoutWidget1);
        label_12->setObjectName("label_12");

        horizontalLayout_13->addWidget(label_12);

        comboRange_2 = new QComboBox(layoutWidget1);
        comboRange_2->addItem(QString());
        comboRange_2->addItem(QString());
        comboRange_2->setObjectName("comboRange_2");
        comboRange_2->setMinimumSize(QSize(131, 30));
        comboRange_2->setMaximumSize(QSize(131, 30));

        horizontalLayout_13->addWidget(comboRange_2);


        verticalLayout_3->addLayout(horizontalLayout_13);


        gridLayout_2->addLayout(verticalLayout_3, 0, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_5, 0, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(188, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnExport = new QPushButton(exportdlg);
        btnExport->setObjectName("btnExport");
        btnExport->setMaximumSize(QSize(80, 32));

        horizontalLayout->addWidget(btnExport);


        verticalLayout_2->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(exportdlg);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(exportdlg);
    } // setupUi

    void retranslateUi(QDialog *exportdlg)
    {
        exportdlg->setWindowTitle(QCoreApplication::translate("exportdlg", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("exportdlg", "Format", nullptr));
        comboFormat->setItemText(0, QCoreApplication::translate("exportdlg", "png", nullptr));
        comboFormat->setItemText(1, QCoreApplication::translate("exportdlg", "bmp", nullptr));
        comboFormat->setItemText(2, QCoreApplication::translate("exportdlg", "jpg", nullptr));

        label_3->setText(QCoreApplication::translate("exportdlg", "File path", nullptr));
        pushButton_2->setText(QCoreApplication::translate("exportdlg", "...", nullptr));
        label_2->setText(QCoreApplication::translate("exportdlg", "File name", nullptr));
        label_5->setText(QCoreApplication::translate("exportdlg", "Quality", nullptr));
        label_4->setText(QString());
        label_13->setText(QCoreApplication::translate("exportdlg", "Dimensions", nullptr));
        comboDimensions->setItemText(0, QCoreApplication::translate("exportdlg", "800x600", nullptr));
        comboDimensions->setItemText(1, QCoreApplication::translate("exportdlg", "1200x800", nullptr));
        comboDimensions->setItemText(2, QCoreApplication::translate("exportdlg", "2400x1600", nullptr));

        label_8->setText(QCoreApplication::translate("exportdlg", "Data range", nullptr));
        comboRange->setItemText(0, QCoreApplication::translate("exportdlg", "Current view", nullptr));
        comboRange->setItemText(1, QCoreApplication::translate("exportdlg", "All history", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(T), QCoreApplication::translate("exportdlg", "Export image", nullptr));
        label_6->setText(QCoreApplication::translate("exportdlg", "Format", nullptr));
        comboFormat_2->setItemText(0, QCoreApplication::translate("exportdlg", "csv", nullptr));
        comboFormat_2->setItemText(1, QCoreApplication::translate("exportdlg", "txt", nullptr));

        label_7->setText(QCoreApplication::translate("exportdlg", "File path", nullptr));
        pushButton_3->setText(QCoreApplication::translate("exportdlg", "...", nullptr));
        label_9->setText(QCoreApplication::translate("exportdlg", "File name", nullptr));
        label_12->setText(QCoreApplication::translate("exportdlg", "Data range", nullptr));
        comboRange_2->setItemText(0, QCoreApplication::translate("exportdlg", "Current view", nullptr));
        comboRange_2->setItemText(1, QCoreApplication::translate("exportdlg", "All history", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("exportdlg", "Export data", nullptr));
        btnExport->setText(QCoreApplication::translate("exportdlg", "Export", nullptr));
    } // retranslateUi

};

namespace Ui {
    class exportdlg: public Ui_exportdlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTDLG_H
