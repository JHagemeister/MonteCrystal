/*
* SpinOrientation.h
*
* This class is intended as the basis class for different spin systems.
* Inheriting classes may implement the Heisenberg, XY and Ising model.
*
*      
*/


#ifndef SPINORIENTATION_H_
#define SPINORIENTATION_H_

// standard includes
#include <memory>
#include <string>

// own
#include "typedefs.h"

class Lattice;
class RanGen;

/// Basis class for spin systems

class SpinOrientation {
public:
	SpinOrientation(int numberAtoms, std::shared_ptr<RanGen> ranGen);
	virtual ~SpinOrientation();
	
	// abstract virtual methods

	/// set random spin configuration
	virtual void random_orientation(void) = 0;
	/// set a spin spiral state
	virtual void set_spin_spiral(Threedim spiralR, Threedim spiralI, Threedim ek, double lambda,  
		Lattice* lattice) = 0;
	/// set a spin spiral state
	virtual void set_spin_spiral(Threedim kVector, Threedim position, int helicity, Lattice* lattice) = 0;
	/// set ferromagnetic state
	virtual void set_ferromagnet(Threedim direction) = 0;
	/// obtain magnetization
	virtual Threedim magnetisation(void) const = 0;
	/// perform random reorientation of spin direction
	virtual void single_orientation(int position) = 0;
	/// restore latest random reorientation
	virtual void restore_single_orientation(void) = 0;
	
	/// read spin configuraiton from file
	void read_spin_configuration(std::string fname);

	/// show spin directions on console
	void show_spin_configuraion(void) const;
	
	/// activate spin for update during simulation
	void set_active_site(int position);
	/// inactivate spin for update during simulation
	void set_inactive_site(int position);
	/// activate all spins for update during simulation
	void set_all_sites_active(void);
	/// inactivate all spins for update during simulation
	void set_all_sites_inactive(void);
	
	/// set spin direction of single spin
	void set_spin(Threedim spin, int position);

	/// get pointer to spin array
	Threedim* get_spin_array(void) const;
	/// get number of spins
	int get_number_atoms(void) const;
	/// get direction of single spin
	Threedim get_spin(int position) const;
	/// get indexes of spins active for update during simulation
	int* get_active_sites(void) const;
	/// get number of spins active for update during simulation
	int get_number_active_sites(void) const;
	/// get indexes of spins inactive for update during simulation
	int* get_inactive_sites(void) const;
	/// get number of sites inactive for update during simulation
	int get_number_inactive_sites(void) const;
	/// get information about activity status of each spin
	int* get_activity_list() const;

protected:

	int _numberAtoms; ///<number of atoms
	std::shared_ptr<RanGen> _ranGen;  ///< random number generator
	Threedim* _spinArray; ///< array containing spin orientations

	int* _activeSites; ///< indexes of lattice sites which are updated during simulation
	int _numberActiveSites; ///< number of active lattice sites

	int* _inactiveSites; ///< indexes of lattice sites which are not updated during simulation
	int _numberInactiveSites; ///< number of inactive lattice sites

	Threedim _spin;  ///< to store latest changed spin
	int _position; ///< index of latest changed spin
};

#endif /* SPINORIENTATION_H_ */
