/*
* typedefs.h
*
* 
*
*/

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <glm/vec3.hpp>

#include <string>

#define FALSE   0
#define TRUE    1

#define PRECISION 0.000001

//Constants
#define kB 0.0861733  // [meV/K]
#define Pi 3.14159265 // pi
#define muBohr 0.057883818 /// [meV/T]
#define gammaElectron 1.760859644*pow(10,11) // [rad/(T s)]

//typedef unsigned int Uint;
//
//typedef long long int Llong; // 64 bit integer
//typedef unsigned long long int Ullong;


/// Simulation method - Monte Carlo or spin dynamics
enum SimulationType
{
	metropolis, landauLifshitzGilbert, converger1
};

/// Select an "experiment". Here, one could also specify new purposes of the program.
enum ProgramType
{
	temperatureMagneticFieldLoop, spinSeebeck, tipMovement, latticeSiteEnergies,
	latticeSiteWindingNumber, Experiment01, EigenFrequency, readLatticeConfiguration, readSpinConfiguration,
	saveLatticeConfiguration, saveSpinConfiguration, latticeMaskRead
};

/// Specification of lattice type.
enum LatticeType
{
	simpleCubic, bodyCenteredCubic, faceCenteredCubic, triangularHexagonal, triangularTriangular,
	triangularHalfDisk, triangularDisk, triangularArrowHead, triangularStripe, squareLattice, hexagonalLattice
};

/// Possible boundary conditions. Not all conditions are compatible with all lattice types.
enum BoundaryConditions
{
	openBound, helical, periodic, periodicX, periodicY
};

/// Spin model
enum SpinType
{
	Heisenberg, Ising
};

/// Used to identify which kind of color map shall be plotted onto the GUI
enum ColorMapType
{
	NoMap, TotalEnergyMap, SingleEnergyMap, AnisotropyEnergyMap, MagnetizationMap,
	CustomDirectionMagnetizationMap, TopologicalChargeMap
};

/// Helper struct with three double values
struct Threedim
{
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
};

/// Helper struct to store information about a cell for the calculation of local topological charge
struct TopologicalChargeCell
{
	int i; ///< lattice site index
	int j; ///< lattice site index
	int k; ///< lattice site index
	int triangleIndex; ///< index to indentify dummy triangle in Lattice class
	Threedim position; ///< spatial position of triangle
};

struct LatticeMaskParameters
{
	std::string fname; ///< file name for read in of mask. bitmap!
	LatticeType latticeType; ///< 2D: square, hexagonal...
	double width; ///< width of read in image in multiples of lattice constants
	double height; ///< height of read in image in multiples of lattice constants
};

/// Helper struct with four int values
struct Fourdim
{
	int i;
	int j;
	int k;
	int l;
};

/// Helper struct with two double values
struct Twodim
{
	double x;
	double y;
};

/// Exchange interaction 
/** specified by energy and order parameter */
struct ExchangeEnergyStruct
{
	double energyParameter; ///< energy parameter [meV]
	int order; ///< 1 for nearest-neighbor interaction, 2 for second-nearest ...
};

enum DMType
{
	Neel, Chiral
};

/// Uniaxial anisotropy energy
struct UniaxialAnisotropyStruct
{
	double energyParameter; ///< energy parameter [meV]
	Threedim direction; ///< orientation of anisotropy axis
};

/// Spatially modulated exchange energy
struct ModulatedExchangeEnergyStruct
{
	double Jx; ///< energy parameter along x-axis [meV]
	double Jy; ///< energy parameter along y-axis [meV]
	double lambda; ///< modulation period
	double alpha; ///< modulation amplitude [rad]
	int modulationNumber; ///< modulation number
};

/// Spatially modulated anisotropy energy
struct ModulatedAnisotropyEnergyStruct
{
	double energyParameter; ///< energy parameter [meV]
	double lambda; ///< modulaiton period
	double width;
	Threedim direction;
	int modulationNumber;
};

/// Temperature parameters
struct TemperatureStruct
{
	double start;
	double end;
	int steps;
	Threedim direction;
};

/// External magnetic field
/** defines constant magnetic field or loop over magnetic field*/
struct MagneticFieldStruct
{
	double start; ///< start value of magnetic field [meV]
	double end; ///< end value of magnetic field [meV]
	int steps; ///< steps for magnetic loop over magnetic fields
	Threedim direction; ///< spatial orientation of homogeneous magnetic field
};

/// Magnetic tip (e.g. scanning tunneling microscope)
struct MagneticTipStruct
{
	Threedim start; ///< start position of tip
	Threedim end; ///< end position of tip
	Threedim magnetizationDirection; ///< magnetization direction
	double energyParameter; ///< energy parameter for tip
	int steps; ///< steps for movement of tip from start to end position
	int stepWidth; ///< output interval
};

/// Additional parameters for Experiment01 simulation
struct Experiment01Struct
{
	double freq; ///< rotation frequency of spins at edge
};

struct ExcitationFrequencyParameters
{
	int* index; ///< index of current eigenstate to display
	double* maxAngle; ///< angle to customize amplitude of excitation
	int* rescale; ///< boolean to trigger rescaling of excitation visualization
	int* pause; ///< boolean to pause vizualization of visualization
	int* videoSequence; ///< boolean to trigger output of video (png) sequence
	int* terminate; ///< boolean to terminate visualization
	double* sleepTime; ///< time in [ms] waited during two visualization steps
	double* segmentationThreshold;
};

struct SkyrmionWindowParameters
{
	Twodim center; ///< skyrmion center coordinates
	int centerDir; ///< -1 for skyrmion center parallel +z, +1 for skyrmion center -z
	double radius; ///< skyrmion radius; here defined as region of linear rotation by pi
	int chiral; ///< 1 for chiral, 0 for Neel skyrmion
	int handed; ///< 1 for right; -1 for left ?
	int order; ///< k-pi skyrmion
};

struct SimulationProcedureParameters
{
	int simulationSteps;
	int outWidth;
	int uiWidth;
	double alpha;
	double deltaT;
};

/// Polar angle and azimuthal angle
struct Angle
{
	double theta; ///< polar angle
	double phi; ///< azimuthal angle
};

/// Vertex for openGL
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

struct SpinMeshParams
{
	int n;
	float r2divr1;
	float l1divl2;
	float r2divl2;
	float scale;
};


#endif /* TYPEDEFS_H_ */
