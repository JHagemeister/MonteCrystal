/********************************************************************************
** Form generated from reading UI file 'ferromagnet.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FERROMAGNET_H
#define UI_FERROMAGNET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include "CustomTableWidget.h"

QT_BEGIN_NAMESPACE

class Ui_FerromagnetWindow
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    CustomTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonUndo;
    QPushButton *pushButtonOk;

    void setupUi(QDialog *FerromagnetWindow)
    {
        if (FerromagnetWindow->objectName().isEmpty())
            FerromagnetWindow->setObjectName(QStringLiteral("FerromagnetWindow"));
        FerromagnetWindow->resize(624, 212);
        FerromagnetWindow->setStyleSheet(QLatin1String("QWidget{\n"
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
        verticalLayout_2 = new QVBoxLayout(FerromagnetWindow);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableWidget = new CustomTableWidget(FerromagnetWindow);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(tableWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButtonApply = new QPushButton(FerromagnetWindow);
        pushButtonApply->setObjectName(QStringLiteral("pushButtonApply"));
        sizePolicy.setHeightForWidth(pushButtonApply->sizePolicy().hasHeightForWidth());
        pushButtonApply->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonApply);

        pushButtonUndo = new QPushButton(FerromagnetWindow);
        pushButtonUndo->setObjectName(QStringLiteral("pushButtonUndo"));
        sizePolicy.setHeightForWidth(pushButtonUndo->sizePolicy().hasHeightForWidth());
        pushButtonUndo->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonUndo);

        pushButtonOk = new QPushButton(FerromagnetWindow);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonOk"));
        sizePolicy.setHeightForWidth(pushButtonOk->sizePolicy().hasHeightForWidth());
        pushButtonOk->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonOk);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);

        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 1);

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(FerromagnetWindow);
        QObject::connect(pushButtonOk, SIGNAL(released()), FerromagnetWindow, SLOT(close()));
        QObject::connect(pushButtonUndo, SIGNAL(released()), FerromagnetWindow, SLOT(on_push_button_undo()));
        QObject::connect(pushButtonApply, SIGNAL(released()), FerromagnetWindow, SLOT(on_push_button_apply()));

        QMetaObject::connectSlotsByName(FerromagnetWindow);
    } // setupUi

    void retranslateUi(QDialog *FerromagnetWindow)
    {
        FerromagnetWindow->setWindowTitle(QApplication::translate("FerromagnetWindow", "FerromagnetWindow", Q_NULLPTR));
        pushButtonApply->setText(QApplication::translate("FerromagnetWindow", "Apply", Q_NULLPTR));
        pushButtonUndo->setText(QApplication::translate("FerromagnetWindow", "Undo", Q_NULLPTR));
        pushButtonOk->setText(QApplication::translate("FerromagnetWindow", "Ok", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FerromagnetWindow: public Ui_FerromagnetWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FERROMAGNET_H
