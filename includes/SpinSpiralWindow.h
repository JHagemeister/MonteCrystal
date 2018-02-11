#ifndef SPINSPIRALWINDOW_H
#define SPINSPIRALWINDOW_H

#include <QDialog>
#include "ui_spinspiral.h"

#include "typedefs.h"

class GUISpinElements;

/// Window to setup spin spiral state
class SpinSpiralWindow : public QDialog 
{
	Q_OBJECT
public:
	SpinSpiralWindow(GUISpinElements* guiSpinElements, QWidget * parent = Q_NULLPTR);
	~SpinSpiralWindow();

public slots:
	void push_button_apply(void); ///< make change to spin system to setup spin spiral state
	void push_button_undo(void); ///< restore initial spin configuration
	void receive_mouse_position(Twodim position); ///< mouse click position to specify position of spin spiral

private:
	void read_parameters(void); ///< read GUI parameters for spin spiral setup

	Ui::SpinSpiralWindow _ui;

	GUISpinElements* _guiSpinElements;

	Twodim _position; ///< position coordinates
	int _helicity; ///< helicity of spin spiral state, +1 or -1 allowed
	double _lambda; ///< spin spiral period
	Threedim _direction; ///< propagation direction
};

#endif // SPINSPIRALWINDOW_H