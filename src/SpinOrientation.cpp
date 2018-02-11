/*
* SpinOrientation.cpp
*
* Copyright 2017 Julian Hagemeister
*
* This file is part of MonteCrystal.
*
* MonteCrystal is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* MonteCrystal is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with MonteCrystal.  If not, see <http://www.gnu.org/licenses/>.
*
*
* This class is intended as the basis class for different spin systems.
* Inheriting classes may implement the Heisenberg, XY and Ising model.
*
*      
*/

#include "SpinOrientation.h"

// forward includes
#include "Lattice.h"
#include "RanGen.h"
#include "Functions.h"

SpinOrientation::SpinOrientation(int numberAtoms, std::shared_ptr<RanGen> ranGen)
{
	/**
	* Constructor of base class for spin configurations.
	*
	* @param[in] numberAtoms Number of lattice sites
	* @param[in] ranGen Pseudo random number generator
	*/
	
	// member initialization
	_ranGen = ranGen;
	_numberAtoms = numberAtoms;

	// array with the spin configurations
	_spinArray = NULL;
	// indexes of active sites that shall be updated during simulation
	_activeSites = NULL;
	// number of active sites
	_numberActiveSites = 0;
	// indexes of inactive sites that go into the energy calculation but are not updated during simulation
	_inactiveSites = NULL;
	// number of inactive sites
	_numberInactiveSites = 0;
	
	// store a single spin in cache. needed for Monte Carlo single spin update mechanism
	// position of lattice site on which latest trial change is performed
	_position = 0;
	// old spin direction at trial step lattice site
	_spin = { 0, 0, 0 };
}

SpinOrientation::~SpinOrientation()
{
	delete[] _spinArray;
	delete[] _activeSites;
	delete[] _inactiveSites;
}

void SpinOrientation::read_spin_configuration(std::string fname)
{
	/**
	* Read an existing spin configuration. The sx, sy and sz values must be in columns 2-4. Column 1 is 
	* reserved for additional information. It can e.g. be a simple enumeration with successive natural number 
	* values. In this case it has no impact. If it contains series of the values 0 and 1 only, the spins
	* with value 1 will be updated during simulations while spins with value 0 will be excluded from update
	* during simulation.
	*
	* Caution: The file to read should have as many lines as atoms.
	*
	* @param[in] fname The name of the file containing the spin configuration to be read in
	*/

	// assume that there are as many lines as number of atoms
	_numberAtoms = Functions::get_num_lines(fname);

	// make room for the new spins
	delete[] _spinArray;
	_spinArray = new Threedim[_numberAtoms];

	// boolean parameter about whether active site information is provided in first column 
	// (either 0 or 1 for every lattice site)
	int activeInformation = TRUE;
	// temporary array to store active site information
	int* activeSitesTmp = new int[_numberAtoms];
	
	// open file
	std::ifstream infile(fname);
	std::string line;

	// temporary helper value for read in
	std::string parameter;

	// keep track of current row index
	int count = 0;

	if (!infile)
	{
		std::cout << "The spin file does not exist!" << std::endl;
		return;
	}
	while (std::getline(infile, line))
	{
		std::istringstream isstream(line);

		// obtain first value of line
		parameter.clear();
		isstream >> parameter;
		// if not 0 or 1, set boolean value active information to zero, all sites will be set active as a 
		// consequence
		if (parameter.compare("0") != 0 && parameter.compare("1") != 0)
		{
			activeInformation = FALSE;
		}
		activeSitesTmp[count] = std::stoi(parameter);

		// Sx value
		parameter.clear();
		isstream >> parameter;
		_spinArray[count].x = std::stof(parameter);

		// Sy value
		parameter.clear();
		isstream >> parameter;
		_spinArray[count].y = std::stof(parameter);

		// Sz value
		parameter.clear();
		isstream >> parameter;
		_spinArray[count].z = std::stof(parameter);

		_spinArray[count] = MyMath::normalize(_spinArray[count]);

		// increase index of row number
		count += 1;
	}
	// close file
	infile.close();

	// check information about active/inactive sites
	if (activeInformation == FALSE)
	{
		// if no consistent information was provided, set all sites active
		set_all_sites_active();
	}
	else
	{
		// Determine number of active sites
		_numberActiveSites = 0;
		for (int i = 0; i < _numberAtoms; i++)
		{
			if (activeSitesTmp[i] == 1)
			{
				_numberActiveSites += 1;
			}
		}

		// derive number of inactive sites
		_numberInactiveSites = _numberAtoms - _numberActiveSites;

		// make room for active and inactive sites indexes
		delete[] _activeSites;
		_activeSites = new int[_numberActiveSites];
		delete[] _inactiveSites;
		_inactiveSites = new int[_numberInactiveSites];

		// helper counters
		int activeSiteCount = 0;
		int inactiveSiteCount = 0;

		// check for all lattice sites whether site is active or inactive
		for (int i = 0; i < _numberAtoms; i++)
		{
			if (activeSitesTmp[i] == 1)
			{
				// site active
				_activeSites[activeSiteCount] = i;
				activeSiteCount += 1;
			}
			else
			{
				// site inactive
				_inactiveSites[inactiveSiteCount] = i;
				inactiveSiteCount += 1;
			}
		}
	}

	delete[] activeSitesTmp;
}

void SpinOrientation::show_spin_configuraion(void) const
{
	/**
	* Show spin orientations on console.
	*/

	for (int i = 0; i < _numberAtoms; ++i)
	{
		std::cout << i << " " << _spinArray[i].x << " " << _spinArray[i].y << " " << _spinArray[i].z
			<< std::endl;
	}
}

void SpinOrientation::set_active_site(int position)
{
	/**
	* If spin with index "position" is active anyway, nothing happens. Otherwise spin with index "position" is
	* set active for update during simulation.
	*
	* @param[in] position Index of spin to be set active
	*/

	if (position < _numberAtoms && position > -1)
	{
		if (_inactiveSites != NULL)
		{
			MyMath::remove_value(_inactiveSites, _numberInactiveSites, position);
			MyMath::add_value(_activeSites, _numberActiveSites, position);
		}
		else
		{
			set_all_sites_inactive();
			set_active_site(position);
		}
	}
	else
	{
		std::cout << "ERROR: Call SpinOrientation::set_active_site(int _position) with invalid position value"
			<< std::endl;
	}
}

void SpinOrientation::set_inactive_site(int position)
{
	/**
	* If spin with index "position" is inactive anyway, nothing happens. Otherwise spin with index "position" 
	* is set inactive for update during simulation.
	*
	* @param[in] position Index of spin to be set inactive
	*/

	if (position < _numberAtoms && position > -1)
	{
		if (_activeSites != NULL)
		{
			MyMath::remove_value(_activeSites, _numberActiveSites, position);
			MyMath::add_value(_inactiveSites, _numberInactiveSites, position);
		}
		else
		{
			set_all_sites_active();
			set_inactive_site(position);
		}
	}
	else
	{
		std::cout << "ERROR: Call SpinOrientation::set_inactive_site(int _position) with invalid position  \
			value"
			<< std::endl;
	}
}

void SpinOrientation::set_all_sites_active(void)
{
	/**
	* Set all spins active for update during simulation
	*/

	delete[] _inactiveSites;
	_inactiveSites = NULL;
	_numberInactiveSites = 0;

	delete[] _activeSites;
	_activeSites = new int[_numberAtoms];
	_numberActiveSites = _numberAtoms;

	for (int i = 0; i < _numberAtoms; i++)
	{
		_activeSites[i] = i;
	}
	
}

void SpinOrientation::set_all_sites_inactive(void)
{
	/**
	* Set all sites inactive for update during simulation
	*/

	delete[] _inactiveSites;
	_inactiveSites = new int[_numberAtoms];;
	_numberInactiveSites = _numberAtoms;

	delete[] _activeSites;
	_activeSites = NULL;
	_numberActiveSites = 0;

	for (int i = 0; i < _numberAtoms; i++)
	{
		_inactiveSites[i] = i;
	}

}

void SpinOrientation::set_spin(Threedim spin, int position)
{
	_spinArray[position] = spin;
}

Threedim* SpinOrientation::get_spin_array(void) const
{
	return _spinArray;
}

int SpinOrientation::get_number_atoms(void) const
{
	return _numberAtoms;
}

Threedim SpinOrientation::get_spin(int position) const
{
	return _spinArray[position];
}

int* SpinOrientation::get_active_sites(void) const
{
	return _activeSites;
}

int SpinOrientation::get_number_active_sites(void) const
{
	return _numberActiveSites;
}

int* SpinOrientation::get_inactive_sites(void) const
{
	return _inactiveSites;
}

int SpinOrientation::get_number_inactive_sites(void) const
{
	return _numberInactiveSites;
}

int* SpinOrientation::get_activity_list() const
{
	int *siteList = new int[_numberAtoms];
	for (int i = 0; i < _numberAtoms; ++i)
	{
		siteList[i] = 0; // inactive site
	}
	for (int i = 0; i < _numberActiveSites; ++i)
	{
		siteList[_activeSites[i]] = 1;
	}
	return siteList;
}