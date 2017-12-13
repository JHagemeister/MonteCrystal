/********************************************************************************
** Form generated from reading UI file 'anisotropy.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANISOTROPY_H
#define UI_ANISOTROPY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include "CustomTableWidget.h"

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEditHexagonal;
    CustomTableWidget *tableWidgetHexagonal;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEditUniaxial;
    QListView *listViewUniaxial;
    CustomTableWidget *tableWidgetUniaxial;
    QHBoxLayout *horizontalLayout;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *pushButtonOk;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(666, 755);
        Dialog->setStyleSheet(QLatin1String("QDialog{\n"
"background: rgb(214, 219, 233,255);\n"
"}\n"
"\n"
"QListView{\n"
"background: rgb(214, 219, 233,255);\n"
"border: 2px solid rgb(231, 236, 250, 255);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"background: rgb(255, 255, 191,255);\n"
"border:  2px solid rgb(255, 170, 0,255);\n"
"}\n"
"\n"
"QPushButton{\n"
"border:  2px solid rgb(231, 236, 250, 255);;\n"
"background: rgb(214, 219, 233,255);\n"
"}\n"
"\n"
"QTableWidget{\n"
"background: rgb(214, 219, 233,255);\n"
"border: 2px solid rgb(231, 236, 250, 255);\n"
"}\n"
"\n"
"QHeaderView::section{\n"
"background-color: rgb(235, 241, 255, 255);\n"
"}\n"
""));
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(10);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textEditHexagonal = new QTextEdit(Dialog);
        textEditHexagonal->setObjectName(QStringLiteral("textEditHexagonal"));
        textEditHexagonal->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEditHexagonal->sizePolicy().hasHeightForWidth());
        textEditHexagonal->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(textEditHexagonal);

        tableWidgetHexagonal = new CustomTableWidget(Dialog);
        tableWidgetHexagonal->setObjectName(QStringLiteral("tableWidgetHexagonal"));
        sizePolicy.setHeightForWidth(tableWidgetHexagonal->sizePolicy().hasHeightForWidth());
        tableWidgetHexagonal->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(tableWidgetHexagonal);

        verticalLayout->setStretch(0, 5);
        verticalLayout->setStretch(1, 9);

        verticalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        textEditUniaxial = new QTextEdit(Dialog);
        textEditUniaxial->setObjectName(QStringLiteral("textEditUniaxial"));
        textEditUniaxial->setEnabled(false);
        sizePolicy.setHeightForWidth(textEditUniaxial->sizePolicy().hasHeightForWidth());
        textEditUniaxial->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(textEditUniaxial);

        listViewUniaxial = new QListView(Dialog);
        listViewUniaxial->setObjectName(QStringLiteral("listViewUniaxial"));
        sizePolicy.setHeightForWidth(listViewUniaxial->sizePolicy().hasHeightForWidth());
        listViewUniaxial->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(listViewUniaxial);

        tableWidgetUniaxial = new CustomTableWidget(Dialog);
        tableWidgetUniaxial->setObjectName(QStringLiteral("tableWidgetUniaxial"));
        sizePolicy.setHeightForWidth(tableWidgetUniaxial->sizePolicy().hasHeightForWidth());
        tableWidgetUniaxial->setSizePolicy(sizePolicy);
        tableWidgetUniaxial->setStyleSheet(QStringLiteral(""));

        verticalLayout_2->addWidget(tableWidgetUniaxial);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        addButton = new QPushButton(Dialog);
        addButton->setObjectName(QStringLiteral("addButton"));
        sizePolicy.setHeightForWidth(addButton->sizePolicy().hasHeightForWidth());
        addButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(addButton);

        deleteButton = new QPushButton(Dialog);
        deleteButton->setObjectName(QStringLiteral("deleteButton"));
        sizePolicy.setHeightForWidth(deleteButton->sizePolicy().hasHeightForWidth());
        deleteButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(deleteButton);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalLayout_2->setStretch(0, 2);
        verticalLayout_2->setStretch(1, 7);
        verticalLayout_2->setStretch(2, 3);
        verticalLayout_2->setStretch(3, 2);

        verticalLayout_3->addLayout(verticalLayout_2);

        pushButtonOk = new QPushButton(Dialog);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonOk"));
        sizePolicy.setHeightForWidth(pushButtonOk->sizePolicy().hasHeightForWidth());
        pushButtonOk->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(pushButtonOk);

        verticalLayout_3->setStretch(0, 3);
        verticalLayout_3->setStretch(1, 7);
        verticalLayout_3->setStretch(2, 1);

        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 1);


        retranslateUi(Dialog);
        QObject::connect(addButton, SIGNAL(released()), Dialog, SLOT(on_add_clicked()));
        QObject::connect(deleteButton, SIGNAL(released()), Dialog, SLOT(on_delete_clicked()));
        QObject::connect(pushButtonOk, SIGNAL(released()), Dialog, SLOT(on_ok_clicked()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Anisotropy window", Q_NULLPTR));
        addButton->setText(QApplication::translate("Dialog", "Add", Q_NULLPTR));
        deleteButton->setText(QApplication::translate("Dialog", "Delete", Q_NULLPTR));
        pushButtonOk->setText(QApplication::translate("Dialog", "Ok", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANISOTROPY_H
