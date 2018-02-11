/*
* Energy.h
*
*      
*
*      Energy is an abstract class. Classes that inherit from Energy are
*      used to evaluate the lattice energy.
*/

#ifndef ENERGY_H_
#define ENERGY_H_

// standard includes
#include <string>

// own
#include "typedefs.h"

/// Basis class for energies

class Energy {
public:
	Energy(double factor, std::string stringID, Threedim* spinArray, double energyParameter);
	virtual ~Energy(void);
	
	/// energy of single spin/ total energy of bonds connected to a spin
	/** @param[in] position Index of lattice site @return The energy of the single atom.*/
	virtual double single_energy(const int &position) const = 0;

	/// effective field acting on a spin
	virtual Threedim effective_field(const int &position) const = 0;
	
	/// return member _factor
	double get_factor(void) const;
	/// return member _stringID
	std::string get_string_id(void) const;
	/// return member _energyParameter
	double get_energy_parameter(void) const;
	
	/// set member _energyParameter
	void set_energy_parameter(double energyParameter);
	/// set member _spinArray
	void set_spin_array(Threedim* spinArray);

protected:
	double _factor; ///< correction factor due to double/multiple summations
	std::string _stringID; ///< name of energy object
	Threedim* _spinArray; ///< spin configuration
	double _energyParameter; ///< energy parameter [meV]
};

#endif /* ENERGY_H_ */
