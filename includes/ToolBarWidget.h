/*
* Toolbar.h
*
* 
*
*/

#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>
#include "ui_toolbar.h"

class ToolBarWidget : public QWidget, public Ui::ToolBar {
public:
	ToolBarWidget(QWidget * parent = Q_NULLPTR);
};

#endif //TOOLBARWIDGET_H
