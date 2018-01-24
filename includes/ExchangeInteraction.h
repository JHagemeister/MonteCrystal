/*
* ExchangeInteraction.h
*      
*/

#ifndef EXCHANGEINTERACTION_H_
#define EXCHANGEINTERACTION_H_

#include "Energy.h"

#include "typedefs.h"

/// Exchange interaction between two spins

class ExchangeInteraction : public Energy {
public:
	ExchangeInteraction(Threedim* spinArray, double energyParameter, int* neighborArray, int nbors, 
		std::string order);
	virtual ~ExchangeInteraction();
	double single_energy(int position);
	virtual double interaction_energy_between_two_spins(int position1, int position2);

	virtual Threedim effective_field(int position);

	int get_nbors(void) const;
	int* get_neighbor_array(void) const;

protected:
	int _nbors; ///< number of neighbors per spin
	int* _neighborArray; ///< indexes of neighbors
};

#endif /* EXCHANGEINTERACTION_H_ */
