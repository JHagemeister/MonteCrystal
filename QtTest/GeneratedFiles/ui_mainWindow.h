/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "CustomTableWidget.h"
#include "oglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_QtMainWindow
{
public:
    QAction *actionsave;
    QAction *actionexit;
    QAction *actionClose;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButtonWorkfolder;
    QComboBox *comboBoxProgramType;
    QComboBox *comboBoxLatticeType;
    QComboBox *comboBoxBoundaryConditions;
    CustomTableWidget *tableWidgetLatticeDimensions;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButtonRandomSpin;
    QPushButton *pushButtonFerromagnet;
    QPushButton *pushButtonSpinSpiral;
    QPushButton *pushButtonSkyrmions;
    CustomTableWidget *tableWidgetExchEnergies1;
    CustomTableWidget *tableWidgetExchEnergies2;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButtonNeel;
    QPushButton *pushButtonChiral;
    CustomTableWidget *tableWidgetDMEnergy;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *checkBoxDipol;
    CustomTableWidget *tableWidgetMagneticMoment;
    QPushButton *pushButtonAnisotropy;
    CustomTableWidget *tableWidgetMagneticField;
    CustomTableWidget *tableWidgetTemperature;
    QComboBox *comboBoxSimulationType;
    CustomTableWidget *tableWidgetSimulation;
    QHBoxLayout *horizontalLayout_5;
    CustomTableWidget *tableWidgetUIUpdate;
    CustomTableWidget *tableWidgetSimulation2;
    QGridLayout *gridLayoutOutput;
    QHBoxLayout *horizontalLayout_6;
    QTextEdit *textEditMovie;
    CustomTableWidget *tableWidgetMovie;
    QPushButton *pushButtonStartStop;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QTextEdit *textEditSimulationInfo;
    QTextEdit *textEditConvergence;
    QTextEdit *textEditSimulationStep;
    QTextEdit *textEditColorMap;
    QPushButton *pushButton;
    OGLWidget *openGLWidget;

    void setupUi(QMainWindow *QtMainWindow)
    {
        if (QtMainWindow->objectName().isEmpty())
            QtMainWindow->setObjectName(QStringLiteral("QtMainWindow"));
        QtMainWindow->setWindowModality(Qt::ApplicationModal);
        QtMainWindow->resize(2560, 1300);
        QtMainWindow->setStyleSheet(QLatin1String("QMainWindow{\n"
"background:  rgb(242, 242, 242);\n"
"}\n"
"\n"
"QComboBox{\n"
"background:  rgb(242, 242, 242);\n"
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
"background:  rgb(242, 242, 242);\n"
"}\n"
"\n"
"QTableWidget {\n"
"    background: rgb(255, 254, 242);\n"
"border: none;\n"
"}\n"
"\n"
"QHeaderView::section{\n"
"background-color:  rgb(232, 232, 232);\n"
"border: 1px solid rgb(255, 254, 242);\n"
"}\n"
"\n"
"QTableView::item:hover {\n"
"    background-color: rgb(255, 255, 191,255);\n"
"}   \n"
"\n"
"QTableWidget::item{ \n"
"selection-background-color: rgb(255, 254, 242);\n"
"}\n"
"\n"
"QTextEdit{\n"
"background:rgb(242, 242, 242);\n"
"border: none;\n"
"}\n"
""));
        actionsave = new QAction(QtMainWindow);
        actionsave->setObjectName(QStringLiteral("actionsave"));
        actionexit = new QAction(QtMainWindow);
        actionexit->setObjectName(QStringLiteral("actionexit"));
        actionClose = new QAction(QtMainWindow);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        centralWidget = new QWidget(QtMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_8 = new QHBoxLayout(centralWidget);
        horizontalLayout_8->setSpacing(10);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pushButtonWorkfolder = new QPushButton(centralWidget);
        pushButtonWorkfolder->setObjectName(QStringLiteral("pushButtonWorkfolder"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButtonWorkfolder->sizePolicy().hasHeightForWidth());
        pushButtonWorkfolder->setSizePolicy(sizePolicy);
        pushButtonWorkfolder->setStyleSheet(QStringLiteral(""));
        pushButtonWorkfolder->setFlat(false);

        verticalLayout->addWidget(pushButtonWorkfolder);

        comboBoxProgramType = new QComboBox(centralWidget);
        comboBoxProgramType->setObjectName(QStringLiteral("comboBoxProgramType"));
        sizePolicy.setHeightForWidth(comboBoxProgramType->sizePolicy().hasHeightForWidth());
        comboBoxProgramType->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(comboBoxProgramType);

        comboBoxLatticeType = new QComboBox(centralWidget);
        comboBoxLatticeType->setObjectName(QStringLiteral("comboBoxLatticeType"));
        sizePolicy.setHeightForWidth(comboBoxLatticeType->sizePolicy().hasHeightForWidth());
        comboBoxLatticeType->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(comboBoxLatticeType);

        comboBoxBoundaryConditions = new QComboBox(centralWidget);
        comboBoxBoundaryConditions->setObjectName(QStringLiteral("comboBoxBoundaryConditions"));
        sizePolicy.setHeightForWidth(comboBoxBoundaryConditions->sizePolicy().hasHeightForWidth());
        comboBoxBoundaryConditions->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(comboBoxBoundaryConditions);

        tableWidgetLatticeDimensions = new CustomTableWidget(centralWidget);
        tableWidgetLatticeDimensions->setObjectName(QStringLiteral("tableWidgetLatticeDimensions"));
        sizePolicy.setHeightForWidth(tableWidgetLatticeDimensions->sizePolicy().hasHeightForWidth());
        tableWidgetLatticeDimensions->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(tableWidgetLatticeDimensions);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 0, -1, -1);
        pushButtonRandomSpin = new QPushButton(centralWidget);
        pushButtonRandomSpin->setObjectName(QStringLiteral("pushButtonRandomSpin"));
        sizePolicy.setHeightForWidth(pushButtonRandomSpin->sizePolicy().hasHeightForWidth());
        pushButtonRandomSpin->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(pushButtonRandomSpin);

        pushButtonFerromagnet = new QPushButton(centralWidget);
        pushButtonFerromagnet->setObjectName(QStringLiteral("pushButtonFerromagnet"));
        sizePolicy.setHeightForWidth(pushButtonFerromagnet->sizePolicy().hasHeightForWidth());
        pushButtonFerromagnet->setSizePolicy(sizePolicy);
        pushButtonFerromagnet->setStyleSheet(QStringLiteral(""));

        horizontalLayout_3->addWidget(pushButtonFerromagnet);

        pushButtonSpinSpiral = new QPushButton(centralWidget);
        pushButtonSpinSpiral->setObjectName(QStringLiteral("pushButtonSpinSpiral"));
        sizePolicy.setHeightForWidth(pushButtonSpinSpiral->sizePolicy().hasHeightForWidth());
        pushButtonSpinSpiral->setSizePolicy(sizePolicy);
        pushButtonSpinSpiral->setStyleSheet(QStringLiteral(""));

        horizontalLayout_3->addWidget(pushButtonSpinSpiral);

        pushButtonSkyrmions = new QPushButton(centralWidget);
        pushButtonSkyrmions->setObjectName(QStringLiteral("pushButtonSkyrmions"));
        sizePolicy.setHeightForWidth(pushButtonSkyrmions->sizePolicy().hasHeightForWidth());
        pushButtonSkyrmions->setSizePolicy(sizePolicy);
        pushButtonSkyrmions->setStyleSheet(QStringLiteral(""));

        horizontalLayout_3->addWidget(pushButtonSkyrmions);

        horizontalLayout_3->setStretch(0, 14);
        horizontalLayout_3->setStretch(1, 6);
        horizontalLayout_3->setStretch(2, 15);
        horizontalLayout_3->setStretch(3, 16);

        verticalLayout->addLayout(horizontalLayout_3);

        tableWidgetExchEnergies1 = new CustomTableWidget(centralWidget);
        tableWidgetExchEnergies1->setObjectName(QStringLiteral("tableWidgetExchEnergies1"));
        sizePolicy.setHeightForWidth(tableWidgetExchEnergies1->sizePolicy().hasHeightForWidth());
        tableWidgetExchEnergies1->setSizePolicy(sizePolicy);
        tableWidgetExchEnergies1->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(tableWidgetExchEnergies1);

        tableWidgetExchEnergies2 = new CustomTableWidget(centralWidget);
        tableWidgetExchEnergies2->setObjectName(QStringLiteral("tableWidgetExchEnergies2"));
        sizePolicy.setHeightForWidth(tableWidgetExchEnergies2->sizePolicy().hasHeightForWidth());
        tableWidgetExchEnergies2->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(tableWidgetExchEnergies2);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(0);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(-1, 0, -1, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, -1, 0, -1);
        pushButtonNeel = new QPushButton(centralWidget);
        pushButtonNeel->setObjectName(QStringLiteral("pushButtonNeel"));
        sizePolicy.setHeightForWidth(pushButtonNeel->sizePolicy().hasHeightForWidth());
        pushButtonNeel->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(pushButtonNeel);

        pushButtonChiral = new QPushButton(centralWidget);
        pushButtonChiral->setObjectName(QStringLiteral("pushButtonChiral"));
        sizePolicy.setHeightForWidth(pushButtonChiral->sizePolicy().hasHeightForWidth());
        pushButtonChiral->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(pushButtonChiral);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 1);

        horizontalLayout_9->addLayout(verticalLayout_3);

        tableWidgetDMEnergy = new CustomTableWidget(centralWidget);
        tableWidgetDMEnergy->setObjectName(QStringLiteral("tableWidgetDMEnergy"));
        sizePolicy.setHeightForWidth(tableWidgetDMEnergy->sizePolicy().hasHeightForWidth());
        tableWidgetDMEnergy->setSizePolicy(sizePolicy);

        horizontalLayout_9->addWidget(tableWidgetDMEnergy);

        horizontalLayout_9->setStretch(0, 2);
        horizontalLayout_9->setStretch(1, 9);

        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(5, -1, -1, -1);
        checkBoxDipol = new QCheckBox(centralWidget);
        checkBoxDipol->setObjectName(QStringLiteral("checkBoxDipol"));
        sizePolicy.setHeightForWidth(checkBoxDipol->sizePolicy().hasHeightForWidth());
        checkBoxDipol->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(checkBoxDipol);

        tableWidgetMagneticMoment = new CustomTableWidget(centralWidget);
        tableWidgetMagneticMoment->setObjectName(QStringLiteral("tableWidgetMagneticMoment"));
        sizePolicy.setHeightForWidth(tableWidgetMagneticMoment->sizePolicy().hasHeightForWidth());
        tableWidgetMagneticMoment->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(tableWidgetMagneticMoment);

        pushButtonAnisotropy = new QPushButton(centralWidget);
        pushButtonAnisotropy->setObjectName(QStringLiteral("pushButtonAnisotropy"));
        sizePolicy.setHeightForWidth(pushButtonAnisotropy->sizePolicy().hasHeightForWidth());
        pushButtonAnisotropy->setSizePolicy(sizePolicy);
        pushButtonAnisotropy->setStyleSheet(QStringLiteral(""));

        horizontalLayout_4->addWidget(pushButtonAnisotropy);

        horizontalLayout_4->setStretch(0, 13);
        horizontalLayout_4->setStretch(1, 19);
        horizontalLayout_4->setStretch(2, 18);

        verticalLayout->addLayout(horizontalLayout_4);

        tableWidgetMagneticField = new CustomTableWidget(centralWidget);
        tableWidgetMagneticField->setObjectName(QStringLiteral("tableWidgetMagneticField"));
        sizePolicy.setHeightForWidth(tableWidgetMagneticField->sizePolicy().hasHeightForWidth());
        tableWidgetMagneticField->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(tableWidgetMagneticField);

        tableWidgetTemperature = new CustomTableWidget(centralWidget);
        tableWidgetTemperature->setObjectName(QStringLiteral("tableWidgetTemperature"));
        sizePolicy.setHeightForWidth(tableWidgetTemperature->sizePolicy().hasHeightForWidth());
        tableWidgetTemperature->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(tableWidgetTemperature);

        comboBoxSimulationType = new QComboBox(centralWidget);
        comboBoxSimulationType->setObjectName(QStringLiteral("comboBoxSimulationType"));
        sizePolicy.setHeightForWidth(comboBoxSimulationType->sizePolicy().hasHeightForWidth());
        comboBoxSimulationType->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(comboBoxSimulationType);

        tableWidgetSimulation = new CustomTableWidget(centralWidget);
        tableWidgetSimulation->setObjectName(QStringLiteral("tableWidgetSimulation"));
        sizePolicy.setHeightForWidth(tableWidgetSimulation->sizePolicy().hasHeightForWidth());
        tableWidgetSimulation->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(tableWidgetSimulation);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        tableWidgetUIUpdate = new CustomTableWidget(centralWidget);
        tableWidgetUIUpdate->setObjectName(QStringLiteral("tableWidgetUIUpdate"));
        sizePolicy.setHeightForWidth(tableWidgetUIUpdate->sizePolicy().hasHeightForWidth());
        tableWidgetUIUpdate->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(tableWidgetUIUpdate);

        tableWidgetSimulation2 = new CustomTableWidget(centralWidget);
        tableWidgetSimulation2->setObjectName(QStringLiteral("tableWidgetSimulation2"));
        sizePolicy.setHeightForWidth(tableWidgetSimulation2->sizePolicy().hasHeightForWidth());
        tableWidgetSimulation2->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(tableWidgetSimulation2);

        horizontalLayout_5->setStretch(0, 100);
        horizontalLayout_5->setStretch(1, 250);

        verticalLayout->addLayout(horizontalLayout_5);

        gridLayoutOutput = new QGridLayout();
        gridLayoutOutput->setSpacing(6);
        gridLayoutOutput->setObjectName(QStringLiteral("gridLayoutOutput"));
        gridLayoutOutput->setSizeConstraint(QLayout::SetNoConstraint);
        gridLayoutOutput->setContentsMargins(5, 2, -1, 2);

        verticalLayout->addLayout(gridLayoutOutput);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(-1, -1, -1, 6);
        textEditMovie = new QTextEdit(centralWidget);
        textEditMovie->setObjectName(QStringLiteral("textEditMovie"));
        sizePolicy.setHeightForWidth(textEditMovie->sizePolicy().hasHeightForWidth());
        textEditMovie->setSizePolicy(sizePolicy);

        horizontalLayout_6->addWidget(textEditMovie);

        tableWidgetMovie = new CustomTableWidget(centralWidget);
        tableWidgetMovie->setObjectName(QStringLiteral("tableWidgetMovie"));
        sizePolicy.setHeightForWidth(tableWidgetMovie->sizePolicy().hasHeightForWidth());
        tableWidgetMovie->setSizePolicy(sizePolicy);

        horizontalLayout_6->addWidget(tableWidgetMovie);

        horizontalLayout_6->setStretch(0, 85);
        horizontalLayout_6->setStretch(1, 340);

        verticalLayout->addLayout(horizontalLayout_6);

        pushButtonStartStop = new QPushButton(centralWidget);
        pushButtonStartStop->setObjectName(QStringLiteral("pushButtonStartStop"));
        sizePolicy.setHeightForWidth(pushButtonStartStop->sizePolicy().hasHeightForWidth());
        pushButtonStartStop->setSizePolicy(sizePolicy);
        pushButtonStartStop->setStyleSheet(QLatin1String("QPushButton {\n"
"color: #333;\n"
"border: 2px solid #555;\n"
"padding: 5px;\n"
"background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(200, 200, 200, 255), stop:1 rgba(170, 170, 170, 255));\n"
"}"));

        verticalLayout->addWidget(pushButtonStartStop);

        verticalLayout->setStretch(0, 41);
        verticalLayout->setStretch(1, 41);
        verticalLayout->setStretch(2, 41);
        verticalLayout->setStretch(3, 41);
        verticalLayout->setStretch(4, 81);
        verticalLayout->setStretch(5, 41);
        verticalLayout->setStretch(6, 81);
        verticalLayout->setStretch(7, 86);
        verticalLayout->setStretch(8, 81);
        verticalLayout->setStretch(9, 50);
        verticalLayout->setStretch(10, 91);
        verticalLayout->setStretch(11, 86);
        verticalLayout->setStretch(12, 41);
        verticalLayout->setStretch(13, 81);
        verticalLayout->setStretch(14, 86);
        verticalLayout->setStretch(15, 140);
        verticalLayout->setStretch(16, 81);
        verticalLayout->setStretch(17, 41);

        horizontalLayout_8->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, -1, -1, -1);
        textEditSimulationInfo = new QTextEdit(centralWidget);
        textEditSimulationInfo->setObjectName(QStringLiteral("textEditSimulationInfo"));
        sizePolicy.setHeightForWidth(textEditSimulationInfo->sizePolicy().hasHeightForWidth());
        textEditSimulationInfo->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(textEditSimulationInfo);

        textEditConvergence = new QTextEdit(centralWidget);
        textEditConvergence->setObjectName(QStringLiteral("textEditConvergence"));
        sizePolicy.setHeightForWidth(textEditConvergence->sizePolicy().hasHeightForWidth());
        textEditConvergence->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(textEditConvergence);

        textEditSimulationStep = new QTextEdit(centralWidget);
        textEditSimulationStep->setObjectName(QStringLiteral("textEditSimulationStep"));
        sizePolicy.setHeightForWidth(textEditSimulationStep->sizePolicy().hasHeightForWidth());
        textEditSimulationStep->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(textEditSimulationStep);

        textEditColorMap = new QTextEdit(centralWidget);
        textEditColorMap->setObjectName(QStringLiteral("textEditColorMap"));
        sizePolicy.setHeightForWidth(textEditColorMap->sizePolicy().hasHeightForWidth());
        textEditColorMap->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(textEditColorMap);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(pushButton);

        horizontalLayout_7->setStretch(0, 12);
        horizontalLayout_7->setStretch(1, 12);
        horizontalLayout_7->setStretch(2, 12);
        horizontalLayout_7->setStretch(3, 12);
        horizontalLayout_7->setStretch(4, 2);

        verticalLayout_2->addLayout(horizontalLayout_7);

        openGLWidget = new OGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(openGLWidget);

        verticalLayout_2->setStretch(0, 4);
        verticalLayout_2->setStretch(1, 100);

        horizontalLayout_8->addLayout(verticalLayout_2);

        horizontalLayout_8->setStretch(0, 10);
        horizontalLayout_8->setStretch(1, 65);
        QtMainWindow->setCentralWidget(centralWidget);

        retranslateUi(QtMainWindow);
        QObject::connect(pushButtonStartStop, SIGNAL(released()), QtMainWindow, SLOT(start_stop_simulation()));
        QObject::connect(actionClose, SIGNAL(triggered()), QtMainWindow, SLOT(close()));
        QObject::connect(pushButtonAnisotropy, SIGNAL(released()), QtMainWindow, SLOT(on_anisotropy_button()));
        QObject::connect(comboBoxLatticeType, SIGNAL(activated(QString)), QtMainWindow, SLOT(on_lattice_type_selected(QString)));
        QObject::connect(comboBoxProgramType, SIGNAL(activated(QString)), QtMainWindow, SLOT(on_program_type_selected(QString)));
        QObject::connect(pushButtonSpinSpiral, SIGNAL(released()), QtMainWindow, SLOT(on_push_button_spin_spiral()));
        QObject::connect(pushButtonSkyrmions, SIGNAL(released()), QtMainWindow, SLOT(on_push_button_skyrmions()));
        QObject::connect(pushButtonFerromagnet, SIGNAL(released()), QtMainWindow, SLOT(on_push_button_ferromagnet()));
        QObject::connect(pushButtonWorkfolder, SIGNAL(released()), QtMainWindow, SLOT(on_push_button_workfolder()));
        QObject::connect(pushButtonRandomSpin, SIGNAL(released()), QtMainWindow, SLOT(on_push_button_random_spin()));
        QObject::connect(pushButton, SIGNAL(released()), QtMainWindow, SLOT(on_push_button_colors()));

        QMetaObject::connectSlotsByName(QtMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *QtMainWindow)
    {
        QtMainWindow->setWindowTitle(QApplication::translate("QtMainWindow", "Monte Crystal", Q_NULLPTR));
        actionsave->setText(QApplication::translate("QtMainWindow", "save", Q_NULLPTR));
        actionexit->setText(QApplication::translate("QtMainWindow", "exit", Q_NULLPTR));
        actionClose->setText(QApplication::translate("QtMainWindow", "Close", Q_NULLPTR));
        pushButtonWorkfolder->setText(QApplication::translate("QtMainWindow", "workfolder", Q_NULLPTR));
        pushButtonRandomSpin->setText(QApplication::translate("QtMainWindow", "Random", Q_NULLPTR));
        pushButtonFerromagnet->setText(QApplication::translate("QtMainWindow", "FM", Q_NULLPTR));
        pushButtonSpinSpiral->setText(QApplication::translate("QtMainWindow", "SpinSpiral", Q_NULLPTR));
        pushButtonSkyrmions->setText(QApplication::translate("QtMainWindow", "Skyrmions", Q_NULLPTR));
        pushButtonNeel->setText(QApplication::translate("QtMainWindow", "Neel", Q_NULLPTR));
        pushButtonChiral->setText(QApplication::translate("QtMainWindow", "Chiral", Q_NULLPTR));
        checkBoxDipol->setText(QApplication::translate("QtMainWindow", "Dipol", Q_NULLPTR));
        pushButtonAnisotropy->setText(QApplication::translate("QtMainWindow", "Anisotropies", Q_NULLPTR));
        textEditMovie->setHtml(QApplication::translate("QtMainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:7.875pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">movie</p></body></html>", Q_NULLPTR));
        pushButtonStartStop->setText(QApplication::translate("QtMainWindow", "Start", Q_NULLPTR));
        textEditSimulationStep->setHtml(QApplication::translate("QtMainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:7.875pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", Q_NULLPTR));
        pushButton->setText(QApplication::translate("QtMainWindow", "Colors", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QtMainWindow: public Ui_QtMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
