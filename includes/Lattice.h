/*
* Lattices.h
*
*/

#ifndef CRYSTALLATTICES_H_
#define CRYSTALLATTICES_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include "typedefs.h"
#include "MyMath.h"
#include "Functions.h"

/// Extensive information about crystal lattice

class Lattice
{
public:
	Lattice(LatticeType latticeType, std::vector<int> latticeDimensions,
		int* millerIndexes, BoundaryConditions boundaryConditions);
	virtual ~Lattice();

	int parameter_consistency(void) const;

	void read_lattice(std::string fname);
	void create_lattice(void);

	void create_mask_read_in_shape(LatticeMaskParameters latticeMaskParams);

	void find_center_site(void);
	void neighbor_distances(void);
	void assign_neighbors(void);
	int is_neighbor(const int &pos1, const int &pos2) const;

	void show_lattice_coordinates(void) const;
	void show_neighbor_distances(void) const;
	void show_distance_neigh(void) const;
	void show_neighbors(const int &index) const;
	void show_skyrmion_number_cells(void) const;
	

	int* get_neighbor_array(const int &order) const;
	int get_number_nth_neighbors(const int &order) const;
	Threedim* get_lattice_coordinate_array(void) const;
	Threedim* get_neighbor_vector_array(const int &order) const;
	double* get_distance_array(void) const;
	int* get_distance_neigh(void) const;
	int get_radius_max(void) const;
	TopologicalChargeCell* get_skN_cells(void) const;
	int get_skN_cell_number(void) const;
	std::vector<Threedim> get_dummy_skN_triangles();
	Fourdim* get_four_spin_cells(void) const;
	int get_number_four_spin_cells_per_atom(void) const;
	int get_number_atoms(void) const;
	int get_center_site(void);
	std::vector<Threedim> get_wigner_seitz_cell(void) const;

	static std::vector<Threedim> get_sites_square_lattice(const int &m, const int &n);
	static std::vector<Threedim> get_sites_hexagonal_lattice(const int &m, const int &n);

	LatticeType _latticeType;
	std::vector<int> _latticeDimensions;
	int* _millerIndexes; 
	BoundaryConditions _boundaryConditions;

protected:
	void create_lattice_sc(int x, int y, int z);
	void create_lattice_bcc(int x, int y, int z);
	void create_lattice_fcc(int x, int y, int z);
	void create_miller_plane(void);
	void create_triangular_hexagonal_open(int n);
	void create_triangular_hexagonal_helical(int n);
	void create_triangular_triangular(int n);
	void create_triangular_half_disk(int r);
	void create_triangular_disk(int r);
	void create_triangular_arrow_head(int m, int n);
	void create_triangular_stripe(int m, int n);

	void finish_cutting_process(int* belongsToTriangle, int tmpnuat);

	Threedim* _latticeCoordArray; ///< lattice coordinates
	int _numberAtoms; ///< number of lattice sites
	int _numberNthNeighbors[8]; ///< number of nth neighors
	int _radiusMax; ///<  number of different atom distances (equals size of distanceArray)
	int _centerSite; ///< index of center lattice site
	double* _distanceArray; ///< all different atom distances
	int* _distanceNeigh;

	std::vector<Threedim> _wignerSeitzCell;

	TopologicalChargeCell* _skNcells; ///< cells to calculate topological charge.
	int _skNcellNum;
	std::vector<Threedim> _dummyskNTriangles;

	Fourdim* _fourSpinCells; ///< cells for four spin interaction; size = _fourSpinCellsPerAtom*_numberAtoms
	int _fourSpinCellsPerAtom; ///< number of cells per atom

	int* _firstNeighborArray; ///< indexes of first neighbor atoms
	Threedim* _firstNeighborVectorArray; // vectors pointing from lattice site to all neighbors
	int* _secondNeighborArray; ///< indexes of second neighbor atoms
	Threedim* _secondNeighborVectorArray; // vectors pointing from lattice site to all neighbors
	int* _thirdNeighborArray; ///< indexes of third neighbor atoms
	Threedim* _thirdNeighborVectorArray; // vectors pointing from lattice site to all neighbors
	int* _fourthNeighborArray; ///< indexes of fourth neighbor atoms
	Threedim* _fourthNeighborVectorArray; // vectors pointing from lattice site to all neighbors
	int* _fifthNeighborArray; ///< indexes of fifth neighbor atoms
	Threedim* _fifthNeighborVectorArray; // vectors pointing from lattice site to all neighbors
	int* _sixthNeighborArray; ///< indexes of sixth neighbor atoms
	int* _seventhNeighborArray; ///< indexes of seventh neighbor atoms
	int* _eighthNeighborArray; ///< indexes of eigth neighbor atoms
};

#endif /* LATTICES_H_ */
