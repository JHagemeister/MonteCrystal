#ifndef STMWINDOW_H
#define STMWINDOW_H

#include <QDialog>
#include <QColorDialog>
#include <QStringList>
#include <glm/vec3.hpp>
#include "ui_stmwindow.h"
#include <QWindow>
#include <QWidget>
#include "OGLWidget.h"
#include "Stm.h"
class OGLWidget;

/// Window to setup STM.
class StmWindow : public QDialog {
    Q_OBJECT

public:
    StmWindow( OGLWidget* oglWidget,QSharedPointer<Lattice> lattice,QWidget * parent = Q_NULLPTR);
    ~StmWindow();
    STM *_stm;

public slots:

    void read_parameters(void);

signals:
    void send_repaint_request(void);

private:
    void getIMG(void);
    void scaleIMG(void);
    void resizeEvent(QResizeEvent* event);
    QImage img;

    Ui::StmWindow _ui;
    OGLWidget* _oglWidget;
    QColorDialog _colorPicker;
    void chooseMinColor(void);
    void chooseMaxColor(void);
    void saveCurrendImg(void);


};

#endif // STMWINDOW_H
