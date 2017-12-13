/*
* GUISimulationProcedureElements.h
*
*      
*/

#ifndef GUISIMULATIONPROCEDUREELEMENTS_H
#define GUISIMULATIONPROCEDUREELEMENTS_H

// Form include
#include "ui_mainWindow.h"

#include <memory>

#include "qobject.h"
#include "qsharedpointer.h"

class Configuration;

/// Management of GUI elements concerning parameters of simulation procedure

class GUISimulationProcedureElements : public QObject
{
	Q_OBJECT
public:
	GUISimulationProcedureElements(Ui::QtMainWindow* ui);
	~GUISimulationProcedureElements();

	void setup_elements(void); ///< setup GUI elements for parameters of simulation procedure
	void read_parameters(QSharedPointer<Configuration> &config); ///< simulation procedure parameters from GUI

	public slots:
	void on_change_in_magnetic_field_table(int row, int column);
	void on_change_in_temperature_table(int row, int column);
	void on_change_in_simulation_table(int row, int column);
	void on_change_in_simulation_table_two(int row, int column);
	void on_change_in_ui_update_table(int row, int column);
	void on_change_in_simulation_type(QString qString);
	void on_change_in_program_type(QString qString);

protected:
	Ui::QtMainWindow* _ui; ///< ui elements

	void set_gui_magnetic_field_elements(QString qString);

	void set_gui_temperature_elements(QString qString);

	MagneticFieldStruct* _llgMagneticFieldParameters;
	MagneticFieldStruct* _monteCarloMagneticFieldParameters;
	MagneticFieldStruct* _converger1MagneticFieldParameters;

	TemperatureStruct* _llgTemperature;
	TemperatureStruct* _monteCarloTemperature;
	TemperatureStruct* _converger1Temperature;

	SimulationProcedureParameters* _llgSimParameters;
	SimulationProcedureParameters* _monteCarloSimParameters;
	SimulationProcedureParameters* _converger1SimParameters;
};

#endif // GUISIMULATIONPROCEDUREELEMENTS_H


