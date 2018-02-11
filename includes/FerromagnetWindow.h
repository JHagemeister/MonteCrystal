#ifndef FERROMAGNETWINDOW_H
#define FERROMAGNETWINDOW_H

#include <QDialog>

#include "typedefs.h"

#include "ui_ferromagnet.h"

class GUISpinElements;

/// Window to setup ferromagnetic state.
class FerromagnetWindow : public QDialog {
	Q_OBJECT

public:
	FerromagnetWindow(GUISpinElements* guiSpinElements, QWidget * parent = Q_NULLPTR);
	~FerromagnetWindow();

	public slots:
	void push_button_apply(void);
	void push_button_undo(void);

private:
	void read_parameters(void);

	Ui::FerromagnetWindow _ui;

	GUISpinElements* _guiSpinElements;

	Threedim _direction;
};

#endif // FERROMAGNETWINDOW_H