/*
* Measurement.h
*
*  Created on: 20.11.2012
*      
*/

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "Observable.h"

/// Coordinate measurements on system

class Measurement {
public:
	Measurement(std::vector<std::shared_ptr<Observable>> observables);
	virtual ~Measurement();
	
	/// make room for measurement data in observables
	void set_number_measurements(int numMeasurements);
	/// tell observables to perform measurement
	void measure(void);
	/// calculate mean value of measurement data accquired through measure()
	void take_mean_values(std::string variable, double temperature);
	/// reset observables' measurement indexes to 0
	void reset_observables_measurement_index(void);

	/// save measurement values accquired thorugh measure()
	template <typename T>
	void save_step_values(std::string fname, std::string stepName, T simStepWidth);
	/// save mean measurement values
	void save_mean_steps(std::string fname, std::string variableName);

protected:
	std::string _meanBody; ///< for storage of mean measurement data of one simulation run
	std::vector<std::shared_ptr<Observable>> _observables; ///< observables for measurements on spin system
};



template <typename T> void Measurement::save_step_values(std::string fname, std::string stepHeader,
	T simStepWidth)
{
	/**
	* This function stores the data as a function of the Monte Carlo Step
	*  into a file.
	*
	* @param[in] fname The name of the output file name
	* @param[in] stepHeader The name of the steps. e.g. MCStep or LLGStep
	* @param[in] simStepWidth The step width between two successive measurements
	*/

	int numObservables = _observables.size();
	if (numObservables > 0)
	{
		// only store something if there are any observables
		// file stream for storage
		std::fstream filestr;
		filestr.open(fname, std::fstream::out);

		// provide the name of the variable(s)
		filestr << stepHeader << " ";
		// provide the names of the observables
		for (int i = 0; i < numObservables; ++i)
		{
			filestr << _observables[i]->get_steps_header();
		}
		filestr << "\n";

		int numMeasurements = _observables[0]->get_measurement_index();
		for (int i = 0; i < numMeasurements - 1; ++i)
		{
			filestr << simStepWidth * (i+1) << " ";
			for (int j = 0; j < numObservables; ++j)
			{
				filestr << _observables[j]->get_step_value(i);
			}
			filestr << std::endl;
		}
		filestr << simStepWidth * numMeasurements << " ";
		for (int j = 0; j < numObservables; ++j)
		{
			filestr << _observables[j]->get_step_value(numMeasurements - 1);
		}
		filestr.close();
	}
}

#endif /* MEASUREMENT_H_ */
