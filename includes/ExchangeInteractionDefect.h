/*
* ExchangeInteractionDefect.h
*
*/

#ifndef EXCHANGEINTERACTIONDEFECT_H_
#define EXCHANGEINTERACTIONDEFECT_H_

#include "Energy.h"

#include "typedefs.h"

#include <unordered_map>
#include <vector>

/// Exchange interaction between two spins

class ExchangeInteractionDefect : public Energy {
public:
	ExchangeInteractionDefect(Threedim* spinArray, int* neighborArray, int nbors, const std::unordered_map<int,double> &defects,
		std::string order);
	virtual ~ExchangeInteractionDefect();
	double single_energy(int position);
	virtual double interaction_energy_between_two_spins(int position1, int position2);

	virtual Threedim effective_field(int position);

protected:
	void setup_energy_bonds(const std::unordered_map<int, double> &defects);
	int _nbors; ///< number of neighbors per spin
	int* _neighborArray; ///< indexes of neighbors

	std::unordered_map<int, std::vector<double>> _bondEnergies;
};

#endif /* EXCHANGEINTERACTION_H_ */
