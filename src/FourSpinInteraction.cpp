/*
* FourSpinInteraction.cpp
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

#include "FourSpinInteraction.h"

// further includes
#include "MyMath.h"

FourSpinInteraction::FourSpinInteraction(Threedim* spinArray, double energyParameter, Fourdim* cells, 
	int nCellsPerAtom):
	Energy(0.25, "E_4Spin ", spinArray, energyParameter), _cells(cells), 
	_nCellsPerAtom(nCellsPerAtom)
{
	/**
	* @param[in] spinArray Pointer to spin configuration.
	* @param[in] energyParameter Energy parameter [meV]
	* @param[in] cells Four spin cells
	* @param[in] nCellsPerAtom Number of Four spin cells per lattice site
	*/
}

FourSpinInteraction::~FourSpinInteraction()
{
}

double FourSpinInteraction::single_energy(int position)
{
	double energy = 0;
	for (int i = 0; i < _nCellsPerAtom; ++i)
	{
		if (_cells[position * _nCellsPerAtom + i].i > -0.5)
		{
			Threedim mi = _spinArray[_cells[position * _nCellsPerAtom + i].i];
			Threedim mj = _spinArray[_cells[position * _nCellsPerAtom + i].j];
			Threedim mk = _spinArray[_cells[position * _nCellsPerAtom + i].k];
			Threedim ml = _spinArray[_cells[position * _nCellsPerAtom + i].l];
			energy += MyMath::dot_product(mi, mj) * MyMath::dot_product(mk, ml)
				+ MyMath::dot_product(mi, ml) * MyMath::dot_product(mj, mk)
				- MyMath::dot_product(mi, mk) * MyMath::dot_product(mj, ml);

		}
	}
	return -_energyParameter * energy;
}

double FourSpinInteraction::interaction_energy_between_two_spins(int position1, int position2)
{
	return 0;
}

Threedim FourSpinInteraction::effective_field(int position)
{
	Threedim field = { 0,0,0 };
	double dotProd = 0;
	for (int i = 0; i < _nCellsPerAtom; ++i)
	{
		if (_cells[position * _nCellsPerAtom + i].i > -0.5)
		{
			Threedim mi = _spinArray[_cells[position * _nCellsPerAtom + i].i];
			Threedim mj = _spinArray[_cells[position * _nCellsPerAtom + i].j];
			Threedim mk = _spinArray[_cells[position * _nCellsPerAtom + i].k];
			Threedim ml = _spinArray[_cells[position * _nCellsPerAtom + i].l];

			dotProd = MyMath::dot_product(mk,ml);
			field = MyMath::add(field,MyMath::mult(mj, dotProd));

			dotProd = MyMath::dot_product(mj,mk);
			field = MyMath::add(field,MyMath::mult(ml,dotProd));

			dotProd = -MyMath::dot_product(mj,ml);
			field = MyMath::add(field,MyMath::mult(mk, dotProd));
		}
	}
	return MyMath::mult(field,_energyParameter);
}