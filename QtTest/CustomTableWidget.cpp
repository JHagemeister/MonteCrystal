/*
* CustomTableWidget.cpp
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

#include "CustomTableWidget.h"

#include <numeric>

#include <QHeaderView>
#include <QScrollBar>

CustomTableWidget::CustomTableWidget(QWidget *parent) :
	QTableWidget(parent)
{
	_height = 0;
	_width = 0;

	setFocusPolicy(Qt::NoFocus);
	verticalHeader()->setHighlightSections(false);
	horizontalHeader()->setHighlightSections(false);
	setEditTriggers(QAbstractItemView::AllEditTriggers);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CustomTableWidget::~CustomTableWidget()
{
}

void CustomTableWidget::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	custom_resize();
}

void CustomTableWidget::set_column_ratio(std::vector<int> columnRatio)
{
	/**
	* @param[in] columnRatio column width ratios
	*/
	_columnRatio = columnRatio;
}

void CustomTableWidget::set_row_ratio(std::vector<int> rowRatio)
{
	/**
	* @param[in] rowRatio row height ratios
	*/
	_rowRatio = rowRatio;
}

void CustomTableWidget::set_horizontal_header_height(int height)
{
	/**
	* @param[in] height Horizontal header height ratio.
	*/
	_height = height;
}

void CustomTableWidget::set_vertical_header_width(int width)
{
	/**
	* @param[in] width Vertical header width ration.
	*/
	_width = width;
}

void CustomTableWidget::custom_resize(void)
{
	/**
	* Apply specified ratios.
	*/

	int width = 0;

	// number of column ratio values and columns should be identical
	if (_columnRatio.size() == (size_t)columnCount()) 
	{
		double sum = std::accumulate(_columnRatio.begin(), _columnRatio.end(), 0);
		double factor = 0;
		if (sum > 0 || width > 0)
		{
			factor = this->width() / (_width + sum);
		}
		verticalHeader()->setFixedWidth(_width*factor);
		for (int i = 0; i < columnCount(); i++)
		{
			width = factor * _columnRatio[i];
			setColumnWidth(i, width);
		}
	}
	else
	{
		if (!verticalHeader()->isHidden())
		{
			width = this->width() / (columnCount() + 1);
			verticalHeader()->setFixedWidth(width);
		}
		else
		{
			width = this->width() / columnCount();
		}

		for (int i = 0; i < columnCount(); i++)
		{
			setColumnWidth(i, width);
		}
	}

	int height = 0;
	if (_rowRatio.size() == (size_t)rowCount())
	{
		double factor = _height + std::accumulate(_rowRatio.begin(), _rowRatio.end(), 0);
		if (factor > 0)
		{
			factor = this->height() / factor;
		}
		else 
		{
			factor = 0;
		}
		horizontalHeader()->setFixedHeight(_height*factor);
		for (int i = 0; i < rowCount(); i++)
		{
			height = factor * _rowRatio[i];
			setRowHeight(i, height);
		}
	}
	else
	{
		if (!horizontalHeader()->isHidden())
		{
			height = this->height() / (rowCount() + 1);
			horizontalHeader()->setFixedHeight(height);
		}
		else
		{
			height = this->height() / rowCount();
		}

		for (int i = 0; i < rowCount(); i++)
		{
			setRowHeight(i, height);
		}
	}
}
