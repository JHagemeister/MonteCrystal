/*
* GUISimulationProcedureElements.h
*
*      
*/

#ifndef GUISIMULATIONPROCEDUREELEMENTS_H
#define GUISIMULATIONPROCEDUREELEMENTS_H

#include <QObject>
#include <QSharedPointer>

#include "typedefs.h"

class Configuration;
class MainWindow;

/// Management of GUI elements concerning parameters of simulation procedure

class GUISimulationProcedureElements : public QObject
{
	Q_OBJECT
public:
	GUISimulationProcedureElements(MainWindow* mw);
	~GUISimulationProcedureElements();

	void setup_elements(void); ///< setup GUI elements for parameters of simulation procedure
	void read_parameters(const std::shared_ptr<Configuration> &config); ///< simulation procedure parameters from GUI

	public slots:
	void change_in_magnetic_field_table(int row, int column);
	void change_in_temperature_table(int row, int column);
	void change_in_simulation_table(int row, int column);
	void change_in_simulation_table_two(int row, int column);
	void change_in_ui_update_table(int row, int column);
	void change_in_simulation_type(QString qString);
	void change_in_program_type(QString qString);

protected:
	MainWindow* _mw; ///< ui elements

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


