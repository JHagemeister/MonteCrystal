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
	double single_energy(const int &position) const;
	virtual Threedim effective_field(const int &position) const;

	int get_nbors(void) const;
	int* get_neighbor_array(void) const;

protected:
	int _nbors; ///< number of neighbors per spin
	int* _neighborArray; ///< indexes of neighbors
};

#endif /* EXCHANGEINTERACTION_H_ */
