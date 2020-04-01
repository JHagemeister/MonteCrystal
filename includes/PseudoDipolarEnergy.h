/*
* PseudoDipolarEnergy.h
*      
*/

#ifndef PSEUDODIPOLARENERGY_H_
#define PSEUDODIPOLARENERGY_H_

#include "Energy.h"

#include "typedefs.h"

class PseudoDipolarEnergy : public Energy {
public:
	PseudoDipolarEnergy(Threedim* spinArray, double energyParameter, int* neighborArray, int nbors, 
		Threedim* neighborVectorArray);
	virtual ~PseudoDipolarEnergy();
	double single_energy(const int &position) const;
	virtual Threedim effective_field(const int &position) const;

	int get_nbors(void) const;
	int* get_neighbor_array(void) const;

protected:
	int _nbors; ///< number of neighbors per spin
	int* _neighborArray; ///< indexes of neighbors
	Threedim* _neighborVectorArray;
};

#endif /* PSEUDODIPOLARENERGY_H_ */
