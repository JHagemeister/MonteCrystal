/*
* LatticeMaskWindow.h
*
*
*/

#ifndef LATTICEMASKWINDOW_H
#define LATTICEMASKWINDOW_H

class GUILatticeElements;

#include <QDialog>
#include "ui_latticemaskwindow.h"

class LatticeMaskWindow : public QDialog
{
	Q_OBJECT

public:
	LatticeMaskWindow(GUILatticeElements* guiLatticeElements, QWidget *parent = Q_NULLPTR);
	~LatticeMaskWindow();

	public slots:
	void ok_button(void);

private:
	Ui::LatticeMaskWindow ui;

	void read_parameters(void);

	LatticeMaskParameters _latticeMaskParameters;

	GUILatticeElements* _guiLatticeElements;
};

#endif // LATTICEMASKWINDOW_H
