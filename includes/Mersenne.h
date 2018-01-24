/*
* Mersenne.h
*
* Mersenne twister pseudo random number generator
*
*/


#ifndef MERSENNE_H_
#define MERSENNE_H_

#include "RanGen.h"
#include <random>

class Mersenne : public RanGen
{
public:
	Mersenne(int seed);
	virtual ~Mersenne();
	virtual int IRandom(int min, int max);
	virtual double Random(void);
	virtual void Shuffle(std::vector<int> &vector);

private:
	std::mt19937 _mt;
};


#endif // MERSENNE_H_
