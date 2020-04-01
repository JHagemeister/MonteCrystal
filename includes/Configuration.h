/*
* ReadConfig.h
*
*  Created on: 28.10.2012
*      
*/

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "typedefs.h"
#include "MyMath.h"

/// Class containing all parameters for simulation

class Configuration
{
public:
	Configuration::Configuration();
	std::string all_parameters(void) const; ///< make a single string from all parameter values
	void show_parameters(void); ///< show parameters on console
	void determine_outputfolder_needed(void); ///< determine whether output folder is needed
	void copy_configuration_file(std::string fname); ///< copy configuration file

	// Simulation parameters
	SimulationType _simulationType; ///< e.g. Metropolis, Landau-Lifshtiz-Gilbert (LLG) ...
	ProgramType _programType; ///< purpose of simulation program e.g. temperature and magnetic field loop
	Experiment01Struct _experiment01;

	// lattice parameters:
	LatticeType _latticeType; ///< lattice type as e.g. simpleCubic, bodyCenteredCubic ...
	std::vector<int> _latticeDimensions; ///< lattice size parameters; depends on _latticeType
	int _millerIndexes[3]; ///< Miller indexes to obtain Miller plane
	BoundaryConditions _boundaryConditions; ///< open, periodic ...; depends on _latticeType
	double _latticeConstant; ///< lattice constant in [Angstrom]
	double _magneticMoment; ///< magnetic moment per spin in multiples of muBohr
	LatticeMaskParameters _latticeMaskParameters; ///< parameters for bitmap mask read in
	
	// spin setup
	SpinType _spinSystem; ///< Heisenberg, Ising ...
	Threedim _initialSpiralR;
	Threedim _initialSpiralI;
	Threedim _initialSpiralek;
	double _initialSpiralLambda;
	double _gaussianSpinSamplingSigma; ///< sigma for random spin change restricted to cone

	// energy parameters
	std::vector<ExchangeEnergyStruct> _exchangeEnergies; 
	std::vector<ExchangeEnergyStruct> _DMEnergies;
	DMType _dmType;
	double _pseudoDipolarEnergy;
	double _biQuadraticEnergy;
	double _fourSpinEnergy;
	double _threeSiteEnergy;
	std::vector<UniaxialAnisotropyStruct> _uniaxialAnisotropyEnergies;
	std::vector<double> _hexagonalAnisotropyEnergies;
	int _dipolEnergy; ///< 0 for no dipol-dipol energy; 1 for dipol-dipol energy
	std::vector<ModulatedExchangeEnergyStruct> _modulatedExchangeEnergies;
	std::vector<ModulatedAnisotropyEnergyStruct> _modulatedAnisotropyEnergies;
	MagneticFieldStruct _magneticField;
	MagneticTipStruct _magneticTip;
	std::unordered_map<int, std::unordered_map<int, double>> _exchangeDefects;
	std::unordered_map<int, std::unordered_map<int, double>> _dmDefects;
	std::unordered_map<int, UniaxialAnisotropyStruct> _anisotropyDefects;

	// parameters for Landau-Lifshitz-Gilbert simulation
	double _LLG_dampingParameter; ///< Gilbert damping parameter. 1 for fastest relaxation. common value 0.1
	double _LLG_timeWidth; ///< time width for one solving step of LLG differential equation [ps]

	// LLG, Monte Carlo mutual parameters 
	int _seed; ///< seed to initialize pseudo random number generator
	double _temperatureStart; ///< start temperature for temperature loop
	double _temperatureEnd; ///< end temperature for temperature loop
	int _temperatureSteps; ///< steps for temperature loop
	Threedim _temperatureGradientDirection; ///< direction of temperature gradient
	int _outputWidth; ///< every _outputWidth simulation steps energy or magnetization etc. values are taken
	int _simulationSteps; ///< number of simulation steps for each set of temperature and magnetic field

	// Excitation Solver parameters
	int _numEigenStates;

	// parameters for output configuration
	bool _doOutput = false; ///< 1 if output into folder wanted, 0 if not output into folder wanted
	bool _doSpinConfigOutput = false; ///< spin configuration output at end of each temperature and magnetic field step
	int _movieStart; ///< simulation step to begin spin configuration output
	int _movieEnd; ///< simulation step to stop spin configuration output
	int _movieWidth;///< every _movieWidth steps between (_movieStart, _movieEnd) output of spin configuration 
	bool _doSimulationStepsOutput = false; ///< save all measurement values taken as a function of simulation step
	bool _doEnergyOutput = false; ///< save energy values, heat capacity as a function of temperature and mgnetic field step
	bool _doMagnetisationOutput = false; ///< save magnetization values, susceptibility as function of temperature and magnetic field
	bool _doAbsoluteMagnetisationOutput = false; ///< save absolute magnetization values as function of temperature and magnetic field
	bool _doNCMROutput = false;
	bool _doSpinResolvedOutput = false; ///< save information for each spin
	bool _doWindingNumberOutput = false; ///< save skyrmion number
	std::string _storageFname; ///< a file name that can be used for data output

	// parameters UI output
	int _uiUpdateWidth; ///< update GUI output every _uiUpdateWidth simulation steps
};

#endif /* CONFIG_H_ */
