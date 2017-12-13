#ifndef SKYRMION_H
#define SKYRMION_H

#include <qdialog.h>
#include "ui_skyrmion.h"

#include "typedefs.h"

class GUISpinElements;

/// Window to add skyrmions to the current spin configuration
class SkyrmionWindow : public QDialog
{
	Q_OBJECT

public:
	SkyrmionWindow(GUISpinElements* guiSpinElements, SkyrmionWindowParameters parameters, QWidget* parent = Q_NULLPTR);
	~SkyrmionWindow();

public slots:
	void on_push_button_apply(void); ///< update spin configuration to add specified skyrmion
	void on_push_button_undo(void); ///< reset initial spin configuration
	void receive_skyrmion_center(Twodim center); ///< receive skyrmion center postition from mouse click

private:
	void read_parameters(void); ///< read parameters for skyrmion setup

	Ui::skyrmion _ui;
	GUISpinElements* _guiSpinElements;

	SkyrmionWindowParameters _parameters;
};

#endif // SKYRMION_H