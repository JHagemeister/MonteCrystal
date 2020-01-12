/*
* openglwidget.h
*
* 
*
*/

#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include "ui_openglwidget.h"

class OpenGLWidget : public QWidget, public Ui::OpenGLWidget {
	Q_OBJECT

public:
	OpenGLWidget(QWidget * parent = Q_NULLPTR);
};

#endif //TOOLBARWIDGET_H
