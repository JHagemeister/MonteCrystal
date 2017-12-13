/*
* Hamiltonian.h
*
*  Created on: 15.03.2012
*      
*/

#ifndef HAMILTONIAN_H_
#define HAMILTONIAN_H_

#include <memory>
#include <vector>

#include "typedefs.h"

#include "Energy.h"

/// Hamiltonian for energy calculations

class Hamiltonian {
public:
	Hamiltonian(std::vector<std::shared_ptr<Energy>> energies, int numberAtoms);
	virtual ~Hamiltonian();

	double single_energy(int position);
	double single_interaction_energy_between_two_spins(int position1, int position2);
	double single_non_interaction_energy(int position);
	double total_energy();
	double total_energy(int position);
	double part_energy(std::shared_ptr<Energy> &energy);
	double part_energy(int index);
	double single_part_energy(int index, int position);

	Threedim effectiveField(int position);

	void set_spin_array(Threedim* spinArray);
	
	std::vector<std::shared_ptr<Energy>> get_energies(void) const;
	int get_number_energies(void) const;

protected:
	std::vector<std::shared_ptr<Energy>> _energies; ///< energy objects
	int _numberAtoms; ///< equal to number of lattice sites
};

#endif /* HAMILTONIAN_H_ */
