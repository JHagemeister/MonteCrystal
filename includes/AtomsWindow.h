#ifndef ATOMSWINDOW_H
#define ATOMSWINDOW_H

#include <QDialog>
#include <QColorDialog>
#include <QStringList>
#include "typedefs.h"
#include <glm/vec3.hpp>
#include "ui_atoms.h"

#include "vector"

class OGLWidget;

/// Window to setup ferromagnetic state.
class AtomsWindow : public QDialog {
	Q_OBJECT

public:
    AtomsWindow( OGLWidget* oglWidget,QWidget * parent = Q_NULLPTR);
    ~AtomsWindow();

    int sectors = 10;
    float radius =0.4;
public slots:
    void read_parameters(void);
    void check_atoms(void);
signals:
    void send_repaint_request(void);

private:

    void chooseMinColor(void);
    void chooseMaxColor(void);
    QStringList getStoredCmaps(void);
    OGLWidget* _oglWidget; ///< oglWidget object
    Ui::AtomsWindow _ui;
    QColorDialog _colorPicker;
    QColor defaultColor;

};

#endif // ATOMSWINDOW_H
