/*
* GUIEnergyElements.h
*
*      
*/

#ifndef GUIENERGYELEMENTS_H
#define GUIENERGYELEMENTS_H

// Form include
#include "ui_mainWindow.h"

#include <memory>

#include <QObject>
#include <QSharedPointer>

class AnisotropyWindow;
class Configuration;

/// Management of GUI elements concerning energy parameters

class GUIEnergyElements: public QObject
{
Q_OBJECT
public:
	GUIEnergyElements(Ui::QtMainWindow* ui);

	void setup_elements(void); ///< setup energy elements
	void set_default_values(void); ///< set default energy parameter values
	void read_parameters(QSharedPointer<Configuration> &config); ///< energy parameters from GUI
	void open_anisotropy_window(void); ///< open additional window to specify anisotropy energies

	public slots:
	void on_push_button_neel(void);
	void on_push_button_chiral(void);
	void on_anisotropy_window_destroyed(void);

protected:
	Ui::QtMainWindow* _ui; ///< ui elements

	AnisotropyWindow* _anisoWindow; ///< window to specify anisotropy energies
	std::shared_ptr<std::vector<double>> _hexAnisotropies; ///< should contain three elements
	std::shared_ptr<std::vector<UniaxialAnisotropyStruct>> _anisotropies; ///< store anisotropy energies

	DMType _dmType; ///< chiral or Neel type
};

#endif // GUIENERGYELEMENTS_H


