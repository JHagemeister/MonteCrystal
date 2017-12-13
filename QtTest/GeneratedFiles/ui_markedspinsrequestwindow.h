/********************************************************************************
** Form generated from reading UI file 'markedspinsrequestwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARKEDSPINSREQUESTWINDOW_H
#define UI_MARKEDSPINSREQUESTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "CustomTableWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MarkedSpinsRequestWindow
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_lock;
    QPushButton *pushButton_unlock;
    CustomTableWidget *tableWidget_direction;
    CustomTableWidget *tableWidget_JDefect;
    CustomTableWidget *tableWidget_DDefect;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEdit_KDefect;
    CustomTableWidget *tableWidget_KDefect;
    QPushButton *pushButton_showDefects;
    QPushButton *pushButton_showLockedSpins;
    QPushButton *pushButton_undo;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QListView *listView_indexes;

    void setupUi(QWidget *MarkedSpinsRequestWindow)
    {
        if (MarkedSpinsRequestWindow->objectName().isEmpty())
            MarkedSpinsRequestWindow->setObjectName(QStringLiteral("MarkedSpinsRequestWindow"));
        MarkedSpinsRequestWindow->resize(490, 537);
        gridLayout = new QGridLayout(MarkedSpinsRequestWindow);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_lock = new QPushButton(MarkedSpinsRequestWindow);
        pushButton_lock->setObjectName(QStringLiteral("pushButton_lock"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_lock->sizePolicy().hasHeightForWidth());
        pushButton_lock->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton_lock);

        pushButton_unlock = new QPushButton(MarkedSpinsRequestWindow);
        pushButton_unlock->setObjectName(QStringLiteral("pushButton_unlock"));
        sizePolicy.setHeightForWidth(pushButton_unlock->sizePolicy().hasHeightForWidth());
        pushButton_unlock->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton_unlock);


        verticalLayout->addLayout(horizontalLayout);

        tableWidget_direction = new CustomTableWidget(MarkedSpinsRequestWindow);
        tableWidget_direction->setObjectName(QStringLiteral("tableWidget_direction"));
        sizePolicy.setHeightForWidth(tableWidget_direction->sizePolicy().hasHeightForWidth());
        tableWidget_direction->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(tableWidget_direction);

        tableWidget_JDefect = new CustomTableWidget(MarkedSpinsRequestWindow);
        tableWidget_JDefect->setObjectName(QStringLiteral("tableWidget_JDefect"));
        sizePolicy.setHeightForWidth(tableWidget_JDefect->sizePolicy().hasHeightForWidth());
        tableWidget_JDefect->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(tableWidget_JDefect);

        tableWidget_DDefect = new CustomTableWidget(MarkedSpinsRequestWindow);
        tableWidget_DDefect->setObjectName(QStringLiteral("tableWidget_DDefect"));
        sizePolicy.setHeightForWidth(tableWidget_DDefect->sizePolicy().hasHeightForWidth());
        tableWidget_DDefect->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(tableWidget_DDefect);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        textEdit_KDefect = new QTextEdit(MarkedSpinsRequestWindow);
        textEdit_KDefect->setObjectName(QStringLiteral("textEdit_KDefect"));
        textEdit_KDefect->setEnabled(false);
        sizePolicy.setHeightForWidth(textEdit_KDefect->sizePolicy().hasHeightForWidth());
        textEdit_KDefect->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(textEdit_KDefect);

        tableWidget_KDefect = new CustomTableWidget(MarkedSpinsRequestWindow);
        tableWidget_KDefect->setObjectName(QStringLiteral("tableWidget_KDefect"));
        sizePolicy.setHeightForWidth(tableWidget_KDefect->sizePolicy().hasHeightForWidth());
        tableWidget_KDefect->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(tableWidget_KDefect);


        verticalLayout->addLayout(verticalLayout_2);

        pushButton_showDefects = new QPushButton(MarkedSpinsRequestWindow);
        pushButton_showDefects->setObjectName(QStringLiteral("pushButton_showDefects"));
        sizePolicy.setHeightForWidth(pushButton_showDefects->sizePolicy().hasHeightForWidth());
        pushButton_showDefects->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(pushButton_showDefects);

        pushButton_showLockedSpins = new QPushButton(MarkedSpinsRequestWindow);
        pushButton_showLockedSpins->setObjectName(QStringLiteral("pushButton_showLockedSpins"));
        sizePolicy.setHeightForWidth(pushButton_showLockedSpins->sizePolicy().hasHeightForWidth());
        pushButton_showLockedSpins->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(pushButton_showLockedSpins);

        pushButton_undo = new QPushButton(MarkedSpinsRequestWindow);
        pushButton_undo->setObjectName(QStringLiteral("pushButton_undo"));
        sizePolicy.setHeightForWidth(pushButton_undo->sizePolicy().hasHeightForWidth());
        pushButton_undo->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(pushButton_undo);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 1);
        verticalLayout->setStretch(3, 1);
        verticalLayout->setStretch(4, 2);
        verticalLayout->setStretch(5, 1);
        verticalLayout->setStretch(6, 1);
        verticalLayout->setStretch(7, 1);

        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label = new QLabel(MarkedSpinsRequestWindow);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_3->addWidget(label);

        listView_indexes = new QListView(MarkedSpinsRequestWindow);
        listView_indexes->setObjectName(QStringLiteral("listView_indexes"));
        sizePolicy.setHeightForWidth(listView_indexes->sizePolicy().hasHeightForWidth());
        listView_indexes->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(listView_indexes);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 15);

        horizontalLayout_2->addLayout(verticalLayout_3);

        horizontalLayout_2->setStretch(0, 7);
        horizontalLayout_2->setStretch(1, 3);

        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        retranslateUi(MarkedSpinsRequestWindow);

        QMetaObject::connectSlotsByName(MarkedSpinsRequestWindow);
    } // setupUi

    void retranslateUi(QWidget *MarkedSpinsRequestWindow)
    {
        MarkedSpinsRequestWindow->setWindowTitle(QApplication::translate("MarkedSpinsRequestWindow", "MarkedSpinsRequestWindow", Q_NULLPTR));
        pushButton_lock->setText(QApplication::translate("MarkedSpinsRequestWindow", "lock spins", Q_NULLPTR));
        pushButton_unlock->setText(QApplication::translate("MarkedSpinsRequestWindow", "unlock spins", Q_NULLPTR));
        pushButton_showDefects->setText(QApplication::translate("MarkedSpinsRequestWindow", "show defects", Q_NULLPTR));
        pushButton_showLockedSpins->setText(QApplication::translate("MarkedSpinsRequestWindow", "show locked spins", Q_NULLPTR));
        pushButton_undo->setText(QApplication::translate("MarkedSpinsRequestWindow", "Remove defects + unlock all", Q_NULLPTR));
        label->setText(QApplication::translate("MarkedSpinsRequestWindow", "Marked Spins", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MarkedSpinsRequestWindow: public Ui_MarkedSpinsRequestWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARKEDSPINSREQUESTWINDOW_H
