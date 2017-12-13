/********************************************************************************
** Form generated from reading UI file 'experiment01.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPERIMENT01_H
#define UI_EXPERIMENT01_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include "CustomTableWidget.h"

QT_BEGIN_NAMESPACE

class Ui_Experiment01
{
public:
    QGridLayout *gridLayout;
    CustomTableWidget *tableWidget;

    void setupUi(QDialog *Experiment01)
    {
        if (Experiment01->objectName().isEmpty())
            Experiment01->setObjectName(QStringLiteral("Experiment01"));
        Experiment01->resize(331, 176);
        gridLayout = new QGridLayout(Experiment01);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tableWidget = new CustomTableWidget(Experiment01);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);


        retranslateUi(Experiment01);

        QMetaObject::connectSlotsByName(Experiment01);
    } // setupUi

    void retranslateUi(QDialog *Experiment01)
    {
        Experiment01->setWindowTitle(QApplication::translate("Experiment01", "Experiment01", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Experiment01: public Ui_Experiment01 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPERIMENT01_H
