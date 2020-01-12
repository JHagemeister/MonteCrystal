/*
* ThreeSiteInteraction.h
*
*  Created on: 29.07.2014
*
*/

#ifndef THREESPININTERACTION_H_
#define THREESPININTERACTION_H_

#include "Energy.h"

#include "typedefs.h"

/// Higher-order exchange interaction between three neighboring spins

class ThreeSpinInteraction : public Energy
{
public:
    ThreeSpinInteraction(Threedim* spinArray, double energyParameter, ThreeSite* cells, int nCellsPerAtom);
       virtual ~ThreeSpinInteraction();
       virtual double single_energy(const int &position) const;
       virtual Threedim effective_field(const int &position) const;

protected:
       int const _nCellsPerAtom; ///< number of three-site cells per atom
       ThreeSite const * const _cells; ///< three-site cells consisting of 3 lattice site indices
};

#endif /* THREESPININTERACTION_H_ */