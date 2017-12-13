/*
* Hamiltonian.cpp
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

#include "Hamiltonian.h"

// forward and further includes
#include "MyMath.h"
#include <iostream>

Hamiltonian::Hamiltonian(std::vector<std::shared_ptr<Energy>> energies, int numberAtoms) 
{
	/**
	* @param[in] energies Energy objects.
	* @param[in] numberAtoms Number of lattice sites.
	*/

	_energies = energies;
	_numberAtoms = numberAtoms;
}

Hamiltonian::~Hamiltonian() 
{
}

double Hamiltonian::single_energy(int position)
{
	/**
	* This function evaluates the TOTAL energy of a single atom AND of the bonds that it contributes to.
	*
	* This is, however, NOT the energy of the atom.
	*
	* @param[in] _position lattice site
	*
	* @return Total energy of the single atom.
	*/
	double energy = 0;
	for (std::vector<std::shared_ptr<Energy> >::iterator it = _energies.begin(); it != _energies.end(); ++it)
	{
		energy += (*it)->single_energy(position);
	}
	return energy;
}

double Hamiltonian::single_interaction_energy_between_two_spins(int position1, int position2)
{
	/**
	* This function evaluates the total interaction energy of a single
	* atom with its neighbors.
	*
	* @param[in] _position lattice site
	*
	* @return The total energy of the single atom.
	*/
	double energy = 0;
	for (std::vector<std::shared_ptr<Energy> >::iterator it = _energies.begin(); it != _energies.end(); ++it)
	{
		if ((*it)->get_factor() < 0.9)
		{
			energy += (*it)->interaction_energy_between_two_spins(position1, position2);
		}
	}
	return energy;
}

double Hamiltonian::single_non_interaction_energy(int position)
{
	double energy = 0;
	for (std::vector<std::shared_ptr<Energy> >::iterator it = _energies.begin(); it != _energies.end(); ++it)
	{
		if ((*it)->get_factor() > 0.9)
		{
			energy += (*it)->single_energy(position);
		}
	}
	return energy;
}

double Hamiltonian::total_energy(void)
{
	/**
	* This function evaluates the total energy of the system. The energy is not
	* normalized to the number of atoms.
	*
	* @return The total energy of the system.
	*/
	double totalEnergy = 0;
	// summation over all energies.
	for (int i = 0; i < _energies.size(); ++i)
	{
		totalEnergy += part_energy(i);
	}
	return totalEnergy;
}

double Hamiltonian::total_energy(int position)
{
	double totalEnergy = 0;
	// summation over all energies.
	for (int i = 0; i < _energies.size(); ++i)
	{
		totalEnergy += single_part_energy(i, position);
	}
	return totalEnergy;
}

double Hamiltonian::part_energy(std::shared_ptr<Energy> &energy)
{
	/**
	* This function evaluates the energy of the whole system for a given term of the Hamiltonian.
	*
	* @param[in] energy Energy object to calculate the energy with.
	*
	* @return The energy.
	*/
	double partEnergy = 0;
	for (int i = 0; i < _numberAtoms; ++i)
	{
		partEnergy += energy->single_energy(i);
	}
	// The multiplication by get_factor() takes care of double summations.
	return (double)partEnergy * energy->get_factor();
}

double Hamiltonian::part_energy(int index)
{
	/**
	* This function evaluates the energy of the whole system for a given term of the Hamiltonian.
	*
	* @param[in] index Index identifying the energy object to calculate the energy with.
	*
	* @return The energy.
	*/
	double partEnergy = 0;
	if (index > -1 && index < _energies.size())
	{
		for (int i = 0; i < _numberAtoms; ++i)
		{
			partEnergy += _energies[index]->single_energy(i);
		}
	}
	else
	{
		std::cout << "Index out of bounds in HeisenbergHamiltonian::SinglePartEnergy(int index)" << std::endl;
	}
	// The multiplication by get_factor() takes care of double summations.
	return (double)partEnergy * _energies[index]->get_factor();
}

double Hamiltonian::single_part_energy(int index, int position)
{
	/**
	* This function evaluates part of the energy of a given atom. The energy object used for the 
	* energy calculation is identified by index parameter.
	*
	* @param[in] index Index to identify energy object with.
	* @param[in] position Index of an atom.
	*
	* @return The energy.
	*/
	double partEnergy = 0;
	if (index > -1 && index < _energies.size())
	{
		partEnergy = _energies[index]->single_energy(position)
			* _energies[index]->get_factor();
	}
	else
	{
		std::cout << "Index out of bounds in HeisenbergHamiltonian::SinglePartEnergy(int _index)" <<std::endl;
	}
	return partEnergy;
}

void Hamiltonian::set_spin_array(Threedim* spinArray)
{
	/**
	* @param[in] spinArray Pointer to a spin array
	*/

	for (int i = 0; i < _energies.size(); i++)
	{
		_energies[i]->set_spin_array(spinArray);
	}
}

Threedim Hamiltonian::effectiveField(int position)
{
	/**
	*  @param[in] position The effective field by all energies acting on spin with index "position"
	*/

	Threedim field = { 0,0,0 };
	for (int i = 0; i < _energies.size(); i++)
	{
		field = MyMath::add(field,_energies[i]->effective_field(position));
	}
	return field;
}

std::vector<std::shared_ptr<Energy>> Hamiltonian::get_energies(void) const
{
	return _energies;
}

int Hamiltonian::get_number_energies(void) const
{
	return _energies.size();
}