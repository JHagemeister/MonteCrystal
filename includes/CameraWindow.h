#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QDialog>

#include "typedefs.h"

#include "ui_camera.h"

class OGLWidget;

/// Window to setup ferromagnetic state.
class CameraWindow : public QDialog {
	Q_OBJECT

public:
    CameraWindow( OGLWidget* oglWidget,QWidget * parent = Q_NULLPTR);
    ~CameraWindow();

public slots:
    void push_button_apply(void);
    void check_base(void);
    void close(void);

signals:
    void send_repaint_request(void);

private:
    void read_parameters(void);
    OGLWidget* _oglWidget; ///< oglWidget object
    Ui::CameraWindow _ui;


};

#endif // CAMERAWINDOW_H
