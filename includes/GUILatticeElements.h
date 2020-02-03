/*
* GUILatticeElements.h
*
*      
*/

#ifndef GUILATTICEELEMENTS_H
#define GUILATTICEELEMENTS_H

#include <QObject>
#include <QString>
#include <QSharedPointer>

#include <typedefs.h>

class MainWindow;

class LatticeMaskWindow;

class Configuration;

/// Management of GUI elements concerning lattice parameters

class GUILatticeElements: public QObject
{
Q_OBJECT
public:
	GUILatticeElements(MainWindow* ui);
	~GUILatticeElements();

	LatticeMaskParameters _latticeMaskParameters; ///< parameters defining lattice mask bitmap read in

	void setup_elements(void); ///< setup GUI elements for lattice parameters 
	void set_default_values(void); ///< set default values for lattice parameters
	void read_parameters(const std::shared_ptr<Configuration> &config); ///< lattice parameters from GUI
	void update_to_lattice_type(QString qString); ///< adjust GUI if different lattice type is selected

	void show_lattice_mask_window(void); ///< open window for lattice mask bitmap parameter specification

protected:
	MainWindow* _mw; ///< ui elements

	LatticeMaskWindow* _latticeMaskWindow; ///< pop-up window to get parameters for lattice mask bitmap read in

	// update ui elements for lattice parameters according to the currently chosen crystal type
	/// adjust GUI element to specify lattile dimensions if different lattice type is selected
	void update_lattice_dimensions_to_lattice_type(QString qString);
	/// adjust GUI element to specify boundary conditions if different lattice type is selected
	void update_boundary_conditions_to_lattice_type(QString qString);
};

#endif // GUILATTICEELEMENTS_H


