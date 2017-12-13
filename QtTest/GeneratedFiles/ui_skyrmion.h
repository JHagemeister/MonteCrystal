/********************************************************************************
** Form generated from reading UI file 'skyrmion.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SKYRMION_H
#define UI_SKYRMION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "CustomTableWidget.h"

QT_BEGIN_NAMESPACE

class Ui_skyrmion
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    CustomTableWidget *tableWidget_Skyrmion;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *comboBox_Type;
    QComboBox *comboBox_Handedness;
    QComboBox *comboBox_CenterDirection;
    QSpacerItem *verticalSpacer;
    CustomTableWidget *tableWidget_order;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_apply;
    QPushButton *pushButton_undo;
    QPushButton *pushButton_ok;

    void setupUi(QWidget *skyrmion)
    {
        if (skyrmion->objectName().isEmpty())
            skyrmion->setObjectName(QStringLiteral("skyrmion"));
        skyrmion->resize(929, 187);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(skyrmion->sizePolicy().hasHeightForWidth());
        skyrmion->setSizePolicy(sizePolicy);
        skyrmion->setStyleSheet(QLatin1String("QWidget{\n"
"background: rgb(214, 219, 233,255);\n"
"}\n"
"\n"
"QComboBox{\n"
"background: rgb(214, 219, 233,255);\n"
"border: none;\n"
"}\n"
"\n"
"QComboBox:hover{\n"
"background: rgb(255, 255, 191,255);\n"
"border:  2px solid rgb(255, 170, 0,255);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"background: rgb(255, 255, 191,255);\n"
"border:  2px solid rgb(255, 170, 0,255);\n"
"}\n"
"\n"
"QPushButton{\n"
"border: none;\n"
"background: rgb(214, 219, 233,255);\n"
"}\n"
"\n"
"QTableWidget {\n"
"    background: rgb(214, 219, 233,255);\n"
"border: none;\n"
"}\n"
"\n"
"QHeaderView::section{\n"
"background-color: rgb(235, 241, 255, 255);\n"
"}\n"
""));
        verticalLayout_3 = new QVBoxLayout(skyrmion);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        tableWidget_Skyrmion = new CustomTableWidget(skyrmion);
        tableWidget_Skyrmion->setObjectName(QStringLiteral("tableWidget_Skyrmion"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableWidget_Skyrmion->sizePolicy().hasHeightForWidth());
        tableWidget_Skyrmion->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(tableWidget_Skyrmion);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        comboBox_Type = new QComboBox(skyrmion);
        comboBox_Type->setObjectName(QStringLiteral("comboBox_Type"));
        sizePolicy1.setHeightForWidth(comboBox_Type->sizePolicy().hasHeightForWidth());
        comboBox_Type->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(comboBox_Type);

        comboBox_Handedness = new QComboBox(skyrmion);
        comboBox_Handedness->setObjectName(QStringLiteral("comboBox_Handedness"));
        sizePolicy1.setHeightForWidth(comboBox_Handedness->sizePolicy().hasHeightForWidth());
        comboBox_Handedness->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(comboBox_Handedness);

        comboBox_CenterDirection = new QComboBox(skyrmion);
        comboBox_CenterDirection->setObjectName(QStringLiteral("comboBox_CenterDirection"));
        sizePolicy1.setHeightForWidth(comboBox_CenterDirection->sizePolicy().hasHeightForWidth());
        comboBox_CenterDirection->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(comboBox_CenterDirection);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        verticalLayout->setStretch(0, 5);

        horizontalLayout_3->addLayout(verticalLayout);

        tableWidget_order = new CustomTableWidget(skyrmion);
        tableWidget_order->setObjectName(QStringLiteral("tableWidget_order"));
        sizePolicy1.setHeightForWidth(tableWidget_order->sizePolicy().hasHeightForWidth());
        tableWidget_order->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(tableWidget_order);

        horizontalLayout_3->setStretch(0, 4);
        horizontalLayout_3->setStretch(1, 5);
        horizontalLayout_3->setStretch(2, 1);

        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_apply = new QPushButton(skyrmion);
        pushButton_apply->setObjectName(QStringLiteral("pushButton_apply"));
        sizePolicy1.setHeightForWidth(pushButton_apply->sizePolicy().hasHeightForWidth());
        pushButton_apply->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(pushButton_apply);

        pushButton_undo = new QPushButton(skyrmion);
        pushButton_undo->setObjectName(QStringLiteral("pushButton_undo"));
        sizePolicy1.setHeightForWidth(pushButton_undo->sizePolicy().hasHeightForWidth());
        pushButton_undo->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(pushButton_undo);

        pushButton_ok = new QPushButton(skyrmion);
        pushButton_ok->setObjectName(QStringLiteral("pushButton_ok"));
        sizePolicy1.setHeightForWidth(pushButton_ok->sizePolicy().hasHeightForWidth());
        pushButton_ok->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(pushButton_ok);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalLayout_2->setStretch(0, 2);
        verticalLayout_2->setStretch(1, 1);

        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(skyrmion);
        QObject::connect(pushButton_apply, SIGNAL(released()), skyrmion, SLOT(on_push_button_apply()));
        QObject::connect(pushButton_undo, SIGNAL(released()), skyrmion, SLOT(on_push_button_undo()));
        QObject::connect(pushButton_ok, SIGNAL(released()), skyrmion, SLOT(close()));

        QMetaObject::connectSlotsByName(skyrmion);
    } // setupUi

    void retranslateUi(QWidget *skyrmion)
    {
        skyrmion->setWindowTitle(QApplication::translate("skyrmion", "Skyrmion window", Q_NULLPTR));
        pushButton_apply->setText(QApplication::translate("skyrmion", "Apply", Q_NULLPTR));
        pushButton_undo->setText(QApplication::translate("skyrmion", "Undo", Q_NULLPTR));
        pushButton_ok->setText(QApplication::translate("skyrmion", "Ok", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class skyrmion: public Ui_skyrmion {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SKYRMION_H
