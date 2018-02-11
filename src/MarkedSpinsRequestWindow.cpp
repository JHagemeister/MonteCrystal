/*
* MarkedSpinsRequestWindow.cpp
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

#include "MarkedSpinsRequestWindow.h"

#include "MarkedSpinsHandler.h"
#include "MyMath.h"

#include <QStringListModel>
#include <QStringList>

MarkedSpinsRequestWindow::MarkedSpinsRequestWindow(MarkedSpinsHandler* markedSpinsHandler, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	_markedSpinsHandler = markedSpinsHandler;
	
	ui.tableWidget_direction->setColumnCount(3);
	ui.tableWidget_direction->setRowCount(1);
	ui.tableWidget_direction->verticalHeader()->hide();
	ui.tableWidget_direction->horizontalHeader()->setVisible(1);
	ui.tableWidget_direction->setHorizontalHeaderLabels(QString("sx;sy;sz").split(";"));
	ui.tableWidget_direction->custom_resize();
	ui.tableWidget_direction->setItem(0, 0, new QTableWidgetItem());
	ui.tableWidget_direction->setItem(0, 1, new QTableWidgetItem());
	ui.tableWidget_direction->setItem(0, 2, new QTableWidgetItem());

	ui.tableWidget_JDefect->setColumnCount(2);
	ui.tableWidget_JDefect->setRowCount(1);
	ui.tableWidget_JDefect->verticalHeader()->hide();
	ui.tableWidget_JDefect->horizontalHeader()->setVisible(1);
	ui.tableWidget_JDefect->setHorizontalHeaderLabels(QString("J_order;param.").split(";"));
	ui.tableWidget_JDefect->custom_resize();
	ui.tableWidget_JDefect->setItem(0, 0, new QTableWidgetItem());
	ui.tableWidget_JDefect->setItem(0, 1, new QTableWidgetItem());

	ui.tableWidget_DDefect->setColumnCount(2);
	ui.tableWidget_DDefect->setRowCount(1);
	ui.tableWidget_DDefect->verticalHeader()->hide();
	ui.tableWidget_DDefect->horizontalHeader()->setVisible(1);
	ui.tableWidget_DDefect->setHorizontalHeaderLabels(QString("D_order;param.").split(";"));
	ui.tableWidget_DDefect->custom_resize();
	ui.tableWidget_DDefect->setItem(0, 0, new QTableWidgetItem());
	ui.tableWidget_DDefect->setItem(0, 1, new QTableWidgetItem());

	
	QString qString("<i>K</i>sin<font size=4><sup>2</sup></font size=4>" + QString(QChar(920)));
	ui.textEdit_KDefect->setText(qString);

	ui.tableWidget_KDefect->setColumnCount(4);
	ui.tableWidget_KDefect->setRowCount(1);
	ui.tableWidget_KDefect->verticalHeader()->hide();
	ui.tableWidget_KDefect->horizontalHeader()->setVisible(1);
	ui.tableWidget_KDefect->setHorizontalHeaderLabels(QString("k;ex;ey;ez").split(";"));
	ui.tableWidget_KDefect->custom_resize();
	ui.tableWidget_KDefect->setItem(0, 0, new QTableWidgetItem());
	ui.tableWidget_KDefect->setItem(0, 1, new QTableWidgetItem("0"));
	ui.tableWidget_KDefect->setItem(0, 2, new QTableWidgetItem("0"));
	ui.tableWidget_KDefect->setItem(0, 3, new QTableWidgetItem("1"));

	connect(ui.pushButton_lock, &QAbstractButton::released, this, &MarkedSpinsRequestWindow::lock_button);
	connect(ui.pushButton_unlock, &QAbstractButton::released, this, &MarkedSpinsRequestWindow::unlock_button);

	connect(ui.tableWidget_direction, &QTableWidget::cellChanged, 
		this, &MarkedSpinsRequestWindow::table_direction);
	
	connect(ui.pushButton_showDefects, &QAbstractButton::released, 
		this, &MarkedSpinsRequestWindow::button_show_defects);
	connect(ui.pushButton_showLockedSpins, &QAbstractButton::released, 
		this, &MarkedSpinsRequestWindow::button_show_locked_spins);

	connect(ui.pushButton_undo, &QAbstractButton::released, 
		this, &MarkedSpinsRequestWindow::button_undo);
}

MarkedSpinsRequestWindow::~MarkedSpinsRequestWindow()
{
}

void MarkedSpinsRequestWindow::get_exchange_defect_parameters(int & order, double & parameter) const
{
	if (ui.tableWidget_JDefect->item(0, 0))
	{
		order = ui.tableWidget_JDefect->item(0, 0)->text().toInt();
	}
	if (ui.tableWidget_JDefect->item(0, 1))
	{
		parameter = ui.tableWidget_JDefect->item(0, 1)->text().toDouble();
	}
}

void MarkedSpinsRequestWindow::get_dm_defect_parameters(int & order, double & parameter) const
{
	if (ui.tableWidget_DDefect->item(0, 0))
	{
		order = ui.tableWidget_DDefect->item(0, 0)->text().toInt();
	}
	if (ui.tableWidget_DDefect->item(0, 1))
	{
		parameter = ui.tableWidget_DDefect->item(0, 1)->text().toDouble();
	}
}

void MarkedSpinsRequestWindow::get_anisotropy_defect_parameters(UniaxialAnisotropyStruct & anisotropy) const
{
	if (ui.tableWidget_KDefect->item(0, 0))
	{
		anisotropy.energyParameter = ui.tableWidget_KDefect->item(0, 0)->text().toDouble();
	}
	else
	{
		anisotropy.energyParameter = 0;
	}
	if (ui.tableWidget_KDefect->item(0, 1))
	{
		anisotropy.direction.x = ui.tableWidget_KDefect->item(0, 1)->text().toDouble();
	}
	else
	{
		anisotropy.direction.x = 0;
	}
	if (ui.tableWidget_KDefect->item(0, 2))
	{
		anisotropy.direction.y = ui.tableWidget_KDefect->item(0, 2)->text().toDouble();
	}
	else
	{
		anisotropy.direction.y = 0;
	}
	if (ui.tableWidget_KDefect->item(0, 3))
	{
		anisotropy.direction.z = ui.tableWidget_KDefect->item(0, 3)->text().toDouble();
	}
	else
	{
		anisotropy.direction.z = 0;
	}
}

void MarkedSpinsRequestWindow::set_indexes(std::vector<int>& indexes)
{
	QStringListModel* model = new QStringListModel();
	QStringList list;
	for (auto it = indexes.begin(); it != indexes.end(); ++it)
	{
		list << QString::number(*it);
	}
	model->setStringList(list);
	QAbstractItemModel* modelOld = ui.listView_indexes->model();
	ui.listView_indexes->setModel(model);
	delete modelOld;
}

void MarkedSpinsRequestWindow::clear_tables(void)
{
	ui.tableWidget_KDefect->item(0,0)->setText("");
	ui.tableWidget_DDefect->item(0, 0)->setText("");
	ui.tableWidget_DDefect->item(0, 1)->setText("");
	ui.tableWidget_JDefect->item(0, 0)->setText("");
	ui.tableWidget_JDefect->item(0, 1)->setText("");
}


void MarkedSpinsRequestWindow::unlock_button(void)
{
	_markedSpinsHandler->unlock_spins();
}

void MarkedSpinsRequestWindow::table_direction(int row, int column)
{
	Threedim direction{ 0,0,0 };
	if (ui.tableWidget_direction->item(0, 0))
	{
		direction.x = ui.tableWidget_direction->item(0, 0)->text().toDouble();
	}
	if (ui.tableWidget_direction->item(0, 1))
	{
		direction.y = ui.tableWidget_direction->item(0, 1)->text().toDouble();
	}
	if (ui.tableWidget_direction->item(0, 2))
	{
		direction.z = ui.tableWidget_direction->item(0, 2)->text().toDouble();
	}

	if (MyMath::norm(direction) > PRECISION)
	{
		_markedSpinsHandler->set_direction(MyMath::normalize(direction));
	}
}

void MarkedSpinsRequestWindow::button_show_defects(void)
{
	_markedSpinsHandler->show_defects();
}

void MarkedSpinsRequestWindow::button_show_locked_spins(void)
{
	_markedSpinsHandler->show_locked_spins();
}

void MarkedSpinsRequestWindow::button_undo(void)
{
	_markedSpinsHandler->clear_defects_and_unlock_all();
}

void MarkedSpinsRequestWindow::lock_button(void)
{
	_markedSpinsHandler->lock_spins();
}