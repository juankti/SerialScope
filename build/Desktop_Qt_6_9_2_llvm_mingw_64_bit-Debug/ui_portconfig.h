/********************************************************************************
** Form generated from reading UI file 'portconfig.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PORTCONFIG_H
#define UI_PORTCONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_portconfig
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnPorts;
    QSpacerItem *horizontalSpacer_2;
    QListWidget *portsList;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *leBaud;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QComboBox *comboBaud_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QComboBox *comboBaud_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QComboBox *comboBaud_4;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QComboBox *comboBaud_5;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QDialog *portconfig)
    {
        if (portconfig->objectName().isEmpty())
            portconfig->setObjectName("portconfig");
        portconfig->resize(337, 303);
        portconfig->setMinimumSize(QSize(337, 303));
        portconfig->setMaximumSize(QSize(337, 303));
        portconfig->setSizeGripEnabled(false);
        gridLayout = new QGridLayout(portconfig);
        gridLayout->setObjectName("gridLayout");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnPorts = new QPushButton(portconfig);
        btnPorts->setObjectName("btnPorts");
        btnPorts->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        btnPorts->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(btnPorts);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_2);

        portsList = new QListWidget(portconfig);
        portsList->setObjectName("portsList");
        portsList->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        portsList->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        portsList->setUniformItemSizes(true);
        portsList->setWordWrap(true);
        portsList->setItemAlignment(Qt::AlignmentFlag::AlignLeading);

        verticalLayout_2->addWidget(portsList);

        verticalSpacer = new QSpacerItem(20, 78, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_2 = new QLabel(portconfig);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(label_2);

        verticalSpacer_2 = new QSpacerItem(20, 13, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        verticalLayout_4->addLayout(verticalLayout_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(portconfig);
        label->setObjectName("label");
        label->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label);

        leBaud = new QLineEdit(portconfig);
        leBaud->setObjectName("leBaud");

        horizontalLayout->addWidget(leBaud);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_3 = new QLabel(portconfig);
        label_3->setObjectName("label_3");
        label_3->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_3->addWidget(label_3);

        comboBaud_2 = new QComboBox(portconfig);
        comboBaud_2->setObjectName("comboBaud_2");

        horizontalLayout_3->addWidget(comboBaud_2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_4 = new QLabel(portconfig);
        label_4->setObjectName("label_4");
        label_4->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_4->addWidget(label_4);

        comboBaud_3 = new QComboBox(portconfig);
        comboBaud_3->setObjectName("comboBaud_3");

        horizontalLayout_4->addWidget(comboBaud_3);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_5 = new QLabel(portconfig);
        label_5->setObjectName("label_5");
        label_5->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_5->addWidget(label_5);

        comboBaud_4 = new QComboBox(portconfig);
        comboBaud_4->setObjectName("comboBaud_4");

        horizontalLayout_5->addWidget(comboBaud_4);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_6 = new QLabel(portconfig);
        label_6->setObjectName("label_6");
        label_6->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_6->addWidget(label_6);

        comboBaud_5 = new QComboBox(portconfig);
        comboBaud_5->setObjectName("comboBaud_5");

        horizontalLayout_6->addWidget(comboBaud_5);


        verticalLayout->addLayout(horizontalLayout_6);


        verticalLayout_4->addLayout(verticalLayout);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);

        pushButton = new QPushButton(portconfig);
        pushButton->setObjectName("pushButton");

        horizontalLayout_7->addWidget(pushButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);


        verticalLayout_4->addLayout(horizontalLayout_7);


        gridLayout->addLayout(verticalLayout_4, 0, 1, 1, 1);


        retranslateUi(portconfig);

        QMetaObject::connectSlotsByName(portconfig);
    } // setupUi

    void retranslateUi(QDialog *portconfig)
    {
        portconfig->setWindowTitle(QCoreApplication::translate("portconfig", "Dialog", nullptr));
        btnPorts->setText(QCoreApplication::translate("portconfig", "Search", nullptr));
        label_2->setText(QString());
        label->setText(QCoreApplication::translate("portconfig", "Baud rate:", nullptr));
        label_3->setText(QCoreApplication::translate("portconfig", "Data bits:", nullptr));
        label_4->setText(QCoreApplication::translate("portconfig", "Parity:", nullptr));
        label_5->setText(QCoreApplication::translate("portconfig", "Stop bits:", nullptr));
        label_6->setText(QCoreApplication::translate("portconfig", "Flow Control:", nullptr));
        pushButton->setText(QCoreApplication::translate("portconfig", "Configure", nullptr));
    } // retranslateUi

};

namespace Ui {
    class portconfig: public Ui_portconfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PORTCONFIG_H
