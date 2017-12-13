/*
* Setup.h
*
*      
*
* This class is intended to set up lattice, spin configuration and Hamiltonian object and energy objects.
* Furthermore it can be used to manipulate energy parameters during a simulation like for example the tip
* position or strength and direction of the external magnetic field.
*/

#ifndef SETUP_H_
#define SETUP_H_

#include <memory>
#include <qsharedpointer.h>

#include "typedefs.h"

class Configuration;
class RanGen;

class Hamiltonian;
class Energy;
class ZeemanEnergy;
class Tip;

class Measurement;
class Observable;

class Lattice;
class SpinOrientation;

/// Manage Lattice, SpinOrientation, Hamiltonian and Measurement objects

class Setup
{
public:
	Setup(QSharedPointer<Configuration> config);
	virtual ~Setup();
	
	// creation of lattice, spinOrientation, Hamiltonian and Energy objects
	void create_crystal_lattice(void);
	void read_crystal_lattice(std::string fname);
	void create_crystal_lattice_from_mask(void);
	void create_spin_orientation(std::shared_ptr<RanGen> ranGen);
	void setup_measurement(void);
	void setup_hamiltonian(void);
	void setup_energies(void);
	
	// manipulation of external magnetic field
	void set_magnetic_field(double H);
	void set_magnetic_field(Threedim direction);
	
	// manipulation of tip parameters as position and magnetization direction
	void set_tip_position(Threedim position);
	void set_tip_strength(double energyParam);
	void set_tip_direction(Threedim tipDirection);

	QSharedPointer<Lattice> _lattice;
	QSharedPointer<SpinOrientation> _spinOrientation;
	std::shared_ptr<Measurement> _measurement;
	QSharedPointer<Hamiltonian> _hamilton;
	std::vector<std::shared_ptr<Energy>> _energies;

private:
	void setup_exchange_interaction(void);
	void setup_DM_interaction(void);
	void setup_biquadratic_interaction(void);
	void setup_four_spin_interaction(void);
	void setup_anisotropy_energy(void);
	void setup_dipol_interaction(void);
	void setup_modulated_exchange_interaction(void);
	void setup_modulated_anisotropy_energy(void);
	void setup_zeeman_energy(void);
	void setup_magnetic_tip(void);
	void setup_exchange_defects(void);
	void setup_dm_defects(void);
	void setup_anisotropy_defects(void);

	QSharedPointer<Configuration> _config;
	
	std::vector<std::shared_ptr<Observable>> observables;
	std::shared_ptr<ZeemanEnergy> _zeemanEnergy;
	std::shared_ptr<Tip> _tipEnergy;
};

#endif /* SETUP_H_ */
