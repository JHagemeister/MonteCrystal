/*
* WindingNumber.h
*
*  Created on: 03.02.2013
*      
*/

#ifndef WINDINGNUMBER_H_
#define WINDINGNUMBER_H_

#include <iostream>
#include <sstream> 

#include "typedefs.h"

#include "Observable.h"

/// Winding/skyrmion number 

class WindingNumber : public Observable
{
public:
	WindingNumber(int numberMeasurements, Threedim* spinArray, TopologicalChargeCell* cells, int cellNum);
	virtual ~WindingNumber();
	virtual std::string get_steps_header(void) const;
	virtual std::string get_mean_header(void) const;
	virtual void take_value(void);
	virtual std::string get_step_value(const int &index) const;
	virtual std::string get_mean_value(const double &temperature) const;
	virtual void clear_storage(void);
	
	void winding_number(double &value); ///< evaluate skyrmion number of system
	void evaluate_local_winding_number();
	double* get_local_winding_number(void) const; ///< calculate winding number resolved to lattice sites

protected:
	Threedim const * const _spinArray; ///< spin configuration
	TopologicalChargeCell const * const _cells; ///< skyrmion cells (consisting of 3 sites each)
	int const _cellNum; ///< number of skyrmion cells
	double* _windingNumber; ///< storage for skyrmion number
	double* _localWindingNumber; ///< storage for lattice resolved winding number
	double _areaUnitSphere; 
};

#endif /* WINDINGNUMBER_H_ */
