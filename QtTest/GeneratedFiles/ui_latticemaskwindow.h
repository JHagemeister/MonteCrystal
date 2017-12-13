/********************************************************************************
** Form generated from reading UI file 'latticemaskwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LATTICEMASKWINDOW_H
#define UI_LATTICEMASKWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include "CustomTableWidget.h"

QT_BEGIN_NAMESPACE

class Ui_LatticeMaskWindow
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    CustomTableWidget *tableWidget_Dimensions;
    QComboBox *comboBox_Type;
    QPushButton *pushButton_Ok;

    void setupUi(QDialog *LatticeMaskWindow)
    {
        if (LatticeMaskWindow->objectName().isEmpty())
            LatticeMaskWindow->setObjectName(QStringLiteral("LatticeMaskWindow"));
        LatticeMaskWindow->resize(537, 158);
        gridLayout = new QGridLayout(LatticeMaskWindow);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tableWidget_Dimensions = new CustomTableWidget(LatticeMaskWindow);
        tableWidget_Dimensions->setObjectName(QStringLiteral("tableWidget_Dimensions"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget_Dimensions->sizePolicy().hasHeightForWidth());
        tableWidget_Dimensions->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(tableWidget_Dimensions);

        comboBox_Type = new QComboBox(LatticeMaskWindow);
        comboBox_Type->setObjectName(QStringLiteral("comboBox_Type"));
        sizePolicy.setHeightForWidth(comboBox_Type->sizePolicy().hasHeightForWidth());
        comboBox_Type->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(comboBox_Type);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);

        pushButton_Ok = new QPushButton(LatticeMaskWindow);
        pushButton_Ok->setObjectName(QStringLiteral("pushButton_Ok"));
        sizePolicy.setHeightForWidth(pushButton_Ok->sizePolicy().hasHeightForWidth());
        pushButton_Ok->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(pushButton_Ok);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 1);

        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(LatticeMaskWindow);

        QMetaObject::connectSlotsByName(LatticeMaskWindow);
    } // setupUi

    void retranslateUi(QDialog *LatticeMaskWindow)
    {
        LatticeMaskWindow->setWindowTitle(QApplication::translate("LatticeMaskWindow", "LatticeMaskWindow", Q_NULLPTR));
        pushButton_Ok->setText(QApplication::translate("LatticeMaskWindow", "Ok", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LatticeMaskWindow: public Ui_LatticeMaskWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LATTICEMASKWINDOW_H
