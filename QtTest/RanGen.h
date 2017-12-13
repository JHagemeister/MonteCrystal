/*
* Random.h
*
*  Created on: 14.03.2012
*      
*
* This class is abstract. It is intended as a base for various classes
* to create random numbers in different ways.
*/

#ifndef RANGEN_H_
#define RANGEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <vector>

/// Basis class for pseudo random number generators

class RanGen {
public:
	RanGen(void);
	virtual ~RanGen(void);
	/// obtain pseudo random number within interval
	virtual int IRandom(int min, int max) = 0;
	/// pseudo random number in [0,1]
	virtual double Random(void) = 0;
	/// random permutation
	virtual void Shuffle(std::vector<int> &vector) = 0;
	/// two  pseudo random numbers according to standard normal distribution (mu = 0 and sigma = 1)
	void polar(double &x1, double &x2);

};

#endif /* RANGEN_H_ */
