/*
* ThreeSiteInteraction.cpp
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

#include "ThreeSpinInteraction.h"

// further includes
#include "MyMath.h"

#include <iostream>

ThreeSpinInteraction::ThreeSpinInteraction(Threedim* spinArray, double energyParameter, ThreeSite* cells,
       int nCellsPerAtom):
       Energy(1.0/3.0, "E_3Site ", spinArray, energyParameter), _cells(cells),
       _nCellsPerAtom(nCellsPerAtom)
{
       /**
       * @param[in] spinArray Pointer to spin configuration.
       * @param[in] energyParameter Energy parameter [meV]
       * @param[in] cells Three-site cells
       * @param[in] nCellsPerAtom Number of three-site cells per lattice site
       */
}

ThreeSpinInteraction::~ThreeSpinInteraction()
{
}

double ThreeSpinInteraction::single_energy(const int &position) const
{
    double energy = 0;
    for (int i = 0; i < _nCellsPerAtom; ++i)
    {
        if (_cells[position * _nCellsPerAtom + i].i > -0.5)
        {
            if (position == 0) {
                std::cout << _cells[position * _nCellsPerAtom + i].i << " " << _cells[position * _nCellsPerAtom + i].j
                    << " " << _cells[position * _nCellsPerAtom + i].k << std::endl;
            }
            Threedim mi = _spinArray[_cells[position * _nCellsPerAtom + i].i];
            Threedim mj = _spinArray[_cells[position * _nCellsPerAtom + i].j];
            Threedim mk = _spinArray[_cells[position * _nCellsPerAtom + i].k];
            energy += MyMath::dot_product(mi, mj) * MyMath::dot_product(mi, mk)
                + MyMath::dot_product(mj, mi) * MyMath::dot_product(mj, mk)
                + MyMath::dot_product(mk, mi) * MyMath::dot_product(mk, mj);
        }
    }
    return -_energyParameter * energy;
}

Threedim ThreeSpinInteraction::effective_field(const int &position) const
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

                       dotProd = MyMath::dot_product(mi,mk);
                       field = MyMath::add(field,MyMath::mult(mj, dotProd));

                       dotProd = MyMath::dot_product(mi,mj);
                       field = MyMath::add(field,MyMath::mult(mk, dotProd));

                       dotProd = MyMath::dot_product(mj,mk);
                       field = MyMath::add(field,MyMath::mult(MyMath::add(mj,mk), dotProd));
               }
       }
       return MyMath::mult(field, _energyParameter);
}
