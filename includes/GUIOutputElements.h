/*
* GUIOutputElements.h
*
*      
*/

#ifndef GUIOUTPUTELEMENTS_H
#define GUIOUTPUTELEMENTS_H

// Form include
#include "ui_mainWindow.h"

#include <QObject>
#include <QSharedPointer>


class Configuration;

/// Management of GUI elements concerning output parameters

class GUIOutputElements : public QObject
{
	Q_OBJECT
public:
	GUIOutputElements(Ui::QtMainWindow* ui);

	void setup_elements(void); ///< setup GUI elements for output parameters
	void set_default_values(void); ///< set default output parameter values
	void read_parameters(QSharedPointer<Configuration> &config); ///< output parameters from GUI

protected:
	Ui::QtMainWindow* _ui; ///< ui elements

	// output configurations for simulation program
	QCheckBox* _checkBox_E; ///< energy output
	QCheckBox* _checkBox_M; ///< magnetization output
	QCheckBox* _checkBox_Mabs; ///< absolute magnetization output
	QCheckBox* _checkBox_NCMR;
	QCheckBox* _checkBox_Skn; ///< skyrmion number ouput
	QCheckBox* _checkBox_espin; ///< each spin
	QCheckBox* _checkBox_spinConfig; ///< spin configuration output at end of temperature-field-loop
	QCheckBox* _checkBox_outSimStep; ///< output of measurements values as a function of simulation step
};

#endif // GUIOUTPUTELEMENTS_H


