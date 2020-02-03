/*
* GUIOutputElements.h
*
*      
*/

#ifndef GUIOUTPUTELEMENTS_H
#define GUIOUTPUTELEMENTS_H

#include <QObject>
#include <QSharedPointer>

class MainWindow;
class Configuration;
class QCheckBox;

/// Management of GUI elements concerning output parameters

class GUIOutputElements : public QObject
{
	Q_OBJECT
public:
	GUIOutputElements(MainWindow* ui);

	void setup_elements(void); ///< setup GUI elements for output parameters
	void set_default_values(void); ///< set default output parameter values
	void read_parameters(const std::shared_ptr<Configuration> &config); ///< output parameters from GUI

protected:
	MainWindow* _mw; ///< ui elements

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


