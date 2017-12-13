/*
* GUISpinElements.h
*
*      
*/

#ifndef GUISPINELEMENTS_H
#define GUISPINELEMENTS_H

// Form include
#include "ui_mainWindow.h"

#include "qstring.h"
#include "qobject.h"
#include "qsharedpointer.h"

class Configuration;
class SpinOrientation;
class FerromagnetWindow;
class SkyrmionWindow;
class SpinSpiralWindow;

/// Management of GUI elements concerning lattice parameters

class GUISpinElements: public QObject
{
Q_OBJECT
public:
	GUISpinElements(Ui::QtMainWindow* ui);
	~GUISpinElements();

	void setup_elements(void); ///< setup GUI elements for spin parameters 
	void set_default_values(void); ///< set default values for spin parameters
	void read_parameters(QSharedPointer<Configuration> &config); ///< spin parameters from GUI

	void open_ferromagnet_window(Threedim* latticeCoordArray, Threedim* spinArray, int numberAtoms);

	void open_skyrmion_window(OGLWidget* oglWidget, Threedim* latticeCoordArray, Threedim* spinArray, 
		int numberAtoms);

	void open_spin_spiral_window(OGLWidget* oglWidget, Threedim* latticeCoordArray, Threedim* spinArray,
		int numberAtoms);

	void random_spin_configuration(SpinOrientation* _spinOrientation);
	void ferromagnet(Threedim direction);
	void skyrmion(SkyrmionWindowParameters parameters);
	void spin_spiral(Threedim kVector, Threedim position, int helicity);
	void restore_spin_configuration(void);

	public slots:
	void on_ferromagnet_window_destroyed(void);
	void on_skyrmion_window_destroyed(void);
	void on_spin_spiral_window_destroyed(void);

protected:
	void close_all_spin_state_windows(void);

	Ui::QtMainWindow* _ui; ///< ui elements

	FerromagnetWindow* _ferromagnetWindow;
	SkyrmionWindow* _skyrmionWindow;
	SpinSpiralWindow* _spinSpiralWindow;

	SkyrmionWindowParameters _skyrmionParameters;

	Threedim* _latticeCoordinateArray;
	Threedim* _spinArray;
	Threedim* _spinArrayTemp;
	int _numberAtoms;
};

#endif // GUISPINELEMENTS_H


