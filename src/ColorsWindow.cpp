/*
* ColorsWindow.cpp
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

#include "ColorsWindow.h"

#include "MyHeaderView.h"
#include <QColor>
#include <QCheckBox>
#include <QMessageBox>
#include "OGLWidget.h"
#include <math.h>
ColorsWindow::ColorsWindow(OGLWidget* oglWidget, QWidget * parent): QDialog(parent), 
_spinColor(0.8f, 0.8f, 0.8f, 1.0f)
{
	_ui.setupUi(this);

	// this helped with a problem that the window was not fully visible on all computer screens
	if (parent != Q_NULLPTR) 
	{
		this->setMaximumHeight(parent->height());
	}

	_oglWidget = oglWidget;
	_visibleSpinOrder = oglWidget->get_visible_spins_order();

	_boolWorkingColorMapCheckBoxes = 0;
	_boolWorkingCheckBoxes = 0;
	_colorMapType = NoMap;

	setup_background_color();

	// setup window elements
	setup_spin_model();
	setup_spin_color();
	setup_color_map_checkBoxes();
	setup_min_max_tables();
	setup_color_map();
	setup_magnetization_tables_color_map();

	// set values as defined previously in _oglWidget
	set_values();

	connect(_ui.tableWidgetBackgroundColor, &QTableWidget::cellChanged, this,
		&ColorsWindow::change_in_background_color_table_widget);
	connect(_ui.tableWidgetColorMap, &QTableWidget::cellChanged, this, 
		&ColorsWindow::change_in_color_map_table_widget);
	connect(_ui.tableWidgetSpinColor, &QTableWidget::cellChanged, this,
		&ColorsWindow::change_in_spin_color_table_widget);

	connect(_ui.checkBoxSpinVectors, &QCheckBox::stateChanged, this, 
		&ColorsWindow::check_state_changed_checkBoxSpinVectors);
	connect(_ui.checkBoxSpinVectorRGB, &QCheckBox::stateChanged, this,
		&ColorsWindow::check_state_changed_checkBoxSpinVectorRGB);
	connect(_ui.checkBoxSpinVectorSingleColor, &QCheckBox::stateChanged, this,
		&ColorsWindow::check_state_changed_checkBoxSpinVectorSingleColor);

	connect(_ui.pushButton_addSpins, &QAbstractButton::released, this,
		&ColorsWindow::pushButton_add_spins);
	connect(_ui.pushButton_removeSpins, &QAbstractButton::released, this,
		&ColorsWindow::pushButton_remove_spins);

	connect(_checkBox_E, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_E);
	connect(_checkBox_J1, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_J1);
	connect(_checkBox_J2, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_J2);
	connect(_checkBox_J3, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_J3);
	connect(_checkBox_J4, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_J4);
	connect(_checkBox_J5, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_J5);
	connect(_checkBox_J6, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_J6);
	connect(_checkBox_J7, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_J7);
	connect(_checkBox_J8, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_J8);
	connect(_checkBox_D1, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_D1);
	connect(_checkBox_D2, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_D2);
	connect(_checkBox_D3, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_D3);
	connect(_checkBox_D4, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_D4);
	connect(_checkBox_D5, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_D5);
	connect(_checkBox_Dipol, &QCheckBox::stateChanged, this, 
		&ColorsWindow::check_state_changed_checkBox_Dipol);
	connect(_checkBox_Anisotropy, &QCheckBox::stateChanged, this, 
		&ColorsWindow::check_state_changed_checkBox_Anisotropy);
	connect(_checkBox_BiQ, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_BiQ);
	connect(_checkBox_4Spin, &QCheckBox::stateChanged, this, 
		&ColorsWindow::check_state_changed_checkBox_4Spin);
	connect(_checkBox_Zeeman, &QCheckBox::stateChanged, this, 
		&ColorsWindow::check_state_changed_checkBox_Zeeman);
	connect(_checkBox_M, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_M);
	connect(_checkBox_Skn, &QCheckBox::stateChanged, this, &ColorsWindow::check_state_changed_checkBox_Skn);

	connect(_ui.tableWidgetMTheta, &QTableWidget::cellChanged, this,
		&ColorsWindow::change_in_magnetization_table_theta_color_map);
	connect(_ui.tableWidgetMPhi, &QTableWidget::cellChanged, this,
		&ColorsWindow::change_in_magnetization_table_phi_color_map);
	connect(_ui.tableWidget_SpinMesh, &QTableWidget::cellChanged, this,
		&ColorsWindow::change_in_spin_model_table);

	connect(_ui.pushButtonOk, &QAbstractButton::released, this, &ColorsWindow::apply_and_close_button);
	connect(_ui.pushButtonApply, &QAbstractButton::released, this, &ColorsWindow::apply_button);
}

ColorsWindow::~ColorsWindow() {
	
}

void ColorsWindow::error_box(void)
{
	QMessageBox::information(this, tr("Error"),
		"Close colors window prior to starting new simulation.");
}

void ColorsWindow::change_in_background_color_table_widget(int row, int column)
{
	/**
	* React to change in table containing color values for color map
	*/
	glm::vec4 color{ 0.0f, 0.0f, 0.0f, 1.0f };
	if (_ui.tableWidgetBackgroundColor->item(0, 0))
	{
		color.r = (float)_ui.tableWidgetBackgroundColor->item(0, 0)->text().toDouble() / 255.;
	}
	if (_ui.tableWidgetBackgroundColor->item(0, 1))
	{
		color.g = (float)_ui.tableWidgetBackgroundColor->item(0, 1)->text().toDouble() / 255.;
	}
	if (_ui.tableWidgetBackgroundColor->item(0, 2))
	{
		color.b = (float)_ui.tableWidgetBackgroundColor->item(0, 2)->text().toDouble() / 255.;
	}
	_oglWidget->set_background_color(color);
	_oglWidget->repaint();
}

void ColorsWindow::change_in_color_map_table_widget(int row, int column)
{
	/**
	* React to change in table containing color values for color map
	*/

	read_color_map_colors();
	update_display_colors_table_widget_color_map();
}

void ColorsWindow::change_in_spin_color_table_widget(int row, int column)
{
	/**
	* React to change in table containing color values for spin color.
	*/

	read_spin_color();
	update_display_color_table_widget_spin_color();
}

void ColorsWindow::change_in_magnetization_table_theta_color_map(int row, int column)
{

	if (_ui.tableWidgetMTheta->item(0, 0))
	{
		if (_ui.tableWidgetMTheta->item(0, 0)->text().compare("") != 0)
		{
			uncheck_all_color_map_check_boxes();
			_colorMapType = CustomDirectionMagnetizationMap;
			_colorMapIdentifier = "CustomMagnetizationDirection ";
			set_current_min_max("", "");
			set_selected_min_max("-1", "1");
			if (!_ui.tableWidgetMPhi->item(0, 0))
			{
				_ui.tableWidgetMPhi->setItem(0, 0, new QTableWidgetItem("0"));
			}
			else
			{
				if (_ui.tableWidgetMPhi->item(0, 0)->text().compare("") == 0)
				{
					_ui.tableWidgetMPhi->item(0, 0)->setText("0");
				}
			}
		}
	}
}

void ColorsWindow::change_in_magnetization_table_phi_color_map(int row, int column)
{
	if (_ui.tableWidgetMPhi->item(0, 0))
	{
		if (_ui.tableWidgetMPhi->item(0, 0)->text().compare("") != 0)
		{
			uncheck_all_color_map_check_boxes();
			_colorMapType = CustomDirectionMagnetizationMap;
			_colorMapIdentifier = "CustomMagnetizationDirection ";
			set_current_min_max("", "");
			set_selected_min_max("-1", "1");
			if (!_ui.tableWidgetMTheta->item(0, 0))
			{
				_ui.tableWidgetMTheta->setItem(0, 0, new QTableWidgetItem("0"));
			}
			else
			{
				if (_ui.tableWidgetMTheta->item(0, 0)->text().compare("") == 0)
				{
					_ui.tableWidgetMTheta->item(0, 0)->setText("0");
				}
			}
		}
	}
}

void ColorsWindow::change_in_spin_model_table(int row, int column)
{
	if (_ui.tableWidget_SpinMesh->item(row, column))
	{
		QString parameter = _ui.tableWidget_SpinMesh->item(row, column)->text();
		if (parameter.compare("") != 0)
		{
			SpinMeshParams params = _oglWidget->_spinMeshParams;
			switch (column)
			{
			case 0:
				if (parameter.toInt() > 3)
				{
					params.n = parameter.toInt();
				}
				else
				{
					set_spin_model_params(params);
				}
				break;
			case 1:
				if (parameter.toFloat() > 1)
				{
					params.r2divr1 = parameter.toFloat();
				}
				else
				{
					set_spin_model_params(params);
				}
				break;
			case 2:
				if (parameter.toFloat() < 0)
				{
					set_spin_model_params(params);
				}
				else
				{
					params.l1divl2 = parameter.toFloat();
				}
				break;
			case 3:
				if (parameter.toFloat() > 0)
				{
                    params.r2divl = parameter.toFloat();
				}
				else
				{
					set_spin_model_params(params);
				}
				break;
			case 4:
				if (parameter.toFloat() > 0)
				{
					params.scale = parameter.toFloat();
				}
				else
				{
					set_spin_model_params(params);
				}
				break;
			}
			_oglWidget->set_spin_model(params);
			_oglWidget->repaint();
		}
		else
		{
			SpinMeshParams params = _oglWidget->_spinMeshParams;
			set_spin_model_params(params);
		}
	}
}

void ColorsWindow::check_state_changed_checkBoxSpinVectors(int state)
{
	_boolWorkingCheckBoxes = 1;
	if (state == 0)
	{
		_ui.checkBoxSpinVectorRGB->setCheckState(Qt::Unchecked);
		_ui.checkBoxSpinVectorSingleColor->setCheckState(Qt::Unchecked);
	}
	else if (state == 2)
	{
		_ui.checkBoxSpinVectorSingleColor->setCheckState(Qt::Checked);
		_ui.checkBoxSpinVectorRGB->setCheckState(Qt::Unchecked);
	}
	_boolWorkingCheckBoxes = 0;
}

void ColorsWindow::check_state_changed_checkBoxSpinVectorSingleColor(int state)
{
	if (_boolWorkingCheckBoxes == 0)
	{
		_boolWorkingCheckBoxes = 1;
		if (state == 0)
		{
			_ui.checkBoxSpinVectors->setChecked(1);
            _ui.checkBoxSpinVectorRGB->setCheckState(Qt::Checked);
		}
		else
		{
			_ui.checkBoxSpinVectors->setChecked(1);
			_ui.checkBoxSpinVectorRGB->setCheckState(Qt::Unchecked);
			read_spin_color();
			
		}
		_boolWorkingCheckBoxes = 0;
	}
}

void ColorsWindow::check_state_changed_checkBoxSpinVectorRGB(int state)
{
	if (_boolWorkingCheckBoxes == 0)
	{
		_boolWorkingCheckBoxes = 1;
		if (state == 0)
		{
			_ui.checkBoxSpinVectors->setChecked(1);
			_ui.checkBoxSpinVectorSingleColor->setCheckState(Qt::Checked);
			read_spin_color();
		}
		else
		{
			_ui.checkBoxSpinVectors->setChecked(1);
			_ui.checkBoxSpinVectorSingleColor->setCheckState(Qt::Unchecked);
		}
		_boolWorkingCheckBoxes = 0;
	}
}

void ColorsWindow::pushButton_add_spins(void)
{
	if (_visibleSpinOrder > 1)
	{
		--_visibleSpinOrder;
	}
	_oglWidget->set_visible_spins_order(_visibleSpinOrder);
	_oglWidget->repaint();
}

void ColorsWindow::pushButton_remove_spins(void)
{
	++_visibleSpinOrder;
	_oglWidget->set_visible_spins_order(_visibleSpinOrder);
	_oglWidget->repaint();
}

void ColorsWindow::check_state_changed_checkBox_E(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_E->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E ");
		_colorMapType = TotalEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_J1(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_J1->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_J1 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_J1 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_J2(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_J2->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_J2 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_J2 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_J3(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_J3->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_J3 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_J3 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_J4(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_J4->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_J4 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_J4 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_J5(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_J5->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_J5 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_J5 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_J6(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_J6->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_J6 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_J6 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_J7(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_J7->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_J7 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_J7 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_J8(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_J8->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_J8 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_J8 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_D1(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_D1->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_D1 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_D1 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_D2(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_D2->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_D2 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_D2 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_D3(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_D3->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_D3 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_D3 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_D4(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_D4->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_D4 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_D4 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_D5(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_D5->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_D5 ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_D5 ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_Dipol(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_Dipol->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_Dipol ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_Dipol ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_Anisotropy(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_Anisotropy->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_Anisotropy ");
		_colorMapType = AnisotropyEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_Anisotropy ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_BiQ(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_BiQ->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_BiQ ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_BiQ ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_4Spin(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_4Spin->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_4Spin ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_4Spin ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_Zeeman(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_Zeeman->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("E_Zeeman ");
		_colorMapType = SingleEnergyMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "E_Zeeman ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_M(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_M->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("M ");
		_colorMapType = MagnetizationMap;
		_boolWorkingColorMapCheckBoxes = 0;
		set_current_min_max("","");
		set_selected_min_max("-1", "1");
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::check_state_changed_checkBox_Skn(int state)
{
	if (_boolWorkingColorMapCheckBoxes == 0 && state == 2)
	{
		_boolWorkingColorMapCheckBoxes = 1;
		uncheck_all_color_map_check_boxes();
		_checkBox_Skn->setCheckState(Qt::Checked);
		_colorMapIdentifier.clear();
		_colorMapIdentifier.append("Skn ");
		_colorMapType = TopologicalChargeMap;
		_boolWorkingColorMapCheckBoxes = 0;
		double min = 0;
		double max = 0;
		_oglWidget->get_current_min_max(_colorMapType, "Skn ", min, max);
		set_current_min_max(QString::number(min), QString::number(max));
		set_selected_min_max(QString::number(min), QString::number(max));
		set_magnetization_table_color_map("", "");
	}
	if (state == 0)
	{
		_colorMapType = NoMap;
		_colorMapIdentifier.clear();
		_colorMapIdentifier = "";
	}
}

void ColorsWindow::apply_button(void)
{
	_oglWidget->set_color_map_type(_colorMapType, _colorMapIdentifier);
	_oglWidget->set_color_map_colors(_colorMapColors);

	double min = 0;
	double max = 0;
	int boolSet = FALSE;
	if (_ui.tableWidgetChosenMin->item(0, 0) && _ui.tableWidgetChosenMax->item(0, 0))
	{
		min = _ui.tableWidgetChosenMin->item(0, 0)->text().toDouble();
		max = _ui.tableWidgetChosenMax->item(0, 0)->text().toDouble();
		boolSet = TRUE;
	}

	if (min < max && boolSet == TRUE)
	{
		_oglWidget->set_color_map_min_max(min, max);
	}
	else
	{
		_oglWidget->set_color_map_min_max_later();
	}

	_oglWidget->update_color_scale();

	_oglWidget->set_bool_display_spins(_ui.checkBoxSpinVectors->isChecked());
	_oglWidget->set_spin_color(_spinColor);
	if (_ui.checkBoxSpinVectorRGB->isChecked())
	{
		_oglWidget->set_spin_color_rgb_scale();
	}


	double theta = 0;
	double phi = 0;
	if (_ui.tableWidgetMTheta->item(0, 0) && _ui.tableWidgetMPhi->item(0, 0))
	{
		theta = _ui.tableWidgetMTheta->item(0, 0)->text().toDouble()*Pi/180.;
		phi = _ui.tableWidgetMPhi->item(0, 0)->text().toDouble()*Pi/180.;
	}
	Threedim direction{ sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta) };
	_oglWidget->set_custom_magnetization_direction_color_map(direction);

	emit send_repaint_request();
}

void ColorsWindow::apply_and_close_button(void)
{
	apply_button();
	this->close();
}

void ColorsWindow::set_current_min_max(QString min, QString max)
{
	if (_ui.tableWidgetCurrentMin->item(0, 0))
	{
		_ui.tableWidgetCurrentMin->item(0, 0)->setText(min);
	}
	else
	{
		_ui.tableWidgetCurrentMin->setItem(0, 0, new QTableWidgetItem(min));
	}
	if (_ui.tableWidgetCurrentMax->item(0, 0))
	{
		_ui.tableWidgetCurrentMax->item(0, 0)->setText(max);
	}
	else
	{
		_ui.tableWidgetCurrentMax->setItem(0, 0, new QTableWidgetItem(max));
	}
}

void ColorsWindow::set_selected_min_max(QString min, QString max)
{
	if (_ui.tableWidgetChosenMin->item(0, 0))
	{
		_ui.tableWidgetChosenMin->item(0, 0)->setText(min);
	}
	else
	{
		_ui.tableWidgetChosenMin->setItem(0, 0, new QTableWidgetItem(min));
	}
	if (_ui.tableWidgetChosenMax->item(0, 0))
	{
		_ui.tableWidgetChosenMax->item(0, 0)->setText(max);
	}
	else
	{
		_ui.tableWidgetChosenMax->setItem(0, 0, new QTableWidgetItem(max));
	}
}

void ColorsWindow::set_values()
{
	_ui.tableWidgetSpinColor->item(0, 0)->setText("200");
	_ui.tableWidgetSpinColor->item(0, 1)->setText("200");
	_ui.tableWidgetSpinColor->item(0, 2)->setText("200");

	if (_oglWidget->get_bool_display_spins())
	{
		_ui.checkBoxSpinVectors->setChecked(1);
		if (_oglWidget->get_bool_spin_single_color())
		{
			_ui.checkBoxSpinVectorSingleColor->setChecked(1);
			_spinColor = _oglWidget->get_spin_color();
		}
		else 
		{
			_ui.checkBoxSpinVectorRGB->setChecked(1);
		}
	}

	update_display_color_table_widget_spin_color();

	_colorMapColors = _oglWidget->get_color_map_colors();
	set_color_map_colors(_colorMapColors);
	update_display_colors_table_widget_color_map();
	_oglWidget->get_color_map_type(_colorMapType, _colorMapIdentifier);

	double min = 0;
	double max = 0;
	_oglWidget->get_current_min_max(_colorMapType, _colorMapIdentifier, min, max);
	set_current_min_max(QString::number(min), QString::number(max));

	_oglWidget->get_color_map_min_max(min, max);
	set_selected_min_max(QString::number(min), QString::number(max));

	if (_colorMapType == CustomDirectionMagnetizationMap)
	{
		Threedim direction = _oglWidget->get_custom_magnetization_direction_color_map();
		double theta = acos(direction.z)*180/Pi;
		double phi = atan2(direction.y, direction.x)*180/Pi;
		set_magnetization_table_color_map(QString::number(theta), QString::number(phi));
	}
	
	_boolWorkingColorMapCheckBoxes = 1;
	if (_colorMapIdentifier.compare("E ") == 0)
	{
		_checkBox_E->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_J1 ") == 0)
	{
		_checkBox_J1->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_J2 ") == 0)
	{
		_checkBox_J2->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_J3 ") == 0)
	{
		_checkBox_J3->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_J4 ") == 0)
	{
		_checkBox_J4->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_J5 ") == 0)
	{
		_checkBox_J5->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_J6 ") == 0)
	{
		_checkBox_J6->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_J7 ") == 0)
	{
		_checkBox_J7->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_J8 ") == 0)
	{
		_checkBox_J8->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_D1 ") == 0)
	{
		_checkBox_D1->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_D2 ") == 0)
	{
		_checkBox_D2->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_D3 ") == 0)
	{
		_checkBox_D3->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_D4 ") == 0)
	{
		_checkBox_D4->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_D5 ") == 0)
	{
		_checkBox_D5->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_Dipol ") == 0)
	{
		_checkBox_Dipol->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_Anisotropy ") == 0)
	{
		_checkBox_Anisotropy->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_BiQ ") == 0)
	{
		_checkBox_BiQ->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_4Spin ") == 0)
	{
		_checkBox_4Spin->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("E_Zeeman ") == 0)
	{
		_checkBox_Zeeman->setChecked(1);
	}
	else if (_colorMapIdentifier.compare("M ") == 0)
	{
		_checkBox_M->setChecked(1);
		set_current_min_max("","");
		set_selected_min_max("-1", "1");
	}
	else if (_colorMapIdentifier.compare("Skn ") == 0)
	{
		_checkBox_Skn->setChecked(1);
	}
	_boolWorkingColorMapCheckBoxes = 0;
}

void ColorsWindow::set_spin_model_params(SpinMeshParams params)
{
	if (!_ui.tableWidget_SpinMesh->item(0, 0))
	{
		_ui.tableWidget_SpinMesh->setItem(0, 0, new QTableWidgetItem());
	}
	_ui.tableWidget_SpinMesh->item(0, 0)->setText(QString::number(params.n));

	if (!_ui.tableWidget_SpinMesh->item(0, 1))
	{
		_ui.tableWidget_SpinMesh->setItem(0, 1, new QTableWidgetItem());
	}
	_ui.tableWidget_SpinMesh->item(0, 1)->setText(QString::number(params.r2divr1));

	if (!_ui.tableWidget_SpinMesh->item(0, 2))
	{
		_ui.tableWidget_SpinMesh->setItem(0, 2, new QTableWidgetItem());
	}
	_ui.tableWidget_SpinMesh->item(0, 2)->setText(QString::number(params.l1divl2));

	if (!_ui.tableWidget_SpinMesh->item(0, 3))
	{
		_ui.tableWidget_SpinMesh->setItem(0, 3, new QTableWidgetItem());
	}
    _ui.tableWidget_SpinMesh->item(0, 3)->setText(QString::number(params.r2divl));

	if (!_ui.tableWidget_SpinMesh->item(0, 4))
	{
		_ui.tableWidget_SpinMesh->setItem(0, 4, new QTableWidgetItem());
	}
	_ui.tableWidget_SpinMesh->item(0, 4)->setText(QString::number(params.scale));
	
}

void ColorsWindow::uncheck_all_color_map_check_boxes(void)
{
	_checkBox_E->setCheckState(Qt::Unchecked);
	_checkBox_J1->setCheckState(Qt::Unchecked);
	_checkBox_J2->setCheckState(Qt::Unchecked);
	_checkBox_J3->setCheckState(Qt::Unchecked);
	_checkBox_J4->setCheckState(Qt::Unchecked);
	_checkBox_J5->setCheckState(Qt::Unchecked);
	_checkBox_J6->setCheckState(Qt::Unchecked);
	_checkBox_J7->setCheckState(Qt::Unchecked);
	_checkBox_J8->setCheckState(Qt::Unchecked);
	_checkBox_D1->setCheckState(Qt::Unchecked);
	_checkBox_D2->setCheckState(Qt::Unchecked);
	_checkBox_D3->setCheckState(Qt::Unchecked);
	_checkBox_D4->setCheckState(Qt::Unchecked);
	_checkBox_D5->setCheckState(Qt::Unchecked);
	_checkBox_Dipol->setCheckState(Qt::Unchecked);
	_checkBox_Anisotropy->setCheckState(Qt::Unchecked);
	_checkBox_BiQ->setCheckState(Qt::Unchecked);
	_checkBox_4Spin->setCheckState(Qt::Unchecked);
	_checkBox_Zeeman->setCheckState(Qt::Unchecked);
	_checkBox_M->setCheckState(Qt::Unchecked);
	_checkBox_Skn->setCheckState(Qt::Unchecked);
}

void ColorsWindow::set_magnetization_table_color_map(QString theta, QString phi)
{
	if (_ui.tableWidgetMTheta->item(0, 0))
	{
		_ui.tableWidgetMTheta->item(0, 0)->setText(theta);
	}
	else
	{
		_ui.tableWidgetMTheta->setItem(0, 0, new QTableWidgetItem(theta));
	}
	if (_ui.tableWidgetMPhi->item(0, 0))
	{
		_ui.tableWidgetMPhi->item(0, 0)->setText(phi);
	}
	else
	{
		_ui.tableWidgetMPhi->setItem(0, 0, new QTableWidgetItem(phi));
	}
}

void ColorsWindow::setup_min_max_tables(void)
{
	std::vector<int> ratio;
	ratio.push_back(2);

	_ui.tableWidgetCurrentMin->setColumnCount(1);
	_ui.tableWidgetCurrentMin->set_column_ratio(ratio);
	_ui.tableWidgetCurrentMin->set_vertical_header_width(1);
	_ui.tableWidgetCurrentMin->setRowCount(1);
	_ui.tableWidgetCurrentMin->horizontalHeader()->hide();
	_ui.tableWidgetCurrentMin->verticalHeader()->setVisible(1);
	_ui.tableWidgetCurrentMin->setVerticalHeaderLabels(QString("min;").split(";"));
	_ui.tableWidgetCurrentMin->custom_resize();
	_ui.tableWidgetCurrentMin->setDisabled(1);

	_ui.tableWidgetCurrentMax->setColumnCount(1);
	_ui.tableWidgetCurrentMax->set_column_ratio(ratio);
	_ui.tableWidgetCurrentMax->set_vertical_header_width(1);
	_ui.tableWidgetCurrentMax->setRowCount(1);
	_ui.tableWidgetCurrentMax->horizontalHeader()->hide();
	_ui.tableWidgetCurrentMax->verticalHeader()->setVisible(1);
	_ui.tableWidgetCurrentMax->setVerticalHeaderLabels(QString("max;").split(";"));
	_ui.tableWidgetCurrentMax->custom_resize();
	_ui.tableWidgetCurrentMax->setDisabled(1);

	_ui.tableWidgetChosenMin->setColumnCount(1);
	_ui.tableWidgetChosenMin->set_column_ratio(ratio);
	_ui.tableWidgetChosenMin->set_vertical_header_width(1);
	_ui.tableWidgetChosenMin->setRowCount(1);
	_ui.tableWidgetChosenMin->horizontalHeader()->hide();
	_ui.tableWidgetChosenMin->verticalHeader()->setVisible(1);
	_ui.tableWidgetChosenMin->setVerticalHeaderLabels(QString("min;").split(";"));
	_ui.tableWidgetChosenMin->custom_resize();

	_ui.tableWidgetChosenMax->setColumnCount(1);
	_ui.tableWidgetChosenMax->set_column_ratio(ratio);
	_ui.tableWidgetChosenMax->set_vertical_header_width(1);
	_ui.tableWidgetChosenMax->setRowCount(1);
	_ui.tableWidgetChosenMax->horizontalHeader()->hide();
	_ui.tableWidgetChosenMax->verticalHeader()->setVisible(1);
	_ui.tableWidgetChosenMax->setVerticalHeaderLabels(QString("max;").split(";"));
	_ui.tableWidgetChosenMax->custom_resize();
}

void ColorsWindow::setup_color_map_checkBoxes(void)
{
	_checkBox_E = new QCheckBox(tr("E"));
	_checkBox_J1 = new QCheckBox(tr("J1"));
	_checkBox_J2 = new QCheckBox(tr("J2"));
	_checkBox_J3 = new QCheckBox(tr("J3"));
	_checkBox_J4 = new QCheckBox(tr("J4"));
	_checkBox_J5 = new QCheckBox(tr("J5"));
	_checkBox_J6 = new QCheckBox(tr("J6"));
	_checkBox_J7 = new QCheckBox(tr("J7"));
	_checkBox_J8 = new QCheckBox(tr("J8"));
	_checkBox_D1 = new QCheckBox(tr("D1"));
	_checkBox_D2 = new QCheckBox(tr("D2"));
	_checkBox_D3 = new QCheckBox(tr("D3"));
	_checkBox_D4 = new QCheckBox(tr("D4"));
	_checkBox_D5 = new QCheckBox(tr("D5"));
	_checkBox_Dipol = new QCheckBox(tr("Dipol"));
	_checkBox_Anisotropy = new QCheckBox(tr("Anisotropies"));
	_checkBox_BiQ = new QCheckBox(tr("BiQ"));
	_checkBox_4Spin = new QCheckBox(tr("4Spin"));
	_checkBox_Zeeman = new QCheckBox(tr("Zeeman"));
	_checkBox_M = new QCheckBox(tr("M"));
	_checkBox_Skn = new QCheckBox(tr("Skn"));

	_ui.gridLayoutColorMapItems->addWidget(_checkBox_E, 0, 0);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_J1, 0, 1);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_J2, 0, 2);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_J3, 0, 3);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_J4, 1, 0);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_J5, 1, 1);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_J6, 1, 2);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_Zeeman, 1, 3);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_J7, 2, 0);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_J8, 2, 1);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_D1, 2, 2);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_Anisotropy, 2, 3);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_D2, 3, 0);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_D3, 3, 1);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_D4, 3, 2);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_D5, 3, 3);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_Dipol, 4, 0);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_BiQ, 4, 1);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_4Spin, 4, 2);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_M, 5, 0);
	_ui.gridLayoutColorMapItems->addWidget(_checkBox_Skn, 5, 1);
}

void ColorsWindow::setup_magnetization_tables_color_map(void)
{
	std::vector<int> ratio;
	ratio.push_back(1);

	_ui.tableWidgetMTheta->setColumnCount(1);
	_ui.tableWidgetMTheta->set_column_ratio(ratio);
	_ui.tableWidgetMTheta->set_vertical_header_width(1);
	_ui.tableWidgetMTheta->setRowCount(1);
	_ui.tableWidgetMTheta->horizontalHeader()->hide();
	_ui.tableWidgetMTheta->verticalHeader()->setVisible(1);
	QString label("M_");
	label.append(QChar(952));
	label.append(" [deg];");
	_ui.tableWidgetMTheta->setVerticalHeaderLabels(label.split(";"));
	_ui.tableWidgetMTheta->custom_resize();

	_ui.tableWidgetMPhi->setColumnCount(1);
	_ui.tableWidgetMPhi->set_column_ratio(ratio);
	_ui.tableWidgetMPhi->set_vertical_header_width(1);
	_ui.tableWidgetMPhi->setRowCount(1);
	_ui.tableWidgetMPhi->horizontalHeader()->hide();
	_ui.tableWidgetMPhi->verticalHeader()->setVisible(1);
	label.clear();
	label.append("M_");
	label.append(QChar(966));
	label.append(" [deg];");
	_ui.tableWidgetMPhi->setVerticalHeaderLabels(label.split(";"));
	_ui.tableWidgetMPhi->custom_resize();
}

void ColorsWindow::setup_background_color(void)
{
	_ui.tableWidgetBackgroundColor->setColumnCount(3);
	_ui.tableWidgetBackgroundColor->setRowCount(1);
	_ui.tableWidgetBackgroundColor->verticalHeader()->hide();
	_ui.tableWidgetBackgroundColor->horizontalHeader()->setVisible(1);
	_ui.tableWidgetBackgroundColor->setHorizontalHeaderLabels(QString("r;g;b;color").split(";"));
	_ui.tableWidgetBackgroundColor->custom_resize();

	_ui.tableWidgetBackgroundColor->setItem(0, 0, new QTableWidgetItem());
	_ui.tableWidgetBackgroundColor->setItem(0, 1, new QTableWidgetItem());
	_ui.tableWidgetBackgroundColor->setItem(0, 2, new QTableWidgetItem());

	glm::vec4 color = _oglWidget->get_background_color();
	_ui.tableWidgetBackgroundColor->item(0, 0)->setText(QString::number(255*color.r));
	_ui.tableWidgetBackgroundColor->setItem(0, 1, new QTableWidgetItem(QString::number(255*color.g)));
	_ui.tableWidgetBackgroundColor->setItem(0, 2, new QTableWidgetItem(QString::number(255*color.b)));
}

void ColorsWindow::setup_spin_color(void)
{
	_ui.tableWidgetSpinColor->setColumnCount(4);
	_ui.tableWidgetSpinColor->setRowCount(1);
	_ui.tableWidgetSpinColor->verticalHeader()->hide();
	_ui.tableWidgetSpinColor->horizontalHeader()->setVisible(1);
	_ui.tableWidgetSpinColor->setHorizontalHeaderLabels(QString("r;g;b;color").split(";"));
	_ui.tableWidgetSpinColor->custom_resize();

	_ui.tableWidgetSpinColor->setItem(0, 0, new QTableWidgetItem());
	_ui.tableWidgetSpinColor->setItem(0, 1, new QTableWidgetItem());
	_ui.tableWidgetSpinColor->setItem(0, 2, new QTableWidgetItem());
	_ui.tableWidgetSpinColor->setItem(0, 3, new QTableWidgetItem());

	_ui.tableWidgetSpinColor->item(0, 3)->setFlags(Qt::NoItemFlags);
}

void ColorsWindow::read_spin_color(void)
{
	if (_ui.tableWidgetSpinColor->item(0, 0))
	{
		_spinColor.r = (float) _ui.tableWidgetSpinColor->item(0, 0)->text().toDouble()/255.;
	}
	if (_ui.tableWidgetSpinColor->item(0, 1))
	{
		_spinColor.g = (float) _ui.tableWidgetSpinColor->item(0, 1)->text().toDouble()/255.;
	}
	if (_ui.tableWidgetSpinColor->item(0, 2))
	{
		_spinColor.b = (float) _ui.tableWidgetSpinColor->item(0, 2)->text().toDouble()/255.;
	}
}

void ColorsWindow::setup_spin_model(void)
{
	_ui.tableWidget_SpinMesh->setColumnCount(5);
	_ui.tableWidget_SpinMesh->setRowCount(1);
	_ui.tableWidget_SpinMesh->verticalHeader()->hide();
	_ui.tableWidget_SpinMesh->horizontalHeader()->setVisible(1);
    _ui.tableWidget_SpinMesh->setHorizontalHeaderLabels(QString("n;r2/r1;l1/l2;r2/lg;scale").split(";"));
	_ui.tableWidget_SpinMesh->custom_resize();

	SpinMeshParams spinMeshParams = _oglWidget->_spinMeshParams;
	set_spin_model_params(spinMeshParams);
}

void ColorsWindow::update_display_color_table_widget_spin_color(void)
{
	_ui.tableWidgetSpinColor->item(0, 3)->setBackgroundColor(QColor((int) (_spinColor.x*255), 
		(int) (_spinColor.y*255), (int) (_spinColor.z*255)));
}


void ColorsWindow::setup_color_map(void)
{
	_ui.tableWidgetColorMap->setColumnCount(4);
	_ui.tableWidgetColorMap->setRowCount(4);
	_ui.tableWidgetColorMap->horizontalHeader()->setVisible(1);
	_ui.tableWidgetColorMap->verticalHeader()->setVisible(1);
	_ui.tableWidgetColorMap->setVerticalHeaderLabels(QString("<min;min;max;>max").split(";"));
	_ui.tableWidgetColorMap->setHorizontalHeaderLabels(QString("r;g;b;color").split(";"));
	_ui.tableWidgetColorMap->custom_resize();

	_ui.tableWidgetColorMap->setItem(0, 0, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(0, 1, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(0, 2, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(0, 3, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(1, 0, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(1, 1, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(1, 2, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(1, 3, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(2, 0, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(2, 1, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(2, 2, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(2, 3, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(3, 0, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(3, 1, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(3, 2, new QTableWidgetItem());
	_ui.tableWidgetColorMap->setItem(3, 3, new QTableWidgetItem());

	_ui.tableWidgetColorMap->item(0, 3)->setFlags(Qt::NoItemFlags);
	_ui.tableWidgetColorMap->item(1, 3)->setFlags(Qt::NoItemFlags);
	_ui.tableWidgetColorMap->item(2, 3)->setFlags(Qt::NoItemFlags);
	_ui.tableWidgetColorMap->item(3, 3)->setFlags(Qt::NoItemFlags);
}

void ColorsWindow::color_map_gray_scale(void)
{
	_ui.tableWidgetColorMap->item(0, 0)->setText("0.0");
	_ui.tableWidgetColorMap->item(0, 1)->setText("0.0");
	_ui.tableWidgetColorMap->item(0, 2)->setText("180");
	_ui.tableWidgetColorMap->item(1, 0)->setText("30");
	_ui.tableWidgetColorMap->item(1, 1)->setText("30");
	_ui.tableWidgetColorMap->item(1, 2)->setText("30");
	_ui.tableWidgetColorMap->item(2, 0)->setText("240");
	_ui.tableWidgetColorMap->item(2, 1)->setText("240");
	_ui.tableWidgetColorMap->item(2, 2)->setText("240");
	_ui.tableWidgetColorMap->item(3, 0)->setText("180");
	_ui.tableWidgetColorMap->item(3, 1)->setText("0.0");
	_ui.tableWidgetColorMap->item(3, 2)->setText("0.0");
}

void ColorsWindow::set_color_map_colors(std::vector<glm::vec4> colorMapColors)
{
	for (int i = 0; i < 4; i++)
	{
		_ui.tableWidgetColorMap->item(i, 0)->setText(QString::number(colorMapColors[i].r*255));
		_ui.tableWidgetColorMap->item(i, 1)->setText(QString::number(colorMapColors[i].g * 255));
		_ui.tableWidgetColorMap->item(i, 2)->setText(QString::number(colorMapColors[i].b * 255));
	}
}

void ColorsWindow::read_color_map_colors(void)
{
	for (int i = 0; i < 4; i++)
	{
		if (_ui.tableWidgetColorMap->item(i, 0))
		{
			_colorMapColors[i].r = (float) _ui.tableWidgetColorMap->item(i, 0)->text().toDouble()/255.;
		}
		if (_ui.tableWidgetColorMap->item(i, 1))
		{
			_colorMapColors[i].g = (float) _ui.tableWidgetColorMap->item(i, 1)->text().toDouble()/255.;
		}
		if (_ui.tableWidgetColorMap->item(i, 2))
		{
			_colorMapColors[i].b = (float) _ui.tableWidgetColorMap->item(i, 2)->text().toDouble()/255.;
		}
	}
}

void ColorsWindow::update_display_colors_table_widget_color_map(void)
{
	for (int i = 0; i < 4; i++)
	{
		_ui.tableWidgetColorMap->item(i, 3)->setBackgroundColor(QColor((int)(_colorMapColors[i].x*255), 
			(int)(_colorMapColors[i].y*255), (int) (_colorMapColors[i].z*255)));
	}
}
