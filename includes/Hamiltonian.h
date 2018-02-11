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

	double single_energy(const int &position) const;
	double total_energy() const;
	double total_energy(const int &position) const;
	double part_energy(std::shared_ptr<Energy> &energy)  const;
	double part_energy(const int &index) const;
	double single_part_energy(const int &index,const int &position) const;

	Threedim effectiveField(const int &position) const;

	void set_spin_array(Threedim* spinArray);
	
	std::vector<std::shared_ptr<Energy>> get_energies(void) const;
	int get_number_energies(void) const;

protected:
	std::vector<std::shared_ptr<Energy>> _energies; ///< energy objects
	int _numberAtoms; ///< equal to number of lattice sites
};

#endif /* HAMILTONIAN_H_ */
