/********************************************************************************
** Form generated from reading UI file 'colors.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORS_H
#define UI_COLORS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "CustomTableWidget.h"

QT_BEGIN_NAMESPACE

class Ui_Colors
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_11;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_14;
    QLabel *labelBackgroundColor;
    CustomTableWidget *tableWidgetBackgroundColor;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *checkBoxSpinVectors;
    QPushButton *pushButton_addSpins;
    QPushButton *pushButton_removeSpins;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBoxSpinVectorSingleColor;
    QSpacerItem *verticalSpacer;
    CustomTableWidget *tableWidgetSpinColor;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBoxSpinVectorRGB;
    QSpacerItem *horizontalSpacer;
    CustomTableWidget *tableWidget_SpinMesh;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_10;
    QGridLayout *gridLayoutColorMapItems;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_13;
    CustomTableWidget *tableWidgetMTheta;
    CustomTableWidget *tableWidgetMPhi;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labelCurrentValueRange;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_7;
    CustomTableWidget *tableWidgetCurrentMin;
    CustomTableWidget *tableWidgetCurrentMax;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_8;
    QLabel *labelChooseRange;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_9;
    CustomTableWidget *tableWidgetChosenMin;
    CustomTableWidget *tableWidgetChosenMax;
    QVBoxLayout *verticalLayout_5;
    CustomTableWidget *tableWidgetColorMap;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *pushButton;
    QPushButton *pushButtonOk;

    void setupUi(QDialog *Colors)
    {
        if (Colors->objectName().isEmpty())
            Colors->setObjectName(QStringLiteral("Colors"));
        Colors->resize(519, 1274);
        Colors->setStyleSheet(QLatin1String("QDialog{\n"
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
"\n"
"QHeaderView::section{\n"
"background-color: rgb(235, 241, 255, 255);\n"
"}\n"
""));
        gridLayout_2 = new QGridLayout(Colors);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(10);
        gridLayout_2->setContentsMargins(3, 0, 3, 0);
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(3);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(-1, 0, -1, 0);
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(60);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(3);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(-1, 0, -1, 0);
        labelBackgroundColor = new QLabel(Colors);
        labelBackgroundColor->setObjectName(QStringLiteral("labelBackgroundColor"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelBackgroundColor->sizePolicy().hasHeightForWidth());
        labelBackgroundColor->setSizePolicy(sizePolicy);

        horizontalLayout_14->addWidget(labelBackgroundColor);

        tableWidgetBackgroundColor = new CustomTableWidget(Colors);
        tableWidgetBackgroundColor->setObjectName(QStringLiteral("tableWidgetBackgroundColor"));
        sizePolicy.setHeightForWidth(tableWidgetBackgroundColor->sizePolicy().hasHeightForWidth());
        tableWidgetBackgroundColor->setSizePolicy(sizePolicy);

        horizontalLayout_14->addWidget(tableWidgetBackgroundColor);

        horizontalLayout_14->setStretch(0, 5);
        horizontalLayout_14->setStretch(1, 8);

        verticalLayout_4->addLayout(horizontalLayout_14);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        checkBoxSpinVectors = new QCheckBox(Colors);
        checkBoxSpinVectors->setObjectName(QStringLiteral("checkBoxSpinVectors"));
        sizePolicy.setHeightForWidth(checkBoxSpinVectors->sizePolicy().hasHeightForWidth());
        checkBoxSpinVectors->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(checkBoxSpinVectors);

        pushButton_addSpins = new QPushButton(Colors);
        pushButton_addSpins->setObjectName(QStringLiteral("pushButton_addSpins"));
        sizePolicy.setHeightForWidth(pushButton_addSpins->sizePolicy().hasHeightForWidth());
        pushButton_addSpins->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(pushButton_addSpins);

        pushButton_removeSpins = new QPushButton(Colors);
        pushButton_removeSpins->setObjectName(QStringLiteral("pushButton_removeSpins"));
        sizePolicy.setHeightForWidth(pushButton_removeSpins->sizePolicy().hasHeightForWidth());
        pushButton_removeSpins->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(pushButton_removeSpins);

        horizontalLayout_4->setStretch(0, 2);
        horizontalLayout_4->setStretch(1, 1);
        horizontalLayout_4->setStretch(2, 1);

        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Ignored, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        checkBoxSpinVectorSingleColor = new QCheckBox(Colors);
        checkBoxSpinVectorSingleColor->setObjectName(QStringLiteral("checkBoxSpinVectorSingleColor"));
        sizePolicy.setHeightForWidth(checkBoxSpinVectorSingleColor->sizePolicy().hasHeightForWidth());
        checkBoxSpinVectorSingleColor->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(checkBoxSpinVectorSingleColor);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Ignored);

        verticalLayout->addItem(verticalSpacer);

        verticalLayout->setStretch(0, 3);
        verticalLayout->setStretch(1, 1);

        horizontalLayout->addLayout(verticalLayout);

        tableWidgetSpinColor = new CustomTableWidget(Colors);
        tableWidgetSpinColor->setObjectName(QStringLiteral("tableWidgetSpinColor"));
        sizePolicy.setHeightForWidth(tableWidgetSpinColor->sizePolicy().hasHeightForWidth());
        tableWidgetSpinColor->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(tableWidgetSpinColor);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 4);

        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        checkBoxSpinVectorRGB = new QCheckBox(Colors);
        checkBoxSpinVectorRGB->setObjectName(QStringLiteral("checkBoxSpinVectorRGB"));
        sizePolicy.setHeightForWidth(checkBoxSpinVectorRGB->sizePolicy().hasHeightForWidth());
        checkBoxSpinVectorRGB->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(checkBoxSpinVectorRGB);

        horizontalSpacer = new QSpacerItem(258, 20, QSizePolicy::Ignored, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout_2->setStretch(0, 2);
        horizontalLayout_2->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalLayout_2->setStretch(0, 2);
        verticalLayout_2->setStretch(1, 1);

        horizontalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 19);

        verticalLayout_3->addLayout(horizontalLayout_3);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 2);

        verticalLayout_4->addLayout(verticalLayout_3);

        tableWidget_SpinMesh = new CustomTableWidget(Colors);
        tableWidget_SpinMesh->setObjectName(QStringLiteral("tableWidget_SpinMesh"));
        sizePolicy.setHeightForWidth(tableWidget_SpinMesh->sizePolicy().hasHeightForWidth());
        tableWidget_SpinMesh->setSizePolicy(sizePolicy);

        verticalLayout_4->addWidget(tableWidget_SpinMesh);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(-1, -1, -1, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label = new QLabel(Colors);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(label);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Ignored, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        verticalLayout_11->addLayout(horizontalLayout_5);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(0);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(-1, -1, 0, 0);
        gridLayoutColorMapItems = new QGridLayout();
        gridLayoutColorMapItems->setSpacing(6);
        gridLayoutColorMapItems->setObjectName(QStringLiteral("gridLayoutColorMapItems"));
        gridLayoutColorMapItems->setContentsMargins(-1, -1, -1, 0);

        horizontalLayout_10->addLayout(gridLayoutColorMapItems);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Ignored);

        horizontalLayout_10->addItem(verticalSpacer_2);

        horizontalLayout_10->setStretch(0, 200);
        horizontalLayout_10->setStretch(1, 1);

        verticalLayout_11->addLayout(horizontalLayout_10);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(12);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(-1, 0, -1, -1);
        tableWidgetMTheta = new CustomTableWidget(Colors);
        tableWidgetMTheta->setObjectName(QStringLiteral("tableWidgetMTheta"));
        sizePolicy.setHeightForWidth(tableWidgetMTheta->sizePolicy().hasHeightForWidth());
        tableWidgetMTheta->setSizePolicy(sizePolicy);

        horizontalLayout_13->addWidget(tableWidgetMTheta);

        tableWidgetMPhi = new CustomTableWidget(Colors);
        tableWidgetMPhi->setObjectName(QStringLiteral("tableWidgetMPhi"));
        sizePolicy.setHeightForWidth(tableWidgetMPhi->sizePolicy().hasHeightForWidth());
        tableWidgetMPhi->setSizePolicy(sizePolicy);

        horizontalLayout_13->addWidget(tableWidgetMPhi);

        horizontalLayout_13->setStretch(0, 1);
        horizontalLayout_13->setStretch(1, 1);

        verticalLayout_11->addLayout(horizontalLayout_13);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(5);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(-1, 0, -1, 0);
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(-1, -1, -1, 0);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        labelCurrentValueRange = new QLabel(Colors);
        labelCurrentValueRange->setObjectName(QStringLiteral("labelCurrentValueRange"));

        horizontalLayout_6->addWidget(labelCurrentValueRange);

        horizontalSpacer_5 = new QSpacerItem(178, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout_8->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        tableWidgetCurrentMin = new CustomTableWidget(Colors);
        tableWidgetCurrentMin->setObjectName(QStringLiteral("tableWidgetCurrentMin"));
        sizePolicy.setHeightForWidth(tableWidgetCurrentMin->sizePolicy().hasHeightForWidth());
        tableWidgetCurrentMin->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(tableWidgetCurrentMin);

        tableWidgetCurrentMax = new CustomTableWidget(Colors);
        tableWidgetCurrentMax->setObjectName(QStringLiteral("tableWidgetCurrentMax"));
        sizePolicy.setHeightForWidth(tableWidgetCurrentMax->sizePolicy().hasHeightForWidth());
        tableWidgetCurrentMax->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(tableWidgetCurrentMax);


        verticalLayout_8->addLayout(horizontalLayout_7);


        verticalLayout_6->addLayout(verticalLayout_8);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(-1, -1, -1, 0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        labelChooseRange = new QLabel(Colors);
        labelChooseRange->setObjectName(QStringLiteral("labelChooseRange"));

        horizontalLayout_8->addWidget(labelChooseRange);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_6);


        verticalLayout_7->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        tableWidgetChosenMin = new CustomTableWidget(Colors);
        tableWidgetChosenMin->setObjectName(QStringLiteral("tableWidgetChosenMin"));
        sizePolicy.setHeightForWidth(tableWidgetChosenMin->sizePolicy().hasHeightForWidth());
        tableWidgetChosenMin->setSizePolicy(sizePolicy);

        horizontalLayout_9->addWidget(tableWidgetChosenMin);

        tableWidgetChosenMax = new CustomTableWidget(Colors);
        tableWidgetChosenMax->setObjectName(QStringLiteral("tableWidgetChosenMax"));
        sizePolicy.setHeightForWidth(tableWidgetChosenMax->sizePolicy().hasHeightForWidth());
        tableWidgetChosenMax->setSizePolicy(sizePolicy);

        horizontalLayout_9->addWidget(tableWidgetChosenMax);


        verticalLayout_7->addLayout(horizontalLayout_9);

        verticalLayout_7->setStretch(0, 1);
        verticalLayout_7->setStretch(1, 1);

        verticalLayout_6->addLayout(verticalLayout_7);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        tableWidgetColorMap = new CustomTableWidget(Colors);
        tableWidgetColorMap->setObjectName(QStringLiteral("tableWidgetColorMap"));
        sizePolicy.setHeightForWidth(tableWidgetColorMap->sizePolicy().hasHeightForWidth());
        tableWidgetColorMap->setSizePolicy(sizePolicy);

        verticalLayout_5->addWidget(tableWidgetColorMap);

        verticalLayout_5->setStretch(0, 9);

        verticalLayout_6->addLayout(verticalLayout_5);

        verticalLayout_6->setStretch(0, 2);
        verticalLayout_6->setStretch(1, 2);
        verticalLayout_6->setStretch(2, 7);

        verticalLayout_11->addLayout(verticalLayout_6);

        verticalLayout_11->setStretch(0, 1);
        verticalLayout_11->setStretch(1, 7);
        verticalLayout_11->setStretch(2, 1);
        verticalLayout_11->setStretch(3, 12);

        verticalLayout_4->addLayout(verticalLayout_11);

        verticalLayout_4->setStretch(0, 2);
        verticalLayout_4->setStretch(1, 5);
        verticalLayout_4->setStretch(2, 2);
        verticalLayout_4->setStretch(3, 35);

        horizontalLayout_11->addLayout(verticalLayout_4);

        horizontalLayout_11->setStretch(0, 8);

        verticalLayout_9->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(0);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, 0, -1, -1);
        pushButton = new QPushButton(Colors);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        horizontalLayout_12->addWidget(pushButton);

        pushButtonOk = new QPushButton(Colors);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonOk"));
        sizePolicy.setHeightForWidth(pushButtonOk->sizePolicy().hasHeightForWidth());
        pushButtonOk->setSizePolicy(sizePolicy);

        horizontalLayout_12->addWidget(pushButtonOk);


        verticalLayout_9->addLayout(horizontalLayout_12);

        verticalLayout_9->setStretch(0, 50);
        verticalLayout_9->setStretch(1, 2);

        gridLayout_2->addLayout(verticalLayout_9, 0, 0, 1, 1);


        retranslateUi(Colors);
        QObject::connect(pushButtonOk, SIGNAL(released()), Colors, SLOT(on_apply_and_close_button()));
        QObject::connect(pushButton, SIGNAL(released()), Colors, SLOT(on_apply_button()));

        QMetaObject::connectSlotsByName(Colors);
    } // setupUi

    void retranslateUi(QDialog *Colors)
    {
        Colors->setWindowTitle(QApplication::translate("Colors", "Colors", Q_NULLPTR));
        labelBackgroundColor->setText(QApplication::translate("Colors", "background color", Q_NULLPTR));
        checkBoxSpinVectors->setText(QApplication::translate("Colors", "show spin vectors", Q_NULLPTR));
        pushButton_addSpins->setText(QApplication::translate("Colors", "add", Q_NULLPTR));
        pushButton_removeSpins->setText(QApplication::translate("Colors", "remove", Q_NULLPTR));
        checkBoxSpinVectorSingleColor->setText(QApplication::translate("Colors", "single color", Q_NULLPTR));
        checkBoxSpinVectorRGB->setText(QApplication::translate("Colors", "rgb scale", Q_NULLPTR));
        label->setText(QApplication::translate("Colors", "color map", Q_NULLPTR));
        labelCurrentValueRange->setText(QApplication::translate("Colors", "current value range", Q_NULLPTR));
        labelChooseRange->setText(QApplication::translate("Colors", "choose range for color coding", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Colors", "Apply", Q_NULLPTR));
        pushButtonOk->setText(QApplication::translate("Colors", "Apply and Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Colors: public Ui_Colors {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORS_H
