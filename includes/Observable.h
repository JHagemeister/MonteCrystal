/*
* Observable.h
*
*  Created on: 20.11.2012
*      
*/

#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_

#include <string>

/// Basis class for observables 
/**
Examples for observables are energy and magnetization.
*/

class Observable {
public:
	Observable(int numberMeasurements);
	virtual ~Observable();

	/// header for output of values as function of simulation step
	virtual std::string get_steps_header(void) const = 0;
	/// header for output of mean values and heat capacity
	virtual std::string get_mean_header(void) const = 0;
	/// store observable value for current spin configuration
	virtual void take_value(void) = 0;
	/// get observable value at certain measurement step
	/** @param[in] index measurement index */
	virtual std::string get_step_value(const int &index) const = 0;
	/// get averaged observale values
	/** @param[in] temperature Temperature in [K] */
	virtual std::string get_mean_value(const double &temperature) const = 0;
	/// clear storage and reset _measurement index
	virtual void clear_storage(void) = 0;

	void set_measurement_index(const int &measurementIndex);
	void set_number_measurements(const int &numberMeasurements);
	
	int get_measurement_index(void) const;
	int get_num_measurements(void) const;

protected:
	int _measurementIndex; ///< current measurement index
	int _numberMeasurements; ///< number of measurement values before memory is full
};

#endif /* OBSERVABLE_H_ */

