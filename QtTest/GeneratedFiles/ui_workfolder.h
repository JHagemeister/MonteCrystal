/********************************************************************************
** Form generated from reading UI file 'workfolder.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORKFOLDER_H
#define UI_WORKFOLDER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Workfolder
{
public:
    QVBoxLayout *verticalLayout_3;
    QFrame *workfolderFrame;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonOk;
    QPushButton *pushButtonSelectOther;

    void setupUi(QWidget *Workfolder)
    {
        if (Workfolder->objectName().isEmpty())
            Workfolder->setObjectName(QStringLiteral("Workfolder"));
        Workfolder->resize(866, 165);
        Workfolder->setStyleSheet(QLatin1String("QWidget#workfolderFrame{\n"
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
        verticalLayout_3 = new QVBoxLayout(Workfolder);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        workfolderFrame = new QFrame(Workfolder);
        workfolderFrame->setObjectName(QStringLiteral("workfolderFrame"));
        workfolderFrame->setFrameShape(QFrame::StyledPanel);
        workfolderFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(workfolderFrame);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(12, 12, 12, 12);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        textEdit = new QTextEdit(workfolderFrame);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(textEdit);

        horizontalLayout_2->setStretch(0, 120);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButtonOk = new QPushButton(workfolderFrame);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonOk"));
        sizePolicy.setHeightForWidth(pushButtonOk->sizePolicy().hasHeightForWidth());
        pushButtonOk->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonOk);

        pushButtonSelectOther = new QPushButton(workfolderFrame);
        pushButtonSelectOther->setObjectName(QStringLiteral("pushButtonSelectOther"));
        sizePolicy.setHeightForWidth(pushButtonSelectOther->sizePolicy().hasHeightForWidth());
        pushButtonSelectOther->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButtonSelectOther);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 5);
        verticalLayout->setStretch(1, 3);

        verticalLayout_2->addLayout(verticalLayout);


        verticalLayout_3->addWidget(workfolderFrame);

        QWidget::setTabOrder(pushButtonOk, pushButtonSelectOther);
        QWidget::setTabOrder(pushButtonSelectOther, textEdit);

        retranslateUi(Workfolder);
        QObject::connect(pushButtonOk, SIGNAL(released()), Workfolder, SLOT(close()));
        QObject::connect(pushButtonSelectOther, SIGNAL(released()), Workfolder, SLOT(on_push_button_select_other()));
        QObject::connect(pushButtonSelectOther, SIGNAL(released()), Workfolder, SLOT(close()));

        QMetaObject::connectSlotsByName(Workfolder);
    } // setupUi

    void retranslateUi(QWidget *Workfolder)
    {
        Workfolder->setWindowTitle(QApplication::translate("Workfolder", "Workfolder window", Q_NULLPTR));
        pushButtonOk->setText(QApplication::translate("Workfolder", "Ok", Q_NULLPTR));
        pushButtonSelectOther->setText(QApplication::translate("Workfolder", "Select other", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Workfolder: public Ui_Workfolder {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORKFOLDER_H
