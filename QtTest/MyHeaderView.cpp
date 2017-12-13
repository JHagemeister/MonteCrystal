/*
* MyHeaderView.cpp
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

#include "MyHeaderView.h"

#include <qpainter.h>
#include <qtextdocument.h>
#include <qstatictext.h>

MyHeaderView::MyHeaderView(Qt::Orientation orientation, double leftMargin, double topMargin, QWidget *parent):
	QHeaderView(orientation)
{
	_xMargin = leftMargin;
	_yMargin = topMargin;
}

MyHeaderView::~MyHeaderView()
{
}

void MyHeaderView::paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const
{
	if (!rect.isValid())
		return;

	QStyleOptionHeader opt;
	initStyleOption(&opt);

	opt.rect = rect;
	opt.section = logicalIndex;
	opt.text = this->model()->headerData(logicalIndex, this->orientation(), Qt::DisplayRole).toString();
	opt.textAlignment = Qt::AlignCenter;

	// the section position

	int visual = visualIndex(logicalIndex);

	Q_ASSERT(visual != -1);

	if (count() == 1)
		opt.position = QStyleOptionHeader::OnlyOneSection;

	else if (visual == 0)
		opt.position = QStyleOptionHeader::Beginning;

	else if (visual == count() - 1)
		opt.position = QStyleOptionHeader::End;

	else
		opt.position = QStyleOptionHeader::Middle;

	QTextDocument TextDoc;
	QString tmp = opt.text;
	opt.text = "";   //IMPORTANT!

					 // draw the section
	style()->drawControl(QStyle::CE_Header, &opt, painter, this);

	painter->save();

	QRect textRect = style()->subElementRect(QStyle::SE_HeaderLabel, &opt, this);

	int x = 0;
	int y = 0;

	QPoint point1 = textRect.topLeft();
	QPoint point2 = textRect.center();

	int len = tmp.size();
	
	x = point1.x() + _xMargin*(point2.x()-point1.x());
	y = point1.y() + _yMargin*(point2.y()-point1.y());

	QPoint point3(x,y);

	painter->translate(point3);

	TextDoc.setDocumentMargin(0);
	TextDoc.setHtml(tmp);
	TextDoc.drawContents(painter, QRect(QPoint(0, 0), textRect.size()));

	painter->restore();
}