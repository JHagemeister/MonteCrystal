/*
* GUIOutputElements.cpp
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

#include "GUIOutputElements.h"

// forward includes
#include "Configuration.h"

GUIOutputElements::GUIOutputElements(Ui::QtMainWindow* ui)
{
	/**
	* @param[in] ui Pointer to object with ui elements
	*/
	_ui = ui;
	setup_elements();
	set_default_values();
}

void GUIOutputElements::setup_elements(void)
{
	/**
	The output parameters are specified here. This includes the update of the OGLwidget and the output into 
	text files.
	*/

	_ui->tableWidgetUIUpdate->setColumnCount(1);
	_ui->tableWidgetUIUpdate->setRowCount(1);
	_ui->tableWidgetUIUpdate->verticalHeader()->hide();
	_ui->tableWidgetUIUpdate->setHorizontalHeaderLabels(QString("uiWidth;").split(";"));

	_ui->tableWidgetMovie->setColumnCount(3);
	_ui->tableWidgetMovie->setRowCount(1);
	_ui->tableWidgetMovie->verticalHeader()->hide();
	_ui->tableWidgetMovie->setHorizontalHeaderLabels(QString("start;stop;width").split(";"));

	_checkBox_E = new QCheckBox(tr("E"));
	_checkBox_M = new QCheckBox(tr("M"));
	_checkBox_Mabs = new QCheckBox(tr("|M|"));
	_checkBox_NCMR = new QCheckBox(tr("NCMR"));
	_checkBox_espin = new QCheckBox(tr("espin"));
	_checkBox_spinConfig = new QCheckBox(tr("spin"));
	_checkBox_Skn = new QCheckBox(tr("SkN"));
	_checkBox_outSimStep = new QCheckBox(tr("Step"));

	_ui->gridLayoutOutput->addWidget(_checkBox_E, 0, 0);
	_ui->gridLayoutOutput->addWidget(_checkBox_M, 0, 1);
	_ui->gridLayoutOutput->addWidget(_checkBox_Mabs, 0, 2);
	_ui->gridLayoutOutput->addWidget(_checkBox_NCMR, 0, 3);
	_ui->gridLayoutOutput->addWidget(_checkBox_espin, 1, 0);
	_ui->gridLayoutOutput->addWidget(_checkBox_Skn, 1, 1);
	_ui->gridLayoutOutput->addWidget(_checkBox_spinConfig, 1, 2);
	_ui->gridLayoutOutput->addWidget(_checkBox_outSimStep, 1, 3);

	QTableWidgetItem* uiWidth = new QTableWidgetItem();
	_ui->tableWidgetUIUpdate->setItem(0, 0, uiWidth);
}

void GUIOutputElements::set_default_values(void)
{
	/**
	* Set default values for output parameters
	*/

	_ui->tableWidgetUIUpdate->item(0, 0)->setText("500");
}

void GUIOutputElements::read_parameters(QSharedPointer<Configuration> &config)
{
	/**
	Reads output parameters from GUI.

	@param[in] config Object to store parameters in.
	*/

	if (_ui->tableWidgetUIUpdate->item(0, 0))
	{
		config->_uiUpdateWidth = _ui->tableWidgetUIUpdate->item(0, 0)->text().toInt();
	}
	if (_checkBox_E->isChecked())
	{
		config->_boolE = TRUE;
	}
	if (_checkBox_M->isChecked())
	{
		config->_boolM = TRUE;
	}
	if (_checkBox_Mabs->isChecked())
	{
		config->_boolMABS = TRUE;
	}
	if (_checkBox_NCMR->isChecked())
	{
		config->_boolNCMR = TRUE;
	}
	if (_checkBox_Skn->isChecked())
	{
		config->_boolWindingNumber = TRUE;
	}
	if (_checkBox_spinConfig->isChecked())
	{
		config->_boolSpinConfig = TRUE;
	}
	if (_checkBox_espin->isChecked())
	{
		config->_boolEachSpin = TRUE;
	}
	if (_checkBox_outSimStep->isChecked())
	{
		config->_boolOutSimulationSteps = TRUE;
	}

	if (_ui->tableWidgetMovie->item(0, 0))
	{
		config->_movieStart = _ui->tableWidgetMovie->item(0, 0)->text().toDouble();
	}
	if (_ui->tableWidgetMovie->item(0, 1))
	{
		config->_movieEnd = _ui->tableWidgetMovie->item(0, 1)->text().toDouble();
	}
	if (_ui->tableWidgetMovie->item(0, 2))
	{
		config->_movieWidth = _ui->tableWidgetMovie->item(0, 2)->text().toDouble();
	}
}
