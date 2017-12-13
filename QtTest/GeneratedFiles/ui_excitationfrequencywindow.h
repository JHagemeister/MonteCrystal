/********************************************************************************
** Form generated from reading UI file 'excitationfrequencywindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXCITATIONFREQUENCYWINDOW_H
#define UI_EXCITATIONFREQUENCYWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include "CustomTableWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ExcitationFrequencyWindow
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    CustomTableWidget *tableWidgetNumEigStates;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_Eigenvalues;
    QPushButton *pushButton_Eigenvectors;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonNext;
    QPushButton *pushButtonPrevious;
    QPushButton *pushButtonPause;
    QTextEdit *textEditEigInformation;
    CustomTableWidget *tableWidgetAngle;
    CustomTableWidget *tableWidgetSpeed;
    CustomTableWidget *tableWidgetSegmentation;
    QPushButton *pushButtonVideoSeq;
    QPushButton *pushButtonClose;

    void setupUi(QDialog *ExcitationFrequencyWindow)
    {
        if (ExcitationFrequencyWindow->objectName().isEmpty())
            ExcitationFrequencyWindow->setObjectName(QStringLiteral("ExcitationFrequencyWindow"));
        ExcitationFrequencyWindow->resize(386, 638);
        ExcitationFrequencyWindow->setStyleSheet(QLatin1String("QDialog{\n"
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
        gridLayout = new QGridLayout(ExcitationFrequencyWindow);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tableWidgetNumEigStates = new CustomTableWidget(ExcitationFrequencyWindow);
        tableWidgetNumEigStates->setObjectName(QStringLiteral("tableWidgetNumEigStates"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidgetNumEigStates->sizePolicy().hasHeightForWidth());
        tableWidgetNumEigStates->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(tableWidgetNumEigStates);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_Eigenvalues = new QPushButton(ExcitationFrequencyWindow);
        pushButton_Eigenvalues->setObjectName(QStringLiteral("pushButton_Eigenvalues"));
        sizePolicy.setHeightForWidth(pushButton_Eigenvalues->sizePolicy().hasHeightForWidth());
        pushButton_Eigenvalues->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(pushButton_Eigenvalues);

        pushButton_Eigenvectors = new QPushButton(ExcitationFrequencyWindow);
        pushButton_Eigenvectors->setObjectName(QStringLiteral("pushButton_Eigenvectors"));
        sizePolicy.setHeightForWidth(pushButton_Eigenvectors->sizePolicy().hasHeightForWidth());
        pushButton_Eigenvectors->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(pushButton_Eigenvectors);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButtonNext = new QPushButton(ExcitationFrequencyWindow);
        pushButtonNext->setObjectName(QStringLiteral("pushButtonNext"));
        sizePolicy.setHeightForWidth(pushButtonNext->sizePolicy().hasHeightForWidth());
        pushButtonNext->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonNext);

        pushButtonPrevious = new QPushButton(ExcitationFrequencyWindow);
        pushButtonPrevious->setObjectName(QStringLiteral("pushButtonPrevious"));
        sizePolicy.setHeightForWidth(pushButtonPrevious->sizePolicy().hasHeightForWidth());
        pushButtonPrevious->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonPrevious);

        pushButtonPause = new QPushButton(ExcitationFrequencyWindow);
        pushButtonPause->setObjectName(QStringLiteral("pushButtonPause"));
        sizePolicy.setHeightForWidth(pushButtonPause->sizePolicy().hasHeightForWidth());
        pushButtonPause->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonPause);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);

        verticalLayout->addLayout(horizontalLayout);

        textEditEigInformation = new QTextEdit(ExcitationFrequencyWindow);
        textEditEigInformation->setObjectName(QStringLiteral("textEditEigInformation"));
        sizePolicy.setHeightForWidth(textEditEigInformation->sizePolicy().hasHeightForWidth());
        textEditEigInformation->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(textEditEigInformation);


        verticalLayout_2->addLayout(verticalLayout);

        tableWidgetAngle = new CustomTableWidget(ExcitationFrequencyWindow);
        tableWidgetAngle->setObjectName(QStringLiteral("tableWidgetAngle"));
        sizePolicy.setHeightForWidth(tableWidgetAngle->sizePolicy().hasHeightForWidth());
        tableWidgetAngle->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(tableWidgetAngle);

        tableWidgetSpeed = new CustomTableWidget(ExcitationFrequencyWindow);
        tableWidgetSpeed->setObjectName(QStringLiteral("tableWidgetSpeed"));
        sizePolicy.setHeightForWidth(tableWidgetSpeed->sizePolicy().hasHeightForWidth());
        tableWidgetSpeed->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(tableWidgetSpeed);

        tableWidgetSegmentation = new CustomTableWidget(ExcitationFrequencyWindow);
        tableWidgetSegmentation->setObjectName(QStringLiteral("tableWidgetSegmentation"));
        sizePolicy.setHeightForWidth(tableWidgetSegmentation->sizePolicy().hasHeightForWidth());
        tableWidgetSegmentation->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(tableWidgetSegmentation);

        pushButtonVideoSeq = new QPushButton(ExcitationFrequencyWindow);
        pushButtonVideoSeq->setObjectName(QStringLiteral("pushButtonVideoSeq"));
        sizePolicy.setHeightForWidth(pushButtonVideoSeq->sizePolicy().hasHeightForWidth());
        pushButtonVideoSeq->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(pushButtonVideoSeq);

        pushButtonClose = new QPushButton(ExcitationFrequencyWindow);
        pushButtonClose->setObjectName(QStringLiteral("pushButtonClose"));
        sizePolicy.setHeightForWidth(pushButtonClose->sizePolicy().hasHeightForWidth());
        pushButtonClose->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(pushButtonClose);

        verticalLayout_2->setStretch(0, 4);
        verticalLayout_2->setStretch(1, 4);
        verticalLayout_2->setStretch(2, 9);
        verticalLayout_2->setStretch(3, 4);
        verticalLayout_2->setStretch(4, 4);
        verticalLayout_2->setStretch(5, 4);
        verticalLayout_2->setStretch(6, 4);
        verticalLayout_2->setStretch(7, 4);

        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(ExcitationFrequencyWindow);
        QObject::connect(pushButtonNext, SIGNAL(released()), ExcitationFrequencyWindow, SLOT(on_button_next()));
        QObject::connect(pushButtonPrevious, SIGNAL(released()), ExcitationFrequencyWindow, SLOT(on_button_previous()));
        QObject::connect(pushButtonClose, SIGNAL(released()), ExcitationFrequencyWindow, SLOT(stop_visualization()));
        QObject::connect(pushButtonPause, SIGNAL(released()), ExcitationFrequencyWindow, SLOT(on_button_pause()));
        QObject::connect(pushButton_Eigenvalues, SIGNAL(released()), ExcitationFrequencyWindow, SLOT(on_button_eigenvalues()));
        QObject::connect(pushButton_Eigenvectors, SIGNAL(released()), ExcitationFrequencyWindow, SLOT(on_button_eigenvectors()));
        QObject::connect(pushButtonVideoSeq, SIGNAL(released()), ExcitationFrequencyWindow, SLOT(on_button_save_image_sequence()));

        QMetaObject::connectSlotsByName(ExcitationFrequencyWindow);
    } // setupUi

    void retranslateUi(QDialog *ExcitationFrequencyWindow)
    {
        ExcitationFrequencyWindow->setWindowTitle(QApplication::translate("ExcitationFrequencyWindow", "ExcitationFrequencyWindow", Q_NULLPTR));
        pushButton_Eigenvalues->setText(QApplication::translate("ExcitationFrequencyWindow", "read eigenvalues", Q_NULLPTR));
        pushButton_Eigenvectors->setText(QApplication::translate("ExcitationFrequencyWindow", "read eigenvectors", Q_NULLPTR));
        pushButtonNext->setText(QApplication::translate("ExcitationFrequencyWindow", "next", Q_NULLPTR));
        pushButtonPrevious->setText(QApplication::translate("ExcitationFrequencyWindow", "previous", Q_NULLPTR));
        pushButtonPause->setText(QApplication::translate("ExcitationFrequencyWindow", "pause", Q_NULLPTR));
        pushButtonVideoSeq->setText(QApplication::translate("ExcitationFrequencyWindow", "Save image sequence", Q_NULLPTR));
        pushButtonClose->setText(QApplication::translate("ExcitationFrequencyWindow", "close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ExcitationFrequencyWindow: public Ui_ExcitationFrequencyWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXCITATIONFREQUENCYWINDOW_H
