/*
* GUISimulationProcedureElements.cpp
*
* Copyright 2017 Julian Hagemeister
*
* This file is part of MonteCrystal.
*
* MonteCrystal is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* MonteCrystal is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with MonteCrystal.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "GUISimulationProcedureElements.h"

// forward includes
#include "Configuration.h"

#include "MainWindow.h"
#include "MyHeaderView.h"
#include <QTableWidget>
#include <QComboBox>

GUISimulationProcedureElements::GUISimulationProcedureElements(MainWindow* mw)
{
	/**
	* @param[in] ui Pointer to GUI window
	*/

	_mw = mw;
	setup_elements();

	_mw->_toolbar->tableWidgetSimulation->item(0, 0)->setText("10");

	connect(_mw->_toolbar->comboBoxProgramType, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
		this, &GUISimulationProcedureElements::change_in_program_type);
	connect(_mw->_toolbar->comboBoxSimulationType, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
		this, &GUISimulationProcedureElements::change_in_simulation_type);

	_llgMagneticFieldParameters = new MagneticFieldStruct({ 0,0,1,Threedim{0,0,1} });
	_monteCarloMagneticFieldParameters = new MagneticFieldStruct({ 0,0,1,Threedim{ 0,0,1 } });
	_converger1MagneticFieldParameters = new MagneticFieldStruct({ 0,0,1,Threedim{ 0,0,1 } });
	connect(_mw->_toolbar->tableWidgetMagneticField, &QTableWidget::cellChanged, this,
		&GUISimulationProcedureElements::change_in_magnetic_field_table);

	_llgTemperature = new TemperatureStruct({ 0,0,1,Threedim{ 0,0,1 } });
	_monteCarloTemperature = new TemperatureStruct({ 100,1,10,Threedim{ 0,0,1 } });
	_converger1Temperature = new TemperatureStruct({ 0,0,1,Threedim{ 0,0,1 } });
	connect(_mw->_toolbar->tableWidgetTemperature, &QTableWidget::cellChanged,
		this, &GUISimulationProcedureElements::change_in_temperature_table);

	_llgSimParameters = new SimulationProcedureParameters({10000,25,20,0.1,0.001});
	_monteCarloSimParameters = new SimulationProcedureParameters({ 50000,100,20,0,0 });
	_converger1SimParameters = new SimulationProcedureParameters({ 10000, 25, 30, 0,0});
	connect(_mw->_toolbar->tableWidgetSimulation, &QTableWidget::cellChanged,
		this, &GUISimulationProcedureElements::change_in_simulation_table);
	connect(_mw->_toolbar->tableWidgetSimulation2, &QTableWidget::cellChanged,
		this, &GUISimulationProcedureElements::change_in_simulation_table_two);
	connect(_mw->_toolbar->tableWidgetUIUpdate, &QTableWidget::cellChanged,
		this, &GUISimulationProcedureElements::change_in_ui_update_table);

	change_in_program_type("temperature-magnetic-field-loop");
	change_in_simulation_type("Metropolis Monte Carlo");
}

GUISimulationProcedureElements::~GUISimulationProcedureElements()
{
	delete _llgMagneticFieldParameters;
	delete _monteCarloMagneticFieldParameters;
	delete _converger1MagneticFieldParameters;

	delete _llgTemperature;
	delete _monteCarloTemperature;
	delete _converger1Temperature;

	delete _llgSimParameters;
	delete _monteCarloSimParameters;
	delete _converger1SimParameters;
}

void GUISimulationProcedureElements::setup_elements(void)
{
	/**
	Parameters concerning the simulation procedure are specified here. These include loops over magnetic field
	and the temperature. Also number of simulation steps, seed for random number generator ... are specified
	here.
	*/

	// simulation method
	_mw->_toolbar->comboBoxSimulationType->addItem((tr("Metropolis Monte Carlo")));
	_mw->_toolbar->comboBoxSimulationType->addItem((tr("LLG simulation")));
	_mw->_toolbar->comboBoxSimulationType->addItem((tr("Converger1")));

	std::vector<int> ratio;

	_mw->_toolbar->tableWidgetMagneticField->setHorizontalHeader(new MyHeaderView(Qt::Horizontal, 0.3, 0.25));
	_mw->_toolbar->tableWidgetMagneticField->setColumnCount(6);
	_mw->_toolbar->tableWidgetMagneticField->setRowCount(1);
	_mw->_toolbar->tableWidgetMagneticField->verticalHeader()->hide();
	ratio.push_back(76);
	ratio.push_back(76);
	ratio.push_back(76);
	ratio.push_back(40);
	ratio.push_back(40);
	ratio.push_back(40);
	_mw->_toolbar->tableWidgetMagneticField->set_column_ratio(ratio);
	_mw->_toolbar->tableWidgetMagneticField->set_horizontal_header_height(5);
	ratio.clear();
	ratio.push_back(4);
	_mw->_toolbar->tableWidgetMagneticField->set_row_ratio(ratio);
	for (int i = 0; i < 6; i++)
	{
		_mw->_toolbar->tableWidgetMagneticField->setItem(0,i, new QTableWidgetItem());
	}

	_mw->_toolbar->tableWidgetTemperature->setHorizontalHeader(new MyHeaderView(Qt::Horizontal, 0.2, 0.25));
	_mw->_toolbar->tableWidgetTemperature->setColumnCount(6);
	_mw->_toolbar->tableWidgetTemperature->setRowCount(1);
	_mw->_toolbar->tableWidgetTemperature->verticalHeader()->hide();
	ratio.clear();
	ratio.push_back(76);
	ratio.push_back(76);
	ratio.push_back(76);
	ratio.push_back(40);
	ratio.push_back(40);
	ratio.push_back(40);
	_mw->_toolbar->tableWidgetTemperature->set_column_ratio(ratio);
	_mw->_toolbar->tableWidgetTemperature->set_horizontal_header_height(45);
	ratio.clear();
	ratio.push_back(40);
	_mw->_toolbar->tableWidgetTemperature->set_row_ratio(ratio);
	for (int i = 0; i < 6; i++)
	{
		_mw->_toolbar->tableWidgetTemperature->setItem(0, i, new QTableWidgetItem());
	}

	_mw->_toolbar->tableWidgetSimulation->setColumnCount(3);
	_mw->_toolbar->tableWidgetSimulation->setRowCount(1);
	_mw->_toolbar->tableWidgetSimulation->verticalHeader()->hide();
	_mw->_toolbar->tableWidgetSimulation->setHorizontalHeaderLabels(QString("seed;SimSteps;outWidth;").split(";"));
	ratio.clear();
	ratio.push_back(100);
	ratio.push_back(134);
	ratio.push_back(115);
	_mw->_toolbar->tableWidgetSimulation->set_column_ratio(ratio);
	_mw->_toolbar->tableWidgetSimulation->set_horizontal_header_height(4);
	ratio.clear();
	ratio.push_back(4);
	_mw->_toolbar->tableWidgetSimulation->set_row_ratio(ratio);
	for (int i = 0; i < 3; i++)
	{
		_mw->_toolbar->tableWidgetSimulation->setItem(0, i, new QTableWidgetItem());
	}
}

void GUISimulationProcedureElements::read_parameters(const std::shared_ptr<Configuration> &config)
{
	if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Metropolis Monte Carlo") == 0)
	{
		config->_simulationType = metropolis;
	}
	else if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("LLG simulation") == 0)
	{
		config->_simulationType = landauLifshitzGilbert;
		if (_mw->_toolbar->tableWidgetSimulation2->item(0, 0))
		{
			double LLG_dampingParameter = _mw->_toolbar->tableWidgetSimulation2->item(0, 0)->text().toDouble();
			config->_LLG_dampingParameter = LLG_dampingParameter;
		}
		if (_mw->_toolbar->tableWidgetSimulation2->item(0, 1))
		{
			double LLG_timeWidth = _mw->_toolbar->tableWidgetSimulation2->item(0, 1)->text().toDouble();
			config->_LLG_timeWidth = LLG_timeWidth;
		}
	}
	else if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Converger1") == 0)
	{
		config->_simulationType = converger1;
	}

	if (_mw->_toolbar->tableWidgetTemperature->item(0, 0))
	{
		config->_temperatureStart = _mw->_toolbar->tableWidgetTemperature->item(0, 0)->text().toDouble();
	}
	if (_mw->_toolbar->tableWidgetTemperature->item(0, 1))
	{
		config->_temperatureEnd = _mw->_toolbar->tableWidgetTemperature->item(0, 1)->text().toDouble();
	}
	if (_mw->_toolbar->tableWidgetTemperature->item(0, 2))
	{
		config->_temperatureSteps = _mw->_toolbar->tableWidgetTemperature->item(0, 2)->text().toInt();
	}
	if (_mw->_toolbar->tableWidgetTemperature->item(0, 3) && _mw->_toolbar->tableWidgetTemperature->item(0, 4)
		&& _mw->_toolbar->tableWidgetTemperature->item(0, 5))
	{
		Threedim direction = { 0,0,0 };
		direction.x = _mw->_toolbar->tableWidgetTemperature->item(0, 3)->text().toDouble();
		direction.y = _mw->_toolbar->tableWidgetTemperature->item(0, 4)->text().toDouble();
		direction.z = _mw->_toolbar->tableWidgetTemperature->item(0, 5)->text().toDouble();
		config->_temperatureGradientDirection = direction;
	}

	if (_mw->_toolbar->tableWidgetSimulation->item(0, 0))
	{
		config->_seed = _mw->_toolbar->tableWidgetSimulation->item(0, 0)->text().toInt();
	}
	if (_mw->_toolbar->tableWidgetSimulation->item(0, 1))
	{
		config->_simulationSteps = _mw->_toolbar->tableWidgetSimulation->item(0, 1)->text().toInt();
	}
	if (_mw->_toolbar->tableWidgetSimulation->item(0, 2))
	{
		config->_outputWidth = _mw->_toolbar->tableWidgetSimulation->item(0, 2)->text().toInt();
	}
}

void GUISimulationProcedureElements::change_in_simulation_type(QString qString)
{
	set_gui_magnetic_field_elements(qString);

	set_gui_temperature_elements(qString);

	if (qString.compare("LLG simulation") == 0)
	{
		_mw->_toolbar->tableWidgetSimulation2->setColumnCount(2);
		_mw->_toolbar->tableWidgetSimulation2->setRowCount(1);
		_mw->_toolbar->tableWidgetSimulation2->verticalHeader()->hide();
		_mw->_toolbar->tableWidgetSimulation2->custom_resize();
		QStringList header;
		header << QChar(945) << QString(QChar(916)) + "t [ps]";
		_mw->_toolbar->tableWidgetSimulation2->setHorizontalHeaderLabels(header);

		QTableWidgetItem* itemDampingParameter = new QTableWidgetItem();
		itemDampingParameter->setText(QString::number(_llgSimParameters->alpha));
		_mw->_toolbar->tableWidgetSimulation2->setItem(0, 0, itemDampingParameter);

		QTableWidgetItem* itemTimeWidth = new QTableWidgetItem();
		itemTimeWidth->setText(QString::number(_llgSimParameters->deltaT));
		_mw->_toolbar->tableWidgetSimulation2->setItem(0, 1, itemTimeWidth);


		_mw->_toolbar->tableWidgetSimulation->item(0, 1)->setText(QString::number(_llgSimParameters->simulationSteps));
		_mw->_toolbar->tableWidgetSimulation->item(0, 2)->setText(QString::number(_llgSimParameters->outWidth));
		if (_mw->_toolbar->tableWidgetUIUpdate->item(0, 0))
		{
			_mw->_toolbar->tableWidgetUIUpdate->item(0, 0)->setText(QString::number(_llgSimParameters->uiWidth));
		}
	}
	else if (qString.compare("Metropolis Monte Carlo") == 0)
	{
		_mw->_toolbar->tableWidgetSimulation->item(0, 1)->setText(QString::number(_monteCarloSimParameters->simulationSteps));
		_mw->_toolbar->tableWidgetSimulation->item(0, 2)->setText(QString::number(_monteCarloSimParameters->outWidth));

		_mw->_toolbar->tableWidgetSimulation2->clear();
		_mw->_toolbar->tableWidgetSimulation2->setColumnCount(0);
		
		if (_mw->_toolbar->tableWidgetUIUpdate->item(0, 0))
		{
			_mw->_toolbar->tableWidgetUIUpdate->item(0, 0)->setText(QString::number(_monteCarloSimParameters->uiWidth));
		}
	}
	else if (qString.compare("Converger1") == 0)
	{
		_mw->_toolbar->tableWidgetSimulation->item(0, 1)->setText(QString::number(_converger1SimParameters->simulationSteps));
		_mw->_toolbar->tableWidgetSimulation->item(0, 2)->setText(QString::number(_converger1SimParameters->outWidth));

		_mw->_toolbar->tableWidgetSimulation2->clear();
		_mw->_toolbar->tableWidgetSimulation2->setColumnCount(0);

		if (_mw->_toolbar->tableWidgetUIUpdate->item(0, 0))
		{
			_mw->_toolbar->tableWidgetUIUpdate->item(0, 0)->setText(QString::number(_converger1SimParameters->uiWidth));
		}
	}
}

void GUISimulationProcedureElements::change_in_program_type(QString qString)
{
	if (qString.compare("temperature-magnetic-field-loop") == 0
		|| qString.compare("eigenFreq") == 0)
	{
		QString header("<i>B</i><font size=4><sub>0 </sub></font size=4>[T];<i>\
B</i><font size=4><sub>1 </sub></font size=4>[T];steps;e<font size=4><sub>x</sub>;\
e<font size=4><sub>y</sub>;e<font size=4><sub>z</sub>");
		_mw->_toolbar->tableWidgetMagneticField->setHorizontalHeaderLabels(header.split(";"));

		std::vector<int> ratio;
		ratio.push_back(76);
		ratio.push_back(76);
		ratio.push_back(76);
		ratio.push_back(40);
		ratio.push_back(40);
		ratio.push_back(40);
		_mw->_toolbar->tableWidgetTemperature->set_column_ratio(ratio);
		_mw->_toolbar->tableWidgetTemperature->custom_resize();
		_mw->_toolbar->tableWidgetTemperature->setHorizontalHeaderLabels(QString(
			"<i>T</i><font size=4><sub>0 </sub></font size=4>[K];\
<i>T</i><font size=4><sub>1 </sub></font size=4>[K];steps;X;X;X").split(";"));
	}
	if (qString.compare("spin-seebeck") == 0)
	{
		QString header("<i>B</i> [T];X;X;e<font size=4><sub>x</sub>;e<font size=4><sub>y</sub>;e<font size=4><sub>z</sub>");
		_mw->_toolbar->tableWidgetMagneticField->setHorizontalHeaderLabels(header.split(";"));

		std::vector<int> ratio;
		ratio.push_back(90);
		ratio.push_back(90);
		ratio.push_back(48);
		ratio.push_back(40);
		ratio.push_back(40);
		ratio.push_back(40);
		_mw->_toolbar->tableWidgetTemperature->set_column_ratio(ratio);
		_mw->_toolbar->tableWidgetTemperature->custom_resize();
		_mw->_toolbar->tableWidgetTemperature->setHorizontalHeaderLabels(
			QString("<i>T</i><font size=4><sub>min </sub></font size=4>[K];\
<i>T</i><font size=4><sub>max </sub></font size=4>[K];X;e<font size=4><sub>x</sub>;\
e<font size=4><sub>y</sub>;e<font size=4><sub>z</sub>").split(";"));
	}

	set_gui_magnetic_field_elements(qString);
	set_gui_temperature_elements(qString);
}

void GUISimulationProcedureElements::change_in_magnetic_field_table(int row, int column)
{
	if (_mw->_toolbar->tableWidgetMagneticField->item(row, column))
	{
		MagneticFieldStruct* magneticFieldParameters = NULL;
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Metropolis Monte Carlo") == 0)
		{
			magneticFieldParameters = _monteCarloMagneticFieldParameters;
		}
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("LLG simulation") == 0)
		{
			magneticFieldParameters = _llgMagneticFieldParameters;
		}
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Converger1") == 0)
		{
			magneticFieldParameters = _converger1MagneticFieldParameters;
		}
		if (magneticFieldParameters != NULL)
		{
			double value = _mw->_toolbar->tableWidgetMagneticField->item(row, column)->text().toDouble();
			switch (column)
			{
			case 0:	magneticFieldParameters->start = value; break;
			case 1: magneticFieldParameters->end = value; break;
			case 2: magneticFieldParameters->steps = value; break;
			case 3: magneticFieldParameters->direction.x = value; break;
			case 4: magneticFieldParameters->direction.y = value; break;
			case 5: magneticFieldParameters->direction.z = value; break;
			default: break;
			}
		}
	}
}

void GUISimulationProcedureElements::change_in_temperature_table(int row, int column)
{
	if (_mw->_toolbar->tableWidgetTemperature->item(row, column))
	{
		TemperatureStruct* temperature = NULL;
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Metropolis Monte Carlo") == 0)
		{
			temperature = _monteCarloTemperature;
		}
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("LLG simulation") == 0)
		{
			temperature = _llgTemperature;
		}
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Converger1") == 0)
		{
			temperature = _converger1Temperature;
		}

		if (temperature != NULL)
		{
			double value = _mw->_toolbar->tableWidgetTemperature->item(row, column)->text().toDouble();
			switch (column)
			{
			case 0:	temperature->start = value; break;
			case 1: temperature->end = value; break;
			case 2: temperature->steps = value; break;
			case 3: temperature->direction.x = value; break;
			case 4: temperature->direction.y = value; break;
			case 5: temperature->direction.z = value; break;
			default: break;
			}
		}
	}
}

void GUISimulationProcedureElements::change_in_simulation_table(int row, int column)
{
	if (_mw->_toolbar->tableWidgetSimulation->item(row, column))
	{
		SimulationProcedureParameters* parameters = NULL;
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Metropolis Monte Carlo") == 0)
		{
			parameters = _monteCarloSimParameters;
		}
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("LLG simulation") == 0)
		{
			parameters = _llgSimParameters;
		}
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Converger1") == 0)
		{
			parameters = _converger1SimParameters;
		}

		if (parameters != NULL)
		{
			double value = _mw->_toolbar->tableWidgetSimulation->item(row, column)->text().toDouble();
			switch (column)
			{
			case 1:	parameters->simulationSteps = value; break;
			case 2: parameters->outWidth = value; break;
			default: break;
			}
		}
	}
}
void GUISimulationProcedureElements::change_in_simulation_table_two(int row, int column)
{
	if (_mw->_toolbar->tableWidgetSimulation2->item(row, column))
	{
		SimulationProcedureParameters* parameters = NULL;
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Metropolis Monte Carlo") == 0)
		{
			parameters = _monteCarloSimParameters;
		}
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("LLG simulation") == 0)
		{
			parameters = _llgSimParameters;
		}
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Converger1") == 0)
		{
			parameters = _converger1SimParameters;
		}

		if (parameters != NULL)
		{
			double value = _mw->_toolbar->tableWidgetSimulation2->item(row, column)->text().toDouble();
			switch (column)
			{
			case 0: parameters->alpha = value; break;
			case 1:	parameters->deltaT = value; break;
			default: break;
			}
		}
	}
}

void GUISimulationProcedureElements::change_in_ui_update_table(int row, int column)
{
	if (_mw->_toolbar->tableWidgetUIUpdate->item(row, column))
	{
		SimulationProcedureParameters* parameters = NULL;
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Metropolis Monte Carlo") == 0)
		{
			parameters = _monteCarloSimParameters;
		}
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("LLG simulation") == 0)
		{
			parameters = _llgSimParameters;
		}
		if (_mw->_toolbar->comboBoxSimulationType->currentText().compare("Converger1") == 0)
		{
			parameters = _converger1SimParameters;
		}

		if (parameters != NULL)
		{
			double value = _mw->_toolbar->tableWidgetUIUpdate->item(row, column)->text().toDouble();
			switch (column)
			{
			case 0: parameters->uiWidth = value; break;
			default: break;
			}
		}
	}
}

void GUISimulationProcedureElements::set_gui_magnetic_field_elements(QString qString)
{
	MagneticFieldStruct* parameters = NULL;
	if (qString.compare("Metropolis Monte Carlo") == 0)
	{
		parameters = _monteCarloMagneticFieldParameters;
	}
	if (qString.compare("LLG simulation") == 0)
	{
		parameters = _llgMagneticFieldParameters;
	}
	if (qString.compare("Converger1") == 0)
	{
		parameters = _converger1MagneticFieldParameters;
	}
	if (parameters != NULL)
	{
		_mw->_toolbar->tableWidgetMagneticField->item(0, 0)->setText(QString::number(parameters->start));
		_mw->_toolbar->tableWidgetMagneticField->item(0, 1)->setText(QString::number(parameters->end));
		_mw->_toolbar->tableWidgetMagneticField->item(0, 2)->setText(QString::number(parameters->steps));
		_mw->_toolbar->tableWidgetMagneticField->item(0, 3)->setText(QString::number(parameters->direction.x));
		_mw->_toolbar->tableWidgetMagneticField->item(0, 4)->setText(QString::number(parameters->direction.y));
		_mw->_toolbar->tableWidgetMagneticField->item(0, 5)->setText(QString::number(parameters->direction.z));
	}
}

void GUISimulationProcedureElements::set_gui_temperature_elements(QString qString)
{
	TemperatureStruct* temperature = NULL;
	if (qString.compare("Metropolis Monte Carlo") == 0)
	{
		temperature = _monteCarloTemperature;
	}
	if (qString.compare("LLG simulation") == 0)
	{
		temperature = _llgTemperature;
	}
	if (qString.compare("Converger1") == 0)
	{
		temperature = _converger1Temperature;
	}
	if (temperature != NULL)
	{
		QString qString = _mw->_toolbar->comboBoxProgramType->currentText();
		if (qString.compare("temperature-magnetic-field-loop") == 0)
		{
			_mw->_toolbar->tableWidgetTemperature->item(0, 0)->setText(QString::number(temperature->start));
			_mw->_toolbar->tableWidgetTemperature->item(0, 1)->setText(QString::number(temperature->end));
			_mw->_toolbar->tableWidgetTemperature->item(0, 2)->setText(QString::number(temperature->steps));
			_mw->_toolbar->tableWidgetTemperature->item(0, 3)->setText("");
			_mw->_toolbar->tableWidgetTemperature->item(0, 4)->setText("");
			_mw->_toolbar->tableWidgetTemperature->item(0, 5)->setText("");
		}
		if (qString.compare("spin-seebeck") == 0)
		{
			_mw->_toolbar->tableWidgetTemperature->item(0, 0)->setText(QString::number(temperature->start));
			_mw->_toolbar->tableWidgetTemperature->item(0, 1)->setText(QString::number(temperature->end));
			_mw->_toolbar->tableWidgetTemperature->item(0, 2)->setText(QString::number(temperature->steps));
			_mw->_toolbar->tableWidgetTemperature->item(0, 3)->setText(QString::number(temperature->direction.x));
			_mw->_toolbar->tableWidgetTemperature->item(0, 4)->setText(QString::number(temperature->direction.y));
			_mw->_toolbar->tableWidgetTemperature->item(0, 5)->setText(QString::number(temperature->direction.z));
		}
	}
}
