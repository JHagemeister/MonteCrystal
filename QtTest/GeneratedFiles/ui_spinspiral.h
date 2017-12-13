/********************************************************************************
** Form generated from reading UI file 'spinspiral.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPINSPIRAL_H
#define UI_SPINSPIRAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "CustomTableWidget.h"

QT_BEGIN_NAMESPACE

class Ui_SpinSpiralWindow
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    CustomTableWidget *tableWidget;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBoxHelicity;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonUndo;
    QPushButton *pushButtonOk;

    void setupUi(QDialog *SpinSpiralWindow)
    {
        if (SpinSpiralWindow->objectName().isEmpty())
            SpinSpiralWindow->setObjectName(QStringLiteral("SpinSpiralWindow"));
        SpinSpiralWindow->resize(996, 234);
        SpinSpiralWindow->setStyleSheet(QLatin1String("QWidget{\n"
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
        verticalLayout_3 = new QVBoxLayout(SpinSpiralWindow);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tableWidget = new CustomTableWidget(SpinSpiralWindow);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(tableWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        comboBoxHelicity = new QComboBox(SpinSpiralWindow);
        comboBoxHelicity->setObjectName(QStringLiteral("comboBoxHelicity"));
        sizePolicy.setHeightForWidth(comboBoxHelicity->sizePolicy().hasHeightForWidth());
        comboBoxHelicity->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(comboBoxHelicity);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 1);

        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 4);
        horizontalLayout->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButtonApply = new QPushButton(SpinSpiralWindow);
        pushButtonApply->setObjectName(QStringLiteral("pushButtonApply"));
        sizePolicy.setHeightForWidth(pushButtonApply->sizePolicy().hasHeightForWidth());
        pushButtonApply->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(pushButtonApply);

        pushButtonUndo = new QPushButton(SpinSpiralWindow);
        pushButtonUndo->setObjectName(QStringLiteral("pushButtonUndo"));
        sizePolicy.setHeightForWidth(pushButtonUndo->sizePolicy().hasHeightForWidth());
        pushButtonUndo->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(pushButtonUndo);

        pushButtonOk = new QPushButton(SpinSpiralWindow);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonOk"));
        sizePolicy.setHeightForWidth(pushButtonOk->sizePolicy().hasHeightForWidth());
        pushButtonOk->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(pushButtonOk);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 1);
        horizontalLayout_2->setStretch(2, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalLayout_2->setStretch(0, 3);
        verticalLayout_2->setStretch(1, 1);

        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(SpinSpiralWindow);
        QObject::connect(pushButtonOk, SIGNAL(released()), SpinSpiralWindow, SLOT(close()));
        QObject::connect(pushButtonApply, SIGNAL(released()), SpinSpiralWindow, SLOT(on_push_button_apply()));
        QObject::connect(pushButtonUndo, SIGNAL(released()), SpinSpiralWindow, SLOT(on_push_button_undo()));

        QMetaObject::connectSlotsByName(SpinSpiralWindow);
    } // setupUi

    void retranslateUi(QDialog *SpinSpiralWindow)
    {
        SpinSpiralWindow->setWindowTitle(QApplication::translate("SpinSpiralWindow", "Spin spiral window", Q_NULLPTR));
        pushButtonApply->setText(QApplication::translate("SpinSpiralWindow", "Apply", Q_NULLPTR));
        pushButtonUndo->setText(QApplication::translate("SpinSpiralWindow", "Undo", Q_NULLPTR));
        pushButtonOk->setText(QApplication::translate("SpinSpiralWindow", "Ok", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SpinSpiralWindow: public Ui_SpinSpiralWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPINSPIRAL_H
