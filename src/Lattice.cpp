/*
* Lattice.cpp
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
*/

#include "Lattice.h"

#include <CImg/CImg.h>

Lattice::Lattice(LatticeType latticeType, std::vector<int> latticeDimensions, int* millerIndexes, 
	BoundaryConditions boundaryConditions)
{
	_latticeType = latticeType;
	_latticeDimensions = latticeDimensions;
	_millerIndexes = millerIndexes;
	_boundaryConditions = boundaryConditions;

	
	_latticeCoordArray = NULL;
	_numberAtoms = 0;
	for (int i = 0; i < 8; ++i) 
	{
		_numberNthNeighbors[i] = 0;
	}
	_radiusMax = 0;
	_centerSite = -1;
	_distanceArray = NULL;
	_distanceNeigh = NULL;

	_skNcells = NULL;
	_skNcellNum = 0;

	_fourSpinCells = NULL;
	_fourSpinCellsPerAtom = 0;

	_threeSiteCells = NULL;
	_threeSiteCellsPerAtom = 0;

	_firstNeighborArray = NULL;
	_firstNeighborVectorArray = NULL;
	_secondNeighborArray = NULL;
	_secondNeighborVectorArray = NULL;
	_thirdNeighborArray = NULL;
	_thirdNeighborVectorArray = NULL;
	_fourthNeighborArray = NULL;
	_fourthNeighborVectorArray = NULL;
	_fifthNeighborArray = NULL;
	_fifthNeighborVectorArray = NULL;
	_sixthNeighborArray = NULL;
	_seventhNeighborArray = NULL;
	_eighthNeighborArray = NULL;
}

Lattice::~Lattice()
{
	delete[] _distanceArray;
	delete[] _distanceNeigh;
	delete[] _latticeCoordArray;
	delete[] _skNcells;
	delete[] _fourSpinCells;
	delete[] _threeSiteCells;

	delete[] _firstNeighborArray;
	delete[] _firstNeighborVectorArray;
	delete[] _secondNeighborArray;
	delete[] _secondNeighborVectorArray;
	delete[] _thirdNeighborArray;
	delete[] _thirdNeighborVectorArray;
	delete[] _fourthNeighborArray;
	delete[] _fourthNeighborVectorArray;
	delete[] _fifthNeighborArray;
	delete[] _fifthNeighborVectorArray;
	delete[] _sixthNeighborArray;
	delete[] _seventhNeighborArray;
	delete[] _eighthNeighborArray;
}

int Lattice::parameter_consistency(void) const
{
	/**
	* Checks if parameters given in constructer are suitable for lattice creation.
	*
	* @return 0 if not consistent; 1 if consistent
 	*/

	int consistency = TRUE;
	switch (_latticeType) {
	case simpleCubic:
		if (_latticeDimensions.size() != 3) 
		{
			consistency = FALSE;
		}
		break;
	case bodyCenteredCubic:
	case faceCenteredCubic:
		if (_latticeDimensions.size() != 3)
		{
			consistency = FALSE;
		}
		if (_boundaryConditions != openBound)
		{
			consistency = FALSE;
		}
		break;

	case triangularHexagonal:
		if (_latticeDimensions.size() != 1)
		{
			consistency = FALSE;
		}
		if (_boundaryConditions != openBound || _boundaryConditions != helical)
		{
			consistency = FALSE;
		}
		break;
	case triangularTriangular:
		if (_latticeDimensions.size() != 1)
		{
			consistency = FALSE;
		}
		break;
	case triangularHalfDisk:
		if (_latticeDimensions.size() != 1)
		{
			consistency = FALSE;
		}
		break;
	case triangularDisk:
		if (_latticeDimensions.size() != 1)
		{
			consistency = FALSE;
		}
		break;
	case triangularArrowHead:
		if (_latticeDimensions.size() != 3)
		{
			consistency = FALSE;
		}
		break;
	case triangularStripe:
		if (_latticeDimensions.size() != 2)
		{
			consistency = FALSE;
		}
		break;
	}
	return consistency;
}

void Lattice::read_lattice(std::string fname)
{
	/**
	* Read lattice configuration from a text file. Note that the number of lines has to correspond with the
	* number of lattice sites. The file must have four columns which are separated by empty spaces. First 
	* column is ignored and can contain any value for e.g. enumeration.
	*
	* @param[in] fname The name of the file with the lattice configuration.
	*/

	_numberAtoms = Functions::get_num_lines(fname);
	delete[] _latticeCoordArray;
	_latticeCoordArray = new Threedim[_numberAtoms];
	Threedim* _latticeCoordArrayPntr = _latticeCoordArray;
	
	std::ifstream infile(fname);
	std::string line;
	std::string item;

	if (!infile)
	{
		std::cout << "The file " << fname << " does not exist!" << std::endl;
		exit(0);
	}
	while (std::getline(infile, line))
	{
		std::istringstream isstream(line);
		item.clear();
		isstream >> item;
		item.clear();
		isstream >> item;

		_latticeCoordArrayPntr->x = std::stof(item);
		item.clear();
		isstream >> item;
		_latticeCoordArrayPntr->y = std::stof(item);
		item.clear();
		isstream >> item;
		_latticeCoordArrayPntr->z = std::stof(item);
		++_latticeCoordArrayPntr;
	}
	infile.close(); 
}

void Lattice::create_lattice(void) 
{
	/**
	* Create lattice configuration according to specified parameters.
	*/

	switch (_latticeType) 
	{
	case simpleCubic: 
		create_lattice_sc(_latticeDimensions[0], _latticeDimensions[1], _latticeDimensions[2]);
		create_miller_plane();
		break;
	case bodyCenteredCubic: 
		create_lattice_bcc(_latticeDimensions[0], _latticeDimensions[1], _latticeDimensions[2]);
		create_miller_plane();
		break;
	case faceCenteredCubic: 
		create_lattice_fcc(_latticeDimensions[0], _latticeDimensions[1], _latticeDimensions[2]);
		create_miller_plane();
		break;

	case triangularHexagonal:
		if (_boundaryConditions == helical) {
			create_triangular_hexagonal_helical(_latticeDimensions[0]);
		}
		else {
			create_triangular_hexagonal_open(_latticeDimensions[0]);
		}
		break;
	case triangularTriangular:
		create_triangular_triangular(_latticeDimensions[0]);
		break;
	case triangularHalfDisk:
		create_triangular_half_disk(_latticeDimensions[0]);
		break;
	case triangularDisk:
		create_triangular_disk(_latticeDimensions[0]);
		break;
	case triangularArrowHead:
		create_triangular_arrow_head(_latticeDimensions[0], _latticeDimensions[1]);
		break;
	case triangularStripe:
		create_triangular_stripe(_latticeDimensions[0], _latticeDimensions[1]);
		break;
	}
}

void Lattice::create_mask_read_in_shape(LatticeMaskParameters latticeMaskParams)
{
	const cimg_library::CImg<float> img(latticeMaskParams.fname.c_str());

	std::vector<Threedim> latticeSites;
	switch (latticeMaskParams.latticeType)
	{
	case squareLattice:
		latticeSites = get_sites_square_lattice((int)latticeMaskParams.height + 1, 
			(int)latticeMaskParams.width + 1);
		break;
	case hexagonalLattice:
		latticeSites = get_sites_hexagonal_lattice((int)latticeMaskParams.height + 1,
			(int)latticeMaskParams.width + 1);
		break;
	default:
		latticeSites = get_sites_square_lattice((int)latticeMaskParams.height + 1,
			(int)latticeMaskParams.width + 1);
	}

	double heightRatioPixelToLatticeParam = img.height()/latticeMaskParams.height;
	double widthRatioPixelToLatticeParam = img.width() / latticeMaskParams.width;
	std::vector<Threedim> tempLatticeSites;

	for (auto it = latticeSites.begin(); it != latticeSites.end(); ++it)
	{
		int pixelX = widthRatioPixelToLatticeParam*(*it).x;
		int pixelY = img.height() - heightRatioPixelToLatticeParam*(*it).y;
		float defaultValue = -1;
		float value = img.atXY(pixelX, pixelY, 0, 0, defaultValue);
		if (value > 100)
		{
			tempLatticeSites.push_back(*it);
		}
	}
	delete[] _latticeCoordArray;
	_numberAtoms = tempLatticeSites.size();
	_latticeCoordArray = new Threedim[_numberAtoms];
	for (int i = 0; i < tempLatticeSites.size(); ++i)
	{
		_latticeCoordArray[i] = tempLatticeSites[i];
	}
	neighbor_distances();
	assign_neighbors();
}

void Lattice::create_lattice_sc(int x, int y, int z)
{
	/**
	* This method produces a simple cubic lattice. 
	*
	* param[in] x Number of atoms along x direction.
	* param[in] y Number of atoms along y direction.
	* param[in] z Number of atoms along z direction.
	*/

	Threedim a1 = { 1, 0, 0 };
	Threedim a2 = { 0, 1, 0 };
	Threedim a3 = { 0, 0, 1 };

	_numberAtoms = x * y * z; //number of atoms
	delete[] _latticeCoordArray;
	_latticeCoordArray = new Threedim[_numberAtoms];

	Threedim vec1 = { 0, 0, 0 };
	Threedim vec2 = { 0, 0, 0 };
	Threedim vec3 = { 0, 0, 0 };
	int count = 0;
	for (int count_x = 0; count_x < x; ++count_x)
	{
		for (int count_y = 0; count_y < y; ++count_y)
		{
			for (int count_z = 0; count_z < z; ++count_z)
			{
				count = count_x * z * y + count_y * z + count_z;
				vec1 = MyMath::mult(a1, count_x);
				vec2 = MyMath::mult(a2, count_y);
				vec3 = MyMath::mult(a3, count_z);
				_latticeCoordArray[count] = MyMath::add(MyMath::add(vec1, vec2), vec3);
			}
		}
	}

	if (z < 2)
	{
		_wignerSeitzCell.clear();
		_wignerSeitzCell.push_back(Threedim{ 0.5, 0.5, 0});
		_wignerSeitzCell.push_back(Threedim{ -0.5, 0.5, 0 });
		_wignerSeitzCell.push_back(Threedim{ -0.5, -0.5, 0 });
		_wignerSeitzCell.push_back(Threedim{ 0.5, -0.5, 0 });

		_dummyskNTriangles.clear();
		Threedim average1{ 1/3., 1/3.,0 };
		_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0,0,0 }, average1));
		_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0,1,0 }, average1));
		_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 1,0,0 }, average1));

		Threedim average2 = { 2/3., 2/3.,0 };
		_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 1,0,0 }, average2));
		_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0,1,0 }, average2));
		_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 1,1,0 }, average2));
		delete[] _skNcells;
		_skNcellNum = 2 * (x-1)*(y-1);
		_skNcells = new TopologicalChargeCell[_skNcellNum]; // cells for evaluation of skyrmion number
		
		int count = 0;
		for (int i = 0; i < x-1; ++i)
		{
			for (int j = 1; j < y; ++j)
			{
				int index = i*y + j;

				_skNcells[count].i = index-1;
				_skNcells[count].j = index;
				_skNcells[count].k = index+y-1;
				_skNcells[count].triangleIndex = 0;
				_skNcells[count].position = MyMath::add(_latticeCoordArray[index-1], average1);
				++count;

				_skNcells[count].i = index;
				_skNcells[count].j = index+y;
				_skNcells[count].k = index+y-1;
				_skNcells[count].triangleIndex = 1;
				_skNcells[count].position = MyMath::add(_latticeCoordArray[index-1], average2);
				++count;
			}
		}
	}

	if (_boundaryConditions == periodic)
	{
		delete[] _firstNeighborArray;
		delete[] _firstNeighborVectorArray;
		int fnbors = 6;
		_numberNthNeighbors[0] = fnbors;
		_firstNeighborArray = new int[_numberAtoms*fnbors];
		_firstNeighborVectorArray = new Threedim[_numberAtoms*fnbors];
		
		for (int i = 0; i < _numberAtoms; ++i)
		{
			int value = (i / z)*z + (i + 1) % z;
			_firstNeighborArray[i*fnbors] = (i / z)*z + (i + 1) % z;
			_firstNeighborVectorArray[i*fnbors] = a3;

			_firstNeighborArray[i*fnbors+1] = ((((i/z)*z+z-1)-(i-1))/z)*z + (i - 1);
			_firstNeighborVectorArray[i*fnbors+1] = MyMath::mult(a3,-1);

			_firstNeighborArray[i*fnbors+2] = (i / (y*z))*y*z + (i + z) % (y*z);
			_firstNeighborVectorArray[i*fnbors+2] = a2;

			_firstNeighborArray[i*fnbors + 3] = (((i/(y*z)*y*z+y*z-1)-(i-z))/(y*z))*y*z + (i - z);
			_firstNeighborVectorArray[i*fnbors + 3] = MyMath::mult(a2, -1);

			_firstNeighborArray[i*fnbors+4] = (i / (x*y*z))*x*y*z + (i + y*z) % (x*y*z);
			_firstNeighborVectorArray[i*fnbors+4] = a1;

			_firstNeighborArray[i*fnbors + 5] = (((i/(x*y*z)*x*y*z + x*y*z - 1) - (i - y*z)) / (x*y*z))*(x*y*z) + (i - y*z);
			_firstNeighborVectorArray[i*fnbors + 5] = MyMath::mult(a1, -1);
		}
		for (int i = 0; i < _numberAtoms; ++i)
		{
			for (int j = 0; j < fnbors; ++j)
			{
				int neighbor = _firstNeighborArray[i*fnbors + j];
				if (i == _firstNeighborArray[i*fnbors + j])
				{
					_firstNeighborArray[i*fnbors + j] = -1;
					_firstNeighborVectorArray[i*fnbors + j] = Threedim{ 0,0,0 };
				}
			}
		}
	}
	else
	{
		neighbor_distances();
		assign_neighbors();
	}
}

void Lattice::create_lattice_bcc(int x, int y, int z)
{
	/**
	* This method produces a body centered cubic lattice. 
	*
	* param[in] x Number of atoms along x direction.
	* param[in] y Number of atoms along y direction.
	* param[in] z Number of atoms along z direction.
	*/

	Threedim a1 = { 1, 0, 0 };
	Threedim a2 = { 0, 1, 0 };
	Threedim a3 = { 0, 0, 1 };

	_numberAtoms = x * y * z * 2; // number of lattice sites
	delete[] _latticeCoordArray;
	_latticeCoordArray = new Threedim[_numberAtoms];

	Threedim vec1 = { 0, 0, 0 };
	Threedim vec2 = { 0, 0, 0 };
	Threedim vec3 = { 0, 0, 0 };
	int count = 0;
	for (int count_x = 0; count_x < x; ++count_x)
	{
		for (int count_y = 0; count_y < y; ++count_y)
		{
			for (int count_z = 0; count_z < z; ++count_z)
			{
				count = count_x * z * 2 * y + count_y * z * 2 + count_z * 2;
				vec1 = MyMath::mult(a1, count_x);
				vec2 = MyMath::mult(a2, count_y);
				vec3 = MyMath::mult(a3, count_z);
				_latticeCoordArray[count] = MyMath::add(MyMath::add(vec1, vec2), vec3);
				_latticeCoordArray[count + 1] = MyMath::add(_latticeCoordArray[count], { 0.5, 0.5, 0.5 });
			}
		}
	}

	neighbor_distances();
	assign_neighbors();
}

void Lattice::create_lattice_fcc(int x, int y, int z)
{

	/**
	* This method produces a face centered cubic lattice. 
	*
	* param[in] x Number of atoms along x direction.
	* param[in] y Number of atoms along y direction.
	* param[in] z Number of atoms along z direction.
	*/

	Threedim a1 = { 1, 0, 0 };
	Threedim a2 = { 0, 1, 0 };
	Threedim a3 = { 0, 0, 1 };

	_numberAtoms = x * y * z * 4; // number of lattice sites
	delete[] _latticeCoordArray;
	_latticeCoordArray = new Threedim[_numberAtoms];

	Threedim vec1 = { 0, 0, 0 };
	Threedim vec2 = { 0, 0, 0 };
	Threedim vec3 = { 0, 0, 0 };
	int count = 0;
	for (int count_x = 0; count_x < x; ++count_x)
	{
		for (int count_y = 0; count_y < y; ++count_y)
		{
			for (int count_z = 0; count_z < z; ++count_z)
			{
				count = count_x * z * 4 * y + count_y * z * 4 + count_z * 4;
				vec1 = MyMath::mult(a1, count_x);
				vec2 = MyMath::mult(a2, count_y);
				vec3 = MyMath::mult(a3, count_z);
				_latticeCoordArray[count] = MyMath::add(MyMath::add(vec1, vec2), vec3);
				_latticeCoordArray[count + 1] = MyMath::add(_latticeCoordArray[count], { 0.5, 0.5, 0 });
				_latticeCoordArray[count + 2] = MyMath::add(_latticeCoordArray[count], { 0.5, 0, 0.5 });
				_latticeCoordArray[count + 3] = MyMath::add(_latticeCoordArray[count], { 0, 0.5, 0.5 });
			}
		}
	}
	neighbor_distances();
	assign_neighbors();
}

void Lattice::create_miller_plane(void)
{

	/**
	* This method evaluates the plane that refers to the given set of Miller indices (h,k,l). The coordinates 
	* of the points in the plane are stored into _latticeCoordArray. If the variables were not defined and are
	* therefore all equal to zero the method will do nothing.
	*/
	
	int miller_idx_h = _millerIndexes[0];
	int miller_idx_k = _millerIndexes[1];
	int miller_idx_l = _millerIndexes[2];

	if (!_latticeCoordArray)
	{
		std::cout << "_latticeCoordArray does not exist." << std::endl;
		exit(0);
	}

	switch (miller_idx_h + miller_idx_k + miller_idx_l)
	{

	case 0:
		break; //If h=k=l=0, don't do anything.

	default:
		find_center_site();
		Threedim * tmp_latticeCoordArray; //temporal array for lattice plane
		tmp_latticeCoordArray = new Threedim[_numberAtoms];
		Threedim millerVec = { (double) miller_idx_h, (double) miller_idx_k, (double) miller_idx_l };
		int count = 0; // number of points in the plane.
		for (int i = 0; i < _numberAtoms; i++)
		{
			double p_mem = FALSE; //Check if i-th point in _latticeCoordArray is member of the plane.
			p_mem = MyMath::dot_product(
				MyMath::difference(_latticeCoordArray[i], _latticeCoordArray[_centerSite]),
				millerVec);

			if (abs(p_mem) < PRECISION)
			{
				tmp_latticeCoordArray[count].x = _latticeCoordArray[i].x;
				tmp_latticeCoordArray[count].y = _latticeCoordArray[i].y;
				tmp_latticeCoordArray[count].z = _latticeCoordArray[i].z;
				count = count + 1;
			}
		}
		delete[] _latticeCoordArray;
		_latticeCoordArray = NULL;
		_latticeCoordArray = new Threedim[count];
		_numberAtoms = count;
		for (int i = 0; i < count; i++)
		{
			_latticeCoordArray[i].x = tmp_latticeCoordArray[i].x;
			_latticeCoordArray[i].y = tmp_latticeCoordArray[i].y;
			_latticeCoordArray[i].z = tmp_latticeCoordArray[i].z;
		}

		delete[] tmp_latticeCoordArray;
		tmp_latticeCoordArray = NULL;
		break;
	}
}

void Lattice::create_triangular_hexagonal_open(int n)
{
	/**
	* This method creates a triangular lattice with a hexagonal shape. The method sets nearest neighbors with 
	* open boundary conditions.
	*
	* @param[in] n Number of sites at one straight edge of the hexagonal shaped system.
	*/

	create_triangular_hexagonal_helical(n);
	neighbor_distances();
	assign_neighbors();

	Threedim vec1 = MyMath::difference(_dummyskNTriangles[0], _dummyskNTriangles[1]);
	Threedim vec2 = MyMath::difference(_dummyskNTriangles[0], _dummyskNTriangles[2]);
	Threedim vec3 = MyMath::difference(_dummyskNTriangles[1], _dummyskNTriangles[2]);
	double perimeter0 = MyMath::norm(vec1) + MyMath::norm(vec2) + MyMath::norm(vec3);

	int* transcriptArray = new int[_skNcellNum];
	int count = 0;
	for (int i = 0; i < _skNcellNum; i++)
	{
		vec1 = MyMath::difference(_latticeCoordArray[_skNcells[i].i], _latticeCoordArray[_skNcells[i].j]);
		vec2 = MyMath::difference(_latticeCoordArray[_skNcells[i].i], _latticeCoordArray[_skNcells[i].k]);
		vec3 = MyMath::difference(_latticeCoordArray[_skNcells[i].j], _latticeCoordArray[_skNcells[i].k]);
		double perimeter = MyMath::norm(vec1) + MyMath::norm(vec2) + MyMath::norm(vec3);

		if (fabs(perimeter0 - perimeter) < PRECISION)
		{
			transcriptArray[i] = TRUE;
			++count;
		}
		else
		{
			transcriptArray[i] = FALSE;
		}
	}
	TopologicalChargeCell* skNCells = new TopologicalChargeCell[count];
	int count2 = 0;
	for (int i = 0; i < _skNcellNum; i++)
	{
		if (transcriptArray[i])
		{
			skNCells[count2] = _skNcells[i];
			++count2;
		}
	}
	delete[] transcriptArray;
	delete[] _skNcells;
	_skNcells = skNCells; // cells for evaluation of skyrmion number
	_skNcellNum = count;
}


void Lattice::create_triangular_hexagonal_helical(int n)
{
	/**
	* This function creates a triangular lattice with a hexagonal shape. The method sets !!!only!!! next 
	* nearest neighbors with helical boundary conditions. All other interactions will be set as with open
	* boundary conditions. (second nearest neighbors, third nearest neighbors ... and 4-spin cells)
	*
	* @param[in] n Number of sites at one straight edge of hexagonal shaped system
	*
	*/

	//lattice vectors
	Threedim a1 = { 1, 0, 0 };
	double y = -sqrt(3) / 2;
	Threedim a2 = { -0.5, y, 0 };
	Threedim a3 = { 0.5, y, 0 };
	//total number of lattice sites; expression is equivalent with 3*(n-0.5)*(n-0.5)+0.25
	_numberAtoms = ((2 * n - 2) * (2 * n - 1) / 2 - (n - 1) * n / 2) * 2 + 2 * n - 1;
	delete[] _latticeCoordArray;
	_latticeCoordArray = new Threedim[_numberAtoms];

	//lattice sites of topmost line. (Barkema Monte Carlo Methods on helical boundaries on triangular lattice)
	int top = 3 * n - 2;

	Threedim vec1 = { 0, 0, 0 };
	Threedim vec2 = { 0, 0, 0 };

	for (int i = 0; i < n; ++i) //sum over rows
	{
		for (int j = 0; j < top; ++j) //sum over columns
		{
			if (i < n - 1) //last rows needs to be treated specially
			{
				vec1 = MyMath::mult(a1, j);
				vec2 = MyMath::mult(a2, i);
				_latticeCoordArray[i * (top)+j] = MyMath::add(vec1, vec2);
			}
			else //treat last row
			{
				if (j < 2 * n - 1)
				{
					vec1 = MyMath::mult(a1, j);
					vec2 = MyMath::mult(a2, i);
					_latticeCoordArray[i * (top)+j] = MyMath::add(vec1, vec2);
				}
			}
		}
	}

	Threedim vec3 = { 0, 0, 0 };
	Threedim vec4 = { 0, 0, 0 };

	//translate right part of lattice sites in order to bring the triangular lattice into a boundary hexagonal shape
	for (int i = 0; i < n - 1; ++i)
	{
		for (int j = 0; j < top; ++j)
		{
			if (j > n + i - 1)
			{
				vec1 = MyMath::mult(a1, -n + 1);
				vec2 = MyMath::mult(a2, n);
				vec3 = MyMath::add(vec1, vec2);
				vec4 = _latticeCoordArray[i * (top)+j];
				_latticeCoordArray[i * (top)+j] = MyMath::add(vec3, vec4);
			}
		}
	}
	//neighbor_distances();
	//assign_neighbors();

	_wignerSeitzCell.clear();
	double value = 1. / (2 * sqrt(3.));
	_wignerSeitzCell.push_back(Threedim{ 0, 1./sqrt(3), 0 });
	_wignerSeitzCell.push_back(Threedim{ -0.5, value, 0 });
	_wignerSeitzCell.push_back(Threedim{ -0.5, -value, 0 });
	_wignerSeitzCell.push_back(Threedim{ 0, -1/sqrt(3), 0 });
	_wignerSeitzCell.push_back(Threedim{ 0.5, -value, 0 });
	_wignerSeitzCell.push_back(Threedim{ 0.5, value, 0 });

	int numNeigh = 6; //number of nearest neighbors
	_numberNthNeighbors[0] = numNeigh; //set number of first neighbors
	delete _firstNeighborArray;
	_firstNeighborArray = new int[_numberAtoms * numNeigh];
	delete _firstNeighborVectorArray;
	_firstNeighborVectorArray = new Threedim[_numberAtoms * numNeigh];

	_dummyskNTriangles.clear();
	Threedim average1{ 0.5, sqrt(3) / 6.,0 };
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0,0,0 }, average1));
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 1,0,0 }, average1));
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0.5,sqrt(3) / 2.,0 }, average1));

	Threedim average2 = { 0,sqrt(3) / 3.,0 };
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0,0,0 }, average2));
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0.5,sqrt(3) / 2. ,0 }, average2));
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ -0.5,sqrt(3) / 2.,0 }, average2));
	delete[] _skNcells;
	_skNcells = new TopologicalChargeCell[_numberAtoms * 2]; // cells for evaluation of skyrmion number
	_skNcellNum = _numberAtoms * 2;

	for (int i = 0; i < _numberAtoms; ++i)
	{
		_skNcells[2 * i].i = i;
		_skNcells[2 * i].j = i; // initialization
		_skNcells[2 * i].k = i; // initialization
		_skNcells[2 * i].triangleIndex = 0;
		_skNcells[2 * i].position = MyMath::add(_latticeCoordArray[i],average1);
		_skNcells[2 * i + 1].i = i;
		_skNcells[2 * i + 1].j = i;
		_skNcells[2 * i + 1].k = i;
		_skNcells[2 * i + 1].triangleIndex = 1;
		_skNcells[2 * i + 1].position = MyMath::add(_latticeCoordArray[i], average2);
	}
	

	//set left and right neighbor
	for (int i = 1; i < _numberAtoms - 1; ++i)
	{
		_firstNeighborArray[i * numNeigh] = i + 1; //right neighbor
		_skNcells[2 * i].j = i + 1;
		_firstNeighborArray[i * numNeigh + 1] = i - 1; //left neighbor
		_firstNeighborVectorArray[i * numNeigh] = a1;
		_firstNeighborVectorArray[i * numNeigh + 1] = MyMath::mult(a1, -1);
	}
	// set left and right neighbor for first and last site
	_firstNeighborArray[0] = 1; //right neighbor
	_skNcells[0].j = 1;
	_firstNeighborArray[1] = _numberAtoms - 1; //left neighbor
	_firstNeighborArray[(_numberAtoms - 1) * numNeigh] = 0; //right neighbor
	_firstNeighborArray[(_numberAtoms - 1) * numNeigh + 1] = _numberAtoms - 2; //left neighbor
	_skNcells[2 * (_numberAtoms - 1)].j = 0;
	_firstNeighborVectorArray[0] = a1;
	_firstNeighborVectorArray[1] = MyMath::mult(a1, -1);
	_firstNeighborVectorArray[(_numberAtoms - 1) * numNeigh] = a1;
	_firstNeighborVectorArray[(_numberAtoms - 1) * numNeigh + 1] = MyMath::mult(a1, -1);
	int neigh;

	//set other 4 neighbors
	for (int i = 0; i < _numberAtoms; ++i)
	{
		neigh = i + 3 * n - 2; // bottom left
		if (neigh < _numberAtoms)
		{
			_firstNeighborArray[i * numNeigh + 2] = neigh;
			_firstNeighborVectorArray[i * numNeigh + 2] = a2;
		}
		else
		{
			neigh = neigh - _numberAtoms;
			_firstNeighborArray[i * numNeigh + 2] = neigh;
			_firstNeighborVectorArray[i * numNeigh + 2] = a2;
		}
		neigh = i - 3 * n + 2; // top right
		if (neigh > -1)
		{
			_firstNeighborArray[i * numNeigh + 3] = neigh;
			_skNcells[2 * i].k = neigh;
			_skNcells[2 * i + 1].j = neigh;
			_firstNeighborVectorArray[i * numNeigh + 3] = MyMath::mult(a2, -1);
		}
		else
		{
			neigh = neigh + _numberAtoms;
			_firstNeighborArray[i * numNeigh + 3] = neigh;
			_skNcells[2 * i].k = neigh;
			_skNcells[2 * i + 1].j = neigh;
			_firstNeighborVectorArray[i * numNeigh + 3] = MyMath::mult(a2, -1);
		}
		neigh = i + 3 * n - 1; // bottom right
		if (neigh < _numberAtoms)
		{
			_firstNeighborArray[i * numNeigh + 4] = neigh;
			_firstNeighborVectorArray[i * numNeigh + 4] = a3;
		}
		else
		{
			neigh = neigh - _numberAtoms;
			_firstNeighborArray[i * numNeigh + 4] = neigh;
			_firstNeighborVectorArray[i * numNeigh + 4] = a3;
		}
		neigh = i - 3 * n + 1; // top left
		if (neigh > -1)
		{
			_firstNeighborArray[i * numNeigh + 5] = neigh;
			_skNcells[2 * i + 1].k = neigh;
			_firstNeighborVectorArray[i * numNeigh + 5] = MyMath::mult(a3, -1);
		}
		else
		{
			neigh = neigh + _numberAtoms;
			_firstNeighborArray[i * numNeigh + 5] = neigh;
			_skNcells[2 * i + 1].k = neigh;
			_firstNeighborVectorArray[i * numNeigh + 5] = MyMath::mult(a3, -1);
		}
	}

	_fourSpinCellsPerAtom = 12;
	delete[] _fourSpinCells;
	_fourSpinCells = new Fourdim[_numberAtoms * _fourSpinCellsPerAtom]; // cells for evaluation of four spin interaction

	for (int i = 0; i < _numberAtoms; ++i)
	{
		_fourSpinCells[i * _fourSpinCellsPerAtom].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom].j = _firstNeighborArray[i * numNeigh + 4]; // bottom right
		_fourSpinCells[i * _fourSpinCellsPerAtom].k = _firstNeighborArray[i * numNeigh]; // right
		_fourSpinCells[i * _fourSpinCellsPerAtom].l = _firstNeighborArray[i * numNeigh + 3]; // top right

		_fourSpinCells[i * _fourSpinCellsPerAtom + 1].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 1].j = _firstNeighborArray[i * numNeigh + 5]; // top left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 1].k = _firstNeighborArray[i * numNeigh + 1]; // left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 1].l = _firstNeighborArray[i * numNeigh + 2]; // bottom left

		_fourSpinCells[i * _fourSpinCellsPerAtom + 2].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 2].j = _firstNeighborArray[i * numNeigh]; // right
		_fourSpinCells[i * _fourSpinCellsPerAtom + 2].k = _firstNeighborArray[i * numNeigh + 3]; // top right
		_fourSpinCells[i * _fourSpinCellsPerAtom + 2].l = _firstNeighborArray[i * numNeigh + 5]; // top left

		_fourSpinCells[i * _fourSpinCellsPerAtom + 3].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 3].j = _firstNeighborArray[i * numNeigh + 1]; // left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 3].k = _firstNeighborArray[i * numNeigh + 2]; // bottom left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 3].l = _firstNeighborArray[i * numNeigh + 4]; // bottom right

		_fourSpinCells[i * _fourSpinCellsPerAtom + 4].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 4].j = _firstNeighborArray[i * numNeigh + 3]; // top right
		_fourSpinCells[i * _fourSpinCellsPerAtom + 4].k = _firstNeighborArray[i * numNeigh + 5]; // top left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 4].l = _firstNeighborArray[i * numNeigh + 1]; // left

		_fourSpinCells[i * _fourSpinCellsPerAtom + 5].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 5].j = _firstNeighborArray[i * numNeigh + 2]; // bottom left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 5].k = _firstNeighborArray[i * numNeigh + 4]; // bottom right
		_fourSpinCells[i * _fourSpinCellsPerAtom + 5].l = _firstNeighborArray[i * numNeigh]; //right

		_fourSpinCells[i * _fourSpinCellsPerAtom + 6].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 6].j = _firstNeighborArray[i * numNeigh + 3]; // top right
		_fourSpinCells[i * _fourSpinCellsPerAtom + 6].k = _firstNeighborArray[_firstNeighborArray[i
			* numNeigh + 3] * numNeigh + 5]; // NN top
		_fourSpinCells[i * _fourSpinCellsPerAtom + 6].l = _firstNeighborArray[i * numNeigh + 5]; // top left

		_fourSpinCells[i * _fourSpinCellsPerAtom + 7].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 7].j = _firstNeighborArray[i * numNeigh]; // right
		_fourSpinCells[i * _fourSpinCellsPerAtom + 7].k = _firstNeighborArray[_firstNeighborArray[i
			* numNeigh] * numNeigh + 3]; //NN top right
		_fourSpinCells[i * _fourSpinCellsPerAtom + 7].l = _firstNeighborArray[i * numNeigh + 3]; // top right

		_fourSpinCells[i * _fourSpinCellsPerAtom + 8].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 8].j = _firstNeighborArray[i * numNeigh + 4]; // bottom right
		_fourSpinCells[i * _fourSpinCellsPerAtom + 8].k = _firstNeighborArray[_firstNeighborArray[i
			* numNeigh + 4] * numNeigh]; // NN bottom right
		_fourSpinCells[i * _fourSpinCellsPerAtom + 8].l = _firstNeighborArray[i * numNeigh]; // right

		_fourSpinCells[i * _fourSpinCellsPerAtom + 9].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 9].j = _firstNeighborArray[i * numNeigh + 2]; // bottom left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 9].k = _firstNeighborArray[_firstNeighborArray[i
			* numNeigh + 2] * numNeigh + 4]; // NN bottom
		_fourSpinCells[i * _fourSpinCellsPerAtom + 9].l = _firstNeighborArray[i * numNeigh + 4]; // bottom right

		_fourSpinCells[i * _fourSpinCellsPerAtom + 10].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 10].j = _firstNeighborArray[i * numNeigh + 1]; // left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 10].k = _firstNeighborArray[_firstNeighborArray[i
			* numNeigh + 1] * numNeigh + 2]; // NN bottom left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 10].l = _firstNeighborArray[i * numNeigh + 2]; //bottom left

		_fourSpinCells[i * _fourSpinCellsPerAtom + 11].i = i;
		_fourSpinCells[i * _fourSpinCellsPerAtom + 11].j = _firstNeighborArray[i * numNeigh + 5]; // top left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 11].k = _firstNeighborArray[_firstNeighborArray[i
			* numNeigh + 5] * numNeigh + 1]; // NN top left
		_fourSpinCells[i * _fourSpinCellsPerAtom + 11].l = _firstNeighborArray[i * numNeigh + 1]; // left
	}
	double tmpvalue = 0;
	int site1 = 0;
	int site2 = 0;
	int site3 = 0;
	int site4 = 0;
	for (int i = 0; i < _numberAtoms * _fourSpinCellsPerAtom; ++i)
	{
		site1 = _fourSpinCells[i].i;
		site2 = _fourSpinCells[i].j;
		site3 = _fourSpinCells[i].k;
		site4 = _fourSpinCells[i].l;
		tmpvalue = MyMath::norm(MyMath::difference(_latticeCoordArray[site1], _latticeCoordArray[site2]));
		tmpvalue += MyMath::norm(MyMath::difference(_latticeCoordArray[site2], _latticeCoordArray[site3]));
		tmpvalue += MyMath::norm(MyMath::difference(_latticeCoordArray[site3], _latticeCoordArray[site4]));
		if (tmpvalue > 3.1)
		{
			_fourSpinCells[i].i = -1;
			_fourSpinCells[i].j = -1;
			_fourSpinCells[i].k = -1;
			_fourSpinCells[i].l = -1;
		}
	}
	
	_threeSiteCellsPerAtom = 6;
	delete[] _threeSiteCells;
	_threeSiteCells = new ThreeSite[_numberAtoms * _threeSiteCellsPerAtom]; // cells for evaluation of three-site interaction
	for (int i = 0; i < _numberAtoms; ++i)
	{
		_threeSiteCells[i * _threeSiteCellsPerAtom].i = i;
		_threeSiteCells[i * _threeSiteCellsPerAtom].j = _firstNeighborArray[i * numNeigh + 4]; // bottom right
		_threeSiteCells[i * _threeSiteCellsPerAtom].k = _firstNeighborArray[i * numNeigh]; // right

		_threeSiteCells[i * _threeSiteCellsPerAtom + 1].i = i;
		_threeSiteCells[i * _threeSiteCellsPerAtom + 1].j = _firstNeighborArray[i * numNeigh + 5]; // top left
		_threeSiteCells[i * _threeSiteCellsPerAtom + 1].k = _firstNeighborArray[i * numNeigh + 1]; // left

		_threeSiteCells[i * _threeSiteCellsPerAtom + 2].i = i;
		_threeSiteCells[i * _threeSiteCellsPerAtom + 2].j = _firstNeighborArray[i * numNeigh]; // right
		_threeSiteCells[i * _threeSiteCellsPerAtom + 2].k = _firstNeighborArray[i * numNeigh + 3]; // top right

		_threeSiteCells[i * _threeSiteCellsPerAtom + 3].i = i;
		_threeSiteCells[i * _threeSiteCellsPerAtom + 3].j = _firstNeighborArray[i * numNeigh + 1]; // left
		_threeSiteCells[i * _threeSiteCellsPerAtom + 3].k = _firstNeighborArray[i * numNeigh + 2]; // bottom left

		_threeSiteCells[i * _threeSiteCellsPerAtom + 4].i = i;
		_threeSiteCells[i * _threeSiteCellsPerAtom + 4].j = _firstNeighborArray[i * numNeigh + 3]; // top right
		_threeSiteCells[i * _threeSiteCellsPerAtom + 4].k = _firstNeighborArray[i * numNeigh + 5]; // top left

		_threeSiteCells[i * _threeSiteCellsPerAtom + 5].i = i;
		_threeSiteCells[i * _threeSiteCellsPerAtom + 5].j = _firstNeighborArray[i * numNeigh + 2]; // bottom left
		_threeSiteCells[i * _threeSiteCellsPerAtom + 5].k = _firstNeighborArray[i * numNeigh + 4]; // bottom right
	}
	for (int i = 0; i < _numberAtoms * _threeSiteCellsPerAtom; ++i)
	{
		site1 = _threeSiteCells[i].i;
		site2 = _threeSiteCells[i].j;
		site3 = _threeSiteCells[i].k;
		tmpvalue = MyMath::norm(MyMath::difference(_latticeCoordArray[site1], _latticeCoordArray[site2]));
		tmpvalue += MyMath::norm(MyMath::difference(_latticeCoordArray[site2], _latticeCoordArray[site3]));
		tmpvalue += MyMath::norm(MyMath::difference(_latticeCoordArray[site3], _latticeCoordArray[site1]));
		if (tmpvalue > 3.1)
		{
			_threeSiteCells[i].i = -1;
			_threeSiteCells[i].j = -1;
			_threeSiteCells[i].k = -1;
		}
	}
}

void Lattice::finish_cutting_process(int* belongsToCutSystem, int numberCutAtoms)
{
	Threedim* tmp_latticeCoordArray = new Threedim[numberCutAtoms];

	// transcript array keeps track to which array position each selected lattice site will be written into 
	// the new _latticeCoordArray; default value -1 means that it is not part of the new lattice.
	int* transcriptArray = new int[_numberAtoms];
	int count = 0;
	for (int i = 0; i < _numberAtoms; ++i)
	{
		transcriptArray[i] = -1;
		if (belongsToCutSystem[i] == TRUE)
		{
			tmp_latticeCoordArray[count].x = _latticeCoordArray[i].x;
			tmp_latticeCoordArray[count].y = _latticeCoordArray[i].y;
			tmp_latticeCoordArray[count].z = _latticeCoordArray[i].z;
			transcriptArray[i] = count;
			count += 1;
		}
	}

	int skNcellNumTemp = 0;
	for (int i = 0; i < _skNcellNum; i++)
	{
		if (belongsToCutSystem[_skNcells[i].i] && belongsToCutSystem[_skNcells[i].j]
			&& belongsToCutSystem[_skNcells[i].k])
		{
			skNcellNumTemp += 1;
		}
	}
	TopologicalChargeCell* _skNcellsTemp = new TopologicalChargeCell[skNcellNumTemp];
	count = 0;
	for (int i = 0; i < _skNcellNum; i++)
	{
		if (belongsToCutSystem[_skNcells[i].i] && belongsToCutSystem[_skNcells[i].j]
			&& belongsToCutSystem[_skNcells[i].k])
		{
			_skNcellsTemp[count] = _skNcells[i];
			_skNcellsTemp[count].i = transcriptArray[_skNcells[i].i];
			_skNcellsTemp[count].j = transcriptArray[_skNcells[i].j];
			_skNcellsTemp[count].k = transcriptArray[_skNcells[i].k];
			count += 1;
		}
	}

	Fourdim* tmp_fourSpinCells = new Fourdim[numberCutAtoms * _fourSpinCellsPerAtom];
	for (int i = 0; i < numberCutAtoms * _fourSpinCellsPerAtom; ++i)
	{
		tmp_fourSpinCells[i].i = -1;
		tmp_fourSpinCells[i].j = -1;
		tmp_fourSpinCells[i].k = -1;
		tmp_fourSpinCells[i].l = -1;
	}
	int position = 0;
	for (int i = 0; i < _numberAtoms; ++i)
	{
		if (transcriptArray[i] != -1)
		{
			position = transcriptArray[i] * _fourSpinCellsPerAtom;
			for (int j = 0; j < _fourSpinCellsPerAtom; ++j)
			{
				tmp_fourSpinCells[position + j].i = _fourSpinCells[i * _fourSpinCellsPerAtom + j].i;
				tmp_fourSpinCells[position + j].j = _fourSpinCells[i * _fourSpinCellsPerAtom + j].j;
				tmp_fourSpinCells[position + j].k = _fourSpinCells[i * _fourSpinCellsPerAtom + j].k;
				tmp_fourSpinCells[position + j].l = _fourSpinCells[i * _fourSpinCellsPerAtom + j].l;
			}
		}
	}
	int tmpBool = TRUE;
	for (int i = 0; i < numberCutAtoms * _fourSpinCellsPerAtom; ++i)
	{
		tmpBool = TRUE;
		if (tmp_fourSpinCells[i].i != -1)
		{
			tmp_fourSpinCells[i].i = transcriptArray[tmp_fourSpinCells[i].i];
			tmp_fourSpinCells[i].j = transcriptArray[tmp_fourSpinCells[i].j];
			tmp_fourSpinCells[i].k = transcriptArray[tmp_fourSpinCells[i].k];
			tmp_fourSpinCells[i].l = transcriptArray[tmp_fourSpinCells[i].l];
		}
		if (tmp_fourSpinCells[i].i < -0.5)
		{
			tmpBool = FALSE;
		}
		if (tmp_fourSpinCells[i].j < -0.5)
		{
			tmpBool = FALSE;
		}
		if (tmp_fourSpinCells[i].k < -0.5)
		{
			tmpBool = FALSE;
		}
		if (tmp_fourSpinCells[i].l < -0.5)
		{
			tmpBool = FALSE;
		}
		if (tmpBool == FALSE)
		{
			tmp_fourSpinCells[i].i = -1;
			tmp_fourSpinCells[i].j = -1;
			tmp_fourSpinCells[i].k = -1;
			tmp_fourSpinCells[i].l = -1;
		}
	}

	ThreeSite * tmp_ThreeSiteCells = new ThreeSite[numberCutAtoms * _threeSiteCellsPerAtom];
	for (int i = 0; i < numberCutAtoms * _threeSiteCellsPerAtom; ++i)
	{
		tmp_ThreeSiteCells[i].i = -1;
		tmp_ThreeSiteCells[i].j = -1;
		tmp_ThreeSiteCells[i].k = -1;
	}
	for (int i = 0; i < _numberAtoms; ++i)
	{
		if (transcriptArray[i] != -1)
		{
			position = transcriptArray[i] * _threeSiteCellsPerAtom;
			for (int j = 0; j < _threeSiteCellsPerAtom; ++j)
			{
				tmp_ThreeSiteCells[position + j].i = _threeSiteCells[i * _threeSiteCellsPerAtom + j].i;
				tmp_ThreeSiteCells[position + j].j = _threeSiteCells[i * _threeSiteCellsPerAtom + j].j;
				tmp_ThreeSiteCells[position + j].k = _threeSiteCells[i * _threeSiteCellsPerAtom + j].k;
			}
		}
	}
	for (int i = 0; i < numberCutAtoms * _threeSiteCellsPerAtom; ++i)
	{
		tmpBool = TRUE;
		if (tmp_ThreeSiteCells[i].i != -1)
		{
			tmp_ThreeSiteCells[i].i = transcriptArray[tmp_ThreeSiteCells[i].i];
			tmp_ThreeSiteCells[i].j = transcriptArray[tmp_ThreeSiteCells[i].j];
			tmp_ThreeSiteCells[i].k = transcriptArray[tmp_ThreeSiteCells[i].k];
		}
		if (tmp_ThreeSiteCells[i].i < -0.5)
		{
			tmpBool = FALSE;
		}
		if (tmp_ThreeSiteCells[i].j < -0.5)
		{
			tmpBool = FALSE;
		}
		if (tmp_ThreeSiteCells[i].k < -0.5)
		{
			tmpBool = FALSE;
		}
		if (tmpBool == FALSE)
		{
			tmp_ThreeSiteCells[i].i = -1;
			tmp_ThreeSiteCells[i].j = -1;
			tmp_ThreeSiteCells[i].k = -1;
		}
	}

	delete[] _latticeCoordArray;
	_latticeCoordArray = tmp_latticeCoordArray;
	_numberAtoms = numberCutAtoms;
	delete[] _skNcells;
	_skNcells = _skNcellsTemp;
	_skNcellNum = skNcellNumTemp;
	delete[] _fourSpinCells;
	_fourSpinCells = tmp_fourSpinCells;
	delete[] _threeSiteCells;
	_threeSiteCells = tmp_ThreeSiteCells;
	delete[] transcriptArray;

	neighbor_distances();
	assign_neighbors();
}

std::vector<Threedim> Lattice::get_sites_square_lattice(const int &m, const int &n)
{
	Threedim a1 = { 1, 0, 0 };
	Threedim a2 = { 0, 1, 0 };

	Threedim vec1 = { 0, 0, 0 };
	Threedim vec2 = { 0, 0, 0 };

	std::vector<Threedim> latticeSites;
	for (int count_x = 0; count_x < n; ++count_x)
	{
		for (int count_y = 0; count_y < m; ++count_y)
		{
			vec1 = MyMath::mult(a1, count_x);
			vec2 = MyMath::mult(a2, count_y);
			latticeSites.push_back(MyMath::add(vec1, vec2));
		}
	}
	return latticeSites;
}

std::vector<Threedim> Lattice::get_sites_hexagonal_lattice(const int &m, const int &n)
{
	Threedim a1 = { 1, 0, 0 };
	double y = sqrt(3) / 2;
	Threedim a2 = { -0.5, y, 0 };
	Threedim a3 = { 0.5, y, 0 };
	
	std::vector<Threedim> latticeSites;
	Threedim vec1 = { 0, 0, 0 };

	for (int i = 0; i < n; ++i)
	{
		vec1 = MyMath::mult(a1, i);
		latticeSites.push_back(vec1);
	}

	Threedim vec2 = { 0, 0, 0 };
	Threedim vec3 = { 0, 0, 0 };
	Threedim vec4 = { 0, 0, 0 };
	for (int i = 1; i < m; ++i)
	{
		if (i % 2 > PRECISION)
		{
			vec2 = a2;
		}
		else
		{
			vec2 = a3;
		}
		vec3 = vec4;
		vec4 = MyMath::add(vec3, vec2);
		for (int j = 0; j < n; ++j)
		{
			vec1 = MyMath::mult(a1, j);
			latticeSites.push_back(MyMath::add(vec1, vec4));
		}
	}
	return latticeSites;
}

void Lattice::create_triangular_triangular(int n)
{
	/**
	* Creates a two-dimensional triangular lattice with a triangular boundary shape.
	*
	*  @param[in] n Number of lattice sites at one edge of the triangular shaped system.
	*/

	// the triangle is cut from a system with a hexagonal boundary shape
	create_triangular_hexagonal_helical(n);
	int* belongsToTriangle = new int[_numberAtoms]; 
	for (int i = 0; i < _numberAtoms; ++i) // set default value false
	{
		belongsToTriangle[i] = FALSE;
	}
	double line = 0;
	int count = 0;
	for (int i = 0; i < _numberAtoms; ++i) // check whether lattice site is in triangle
	{
		if (_latticeCoordArray[i].y > -(n - 0.5) * sqrt(3.0) / 2.0)
		{
			line = -_latticeCoordArray[i].y * 2.0 / sqrt(3.0);
			if (_latticeCoordArray[i].x < 0.5 + line * 0.5)
			{
				belongsToTriangle[i] = TRUE;
				count += 1;
			}
		}
	}
	int tmpnuat = n * (n + 1) / 2;
	// consistency check whether the number of selected lattice sites corresponds with the expected one
	if (tmpnuat != count) 
	{
		std::cout << "error with creation of triangular lattice." << std::endl;
	}	

	finish_cutting_process(belongsToTriangle, tmpnuat);

	delete[] belongsToTriangle;
}

void Lattice::create_triangular_half_disk(int r)
{
	int n = 2 * r + 1;
	Threedim center = { (double) r, 0., 0. };
	create_triangular_hexagonal_helical(n);
	int* belongsToDisk = new int[_numberAtoms];
	for (int i = 0; i < _numberAtoms; ++i)
	{
		belongsToDisk[i] = FALSE;
	}
	int count = 0;
	double tmpvalue = 0;
	for (int i = 0; i < _numberAtoms; ++i)
	{
		tmpvalue = MyMath::norm(MyMath::difference(center, _latticeCoordArray[i]));
		if (tmpvalue < r)
		{
			belongsToDisk[i] = TRUE;
			count += 1;
		}
	}
	int tmpnuat = count;

	finish_cutting_process(belongsToDisk, tmpnuat);

	delete[] belongsToDisk;
}

void Lattice::create_triangular_disk(int r)
{
	int n = 3 * r;
	Threedim center = { ((n - 1) / 2.), -(n - 1) * sqrt(3) / 2, 0. };
	create_triangular_hexagonal_helical(n);
	int* belongsToDisk = new int[_numberAtoms];
	for (int i = 0; i < _numberAtoms; ++i)
	{
		belongsToDisk[i] = FALSE;
	}
	int count = 0;
	double tmpvalue = 0;
	for (int i = 0; i < _numberAtoms; ++i)
	{
		tmpvalue = MyMath::norm(MyMath::difference(center, _latticeCoordArray[i]));
		if (tmpvalue < r)
		{
			belongsToDisk[i] = TRUE;
			count += 1;
		}
	}
	int tmpnuat = count;

	finish_cutting_process(belongsToDisk, tmpnuat);

	delete[] belongsToDisk;
}

void Lattice::create_triangular_arrow_head(int m, int n)
{
	create_triangular_hexagonal_open(m);
	int* belongsToArrow = new int[_numberAtoms];
	for (int i = 0; i < _numberAtoms; ++i)
	{
		belongsToArrow[i] = TRUE;
	}
	
	double a = sqrt(3.);
	double b = 0;
	double a1 = sqrt(3.) / 3.;

	Threedim point = { 0, sqrt(3.)/2, 0 };
	b = point.y - a*point.x;

	for (int i = 0; i < _numberAtoms; ++i)
	{
		if (_latticeCoordArray[i].y > a*_latticeCoordArray[i].x + b)
		{
			belongsToArrow[i] = FALSE;
		}

		if (_latticeCoordArray[i].y > -a*_latticeCoordArray[i].x + b)
		{
			belongsToArrow[i] = FALSE;
		}
	}

	double x0 = (m - 1) / 2. + 1 / 8.;
	point = { -x0, -a*x0, 0 };
	b = point.y + a1*point.x;

	for (int i = 0; i < _numberAtoms; ++i)
	{
		if (_latticeCoordArray[i].y < -a1*_latticeCoordArray[i].x + b)
		{
			belongsToArrow[i] = FALSE;
		}
	}

	point = { x0, -a*x0, 0 };
	b = point.y - a1*point.x;

	for (int i = 0; i < _numberAtoms; ++i)
	{
		if (_latticeCoordArray[i].y < a1*_latticeCoordArray[i].x + b)
		{
			belongsToArrow[i] = FALSE;
		}
	}

	point = { 0, sqrt(3.) / 2 - n*sqrt(3.), 0 };
	b = point.y - a*point.x;
	double b1 = point.y + a*point.x;

	for (int i = 0; i < _numberAtoms; ++i)
	{
		if (_latticeCoordArray[i].y < a*_latticeCoordArray[i].x + b)
		{
			if (_latticeCoordArray[i].y < -a*_latticeCoordArray[i].x + b1)
			{
				belongsToArrow[i] = FALSE;
			}
		}
	}

	int tmpnuat = 0;
	for (int i = 0; i < _numberAtoms; ++i)
	{
		if (belongsToArrow[i] == TRUE)
		{
			tmpnuat += 1;
		}
	}

	finish_cutting_process(belongsToArrow, tmpnuat);

	delete[] belongsToArrow;
}

void Lattice::create_triangular_stripe(int m, int n)
{
	/*
	* This function creates a triangular lattice with a rectangular shape.
	* The functions sets nearest neighbors with open boundary conditions.
	*
	* @param[in] m number of atomic rows
	* @param[in] n number of close packed atoms along x-direction
	*/

	int boolPeriodicX = FALSE;
	int boolPeriodicY = FALSE;

	switch (_boundaryConditions) {
	case periodic:
		boolPeriodicX = TRUE;
		boolPeriodicY = TRUE;
		break;
	case periodicX:
		boolPeriodicX = TRUE;
		break;
	case periodicY:
		boolPeriodicY = TRUE;
		break;

	}

	if (m % 2 != 0)
	{
		boolPeriodicY = FALSE;
	}

	//lattice vectors
	Threedim a1 = { 1, 0, 0 };
	double y = -sqrt(3) / 2;
	Threedim a2 = { -0.5, y, 0 };
	Threedim a3 = { 0.5, y, 0 };
	_numberAtoms = n * m;
	delete[] _latticeCoordArray;
	_latticeCoordArray = new Threedim[_numberAtoms];

	Threedim vec1 = { 0, 0, 0 };

	for (int i = 0; i < n; ++i)
	{
		vec1 = MyMath::mult(a1, i);
		_latticeCoordArray[i] = vec1;
	}

	Threedim vec2 = { 0, 0, 0 };
	Threedim vec3 = { 0, 0, 0 };
	Threedim vec4 = { 0, 0, 0 };
	for (int i = 1; i < m; ++i)
	{
		if (i % 2 > PRECISION)
		{
			vec2 = a2;
		}
		else
		{
			vec2 = a3;
		}
		vec3 = vec4;
		vec4 = MyMath::add(vec3, vec2);
		for (int j = 0; j < n; ++j)
		{
			vec1 = MyMath::mult(a1, j);
			_latticeCoordArray[i * n + j] = MyMath::add(vec1, vec4);
		}
	}

	neighbor_distances();
	assign_neighbors();

	_wignerSeitzCell.clear();
	double value = 1./(2*sqrt(3.));
	_wignerSeitzCell.push_back(Threedim{ 0, 1/sqrt(3), 0 });
	_wignerSeitzCell.push_back(Threedim{ -0.5, value, 0 });
	_wignerSeitzCell.push_back(Threedim{ -0.5, -value, 0 });
	_wignerSeitzCell.push_back(Threedim{ 0, -1/sqrt(3), 0 });
	_wignerSeitzCell.push_back(Threedim{ 0.5, -value, 0 });
	_wignerSeitzCell.push_back(Threedim{ 0.5, value, 0 });

	_dummyskNTriangles.clear();
	Threedim average1{ 0.5, -sqrt(3) / 6.,0 };
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0,0,0 },average1));
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 1,0,0 },average1));
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0.5,-sqrt(3) / 2.,0 },average1));

	Threedim average2 = {0,sqrt(3)/3.,0};
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0,0,0 },average2));
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ 0.5,-sqrt(3) / 2. ,0},average2));
	_dummyskNTriangles.push_back(MyMath::difference(Threedim{ -0.5,-sqrt(3) / 2.,0 },average2));
	_skNcellNum =  m * n * 2;
	delete[] _skNcells;
	_skNcells = new TopologicalChargeCell[_skNcellNum]; // cells for evaluation of skyrmion number
	int count = 0;
	int countskN = 0;
	int even = TRUE;
	for (int i = 0; i < _numberAtoms - n; ++i)
	{
		if (count >(n - 1))
		{
			if (even == TRUE)
			{
				even = FALSE;
			}
			else
			{
				even = TRUE;
			}
			count = 0;
		}
		if (even == TRUE)
		{
			if (count < n - 1)
			{
				_skNcells[countskN].i = i;
				_skNcells[countskN].j = i + 1;
				_skNcells[countskN].k = i + n + 1;
				_skNcells[countskN].triangleIndex = 0;
				_skNcells[countskN].position = MyMath::add(_latticeCoordArray[i], average1);
				countskN += 1;
				_skNcells[countskN].i = i;
				_skNcells[countskN].j = i + n + 1;
				_skNcells[countskN].k = i + n;
				_skNcells[countskN].triangleIndex = 1;
				_skNcells[countskN].position = MyMath::add(_latticeCoordArray[i], average2);
				countskN += 1;
			}
			if (count == n - 1 && boolPeriodicX == 1)
			{
				_skNcells[countskN].i = i;
				_skNcells[countskN].j = i - n + 1;
				_skNcells[countskN].k = i + 1;
				_skNcells[countskN].triangleIndex = 0;
				_skNcells[countskN].position = MyMath::add(_latticeCoordArray[i], average1);
				countskN += 1;
				_skNcells[countskN].i = i;
				_skNcells[countskN].j = i + 1;
				_skNcells[countskN].k = i + n;
				_skNcells[countskN].triangleIndex = 1;
				_skNcells[countskN].position = MyMath::add(_latticeCoordArray[i], average2);
				countskN += 1;
			}
			count += 1;
		}
		else
		{
			if (count < 1)
			{
				_skNcells[countskN].i = i;
				_skNcells[countskN].j = i + 1;
				_skNcells[countskN].k = i + n;
				_skNcells[countskN].triangleIndex = 0;
				_skNcells[countskN].position = MyMath::add(_latticeCoordArray[i], average1);
				countskN += 1;
				if (boolPeriodicX == 1)
				{
					_skNcells[countskN].i = i;
					_skNcells[countskN].j = i + n;
					_skNcells[countskN].k = i + 2 * n - 1;
					_skNcells[countskN].triangleIndex = 1;
					_skNcells[countskN].position = MyMath::add(_latticeCoordArray[i], average2);
					countskN += 1;
				}
			}
			else
			{
				if (count < n - 1)
				{
					_skNcells[countskN].i = i;
					_skNcells[countskN].j = i + 1;
					_skNcells[countskN].k = i + n;
					_skNcells[countskN].triangleIndex = 0;
					_skNcells[countskN].position = MyMath::add(_latticeCoordArray[i], average1);
					countskN += 1;
					_skNcells[countskN].i = i;
					_skNcells[countskN].j = i + n;
					_skNcells[countskN].k = i + n - 1;
					_skNcells[countskN].triangleIndex = 1;
					_skNcells[countskN].position = MyMath::add(_latticeCoordArray[i], average2);
					countskN += 1;
				}
				else
				{
					_skNcells[countskN].i = i;
					_skNcells[countskN].j = i + n;
					_skNcells[countskN].k = i + n - 1;
					_skNcells[countskN].triangleIndex = 1;
					_skNcells[countskN].position = MyMath::add(_latticeCoordArray[i], average2);
					countskN += 1;
					if (boolPeriodicX == 1)
					{
						_skNcells[countskN].i = i;
						_skNcells[countskN].j = i - n + 1;
						_skNcells[countskN].k = i + n;
						_skNcells[countskN].triangleIndex = 0;
						_skNcells[countskN].position = MyMath::add(_latticeCoordArray[i], average1);
						countskN += 1;
					}
				}
			}
			count += 1;
		}
	}
	if (boolPeriodicY == 1)
	{
		for (int i = 0; i < n - 1; ++i)
		{
			_skNcells[countskN].i = (m - 1) * n + i;
			_skNcells[countskN].j = (m - 1) * n + i + 1;
			_skNcells[countskN].k = i;
			_skNcells[countskN].triangleIndex = 0;
			_skNcells[countskN].position = MyMath::add(_latticeCoordArray[(m - 1) * n + i], average1);
			countskN += 1;
		}
		_skNcells[countskN].i = m * n - 1;
		_skNcells[countskN].j = (m - 1) * n;
		_skNcells[countskN].k = n - 1;
		_skNcells[countskN].triangleIndex = 0;
		_skNcells[countskN].position = MyMath::add(_latticeCoordArray[m * n - 1], average1);
		countskN += 1;

		for (int i = 1; i < n ; ++i)
		{
			_skNcells[countskN].i = (m - 1) * n + i;
			_skNcells[countskN].j = i;
			_skNcells[countskN].k = i - 1;
			_skNcells[countskN].triangleIndex = 1;
			_skNcells[countskN].position = MyMath::add(_latticeCoordArray[(m - 1) * n + i], average2);
			countskN += 1;
		}
		_skNcells[countskN].i = (m - 1) * n;
		_skNcells[countskN].j = 0;
		_skNcells[countskN].k = n - 1;
		_skNcells[countskN].triangleIndex = 1;
		_skNcells[countskN].position = MyMath::add(_latticeCoordArray[(m - 1) * n], average2);
		countskN += 1;
	}
	_skNcellNum = countskN;

	int ind1 = 0;
	int ind2 = 0;

	if (boolPeriodicX == 1)
	{
		// first lattice sites of first and last row
		ind2 = n - 1; //last lattice site of first row
		_firstNeighborArray[3] = ind2;
		_firstNeighborVectorArray[3] = MyMath::mult(a1, -1);

		ind1 = (m - 1) * n; // first lattice site of last row
		ind2 = m * n - 1; //last lattice site
		_firstNeighborArray[6 * ind1 + 2] = ind2;
		_firstNeighborVectorArray[6 * ind1 + 2] = MyMath::mult(a1, -1);
		_firstNeighborArray[6 * ind1 + 3] = ind1 - 1;
		_firstNeighborVectorArray[6 * ind1 + 3] = MyMath::mult(a3, -1);

		// last lattice sites of first and last row
		ind1 = n - 1; //last site of first row
		_firstNeighborArray[6 * ind1 + 2] = 0;
		_firstNeighborVectorArray[6 * ind1 + 2] = a1;
		_firstNeighborArray[6 * ind1 + 3] = n;
		_firstNeighborVectorArray[6 * ind1 + 3] = a3;

		ind1 = (m * n) - 1; //last lattice site
		ind2 = (m - 1) * n; //first lattice site of last row
		_firstNeighborArray[6 * ind1 + 3] = ind2;
		_firstNeighborVectorArray[6 * ind1 + 3] = a1;

		//the first and last atom of the first and last row are neglected
		for (int i = 1; i < m - 1; i++)
		{
			ind1 = i * n; // first site of a row
			ind2 = (i + 1) * n - 1; // last site of the same row
			if (i % 2 == 1)
			{
				// first site of row
				_firstNeighborArray[6 * ind1 + 3] = ind2;
				_firstNeighborVectorArray[6 * ind1 + 3] = MyMath::mult(a1, -1);
				_firstNeighborArray[6 * ind1 + 4] = ind1 - 1;
				_firstNeighborVectorArray[6 * ind1 + 4] = MyMath::mult(a3, -1);
				_firstNeighborArray[6 * ind1 + 5] = ind2 + n;
				_firstNeighborVectorArray[6 * ind1 + 5] = a2;
				//last site of row
				_firstNeighborArray[6 * ind2 + 5] = ind1;
				_firstNeighborVectorArray[6 * ind2 + 5] = a1;
			}
			else
			{
				// first site of row
				_firstNeighborArray[6 * ind1 + 5] = ind2;
				_firstNeighborVectorArray[6 * ind1 + 5] = MyMath::mult(a1, -1);
				//last site of row
				_firstNeighborArray[6 * ind2 + 3] = ind1;
				_firstNeighborVectorArray[6 * ind2 + 3] = a1;
				_firstNeighborArray[6 * ind2 + 4] = ind2 + 1;
				_firstNeighborVectorArray[6 * ind2 + 4] = a3;
				_firstNeighborArray[6 * ind2 + 5] = ind1 - n;
				_firstNeighborVectorArray[6 * ind2 + 5] = MyMath::mult(a2, -1);
			}
		}
	}

	if (boolPeriodicY == 1)
	{
		if (m % 2 == 0)
		{
			if (boolPeriodicX == 1)
			{
				// first lattice sites of first and last row
				ind2 = (m - 1) * n; //first lattice site of last row
				_firstNeighborArray[4] = ind2;
				_firstNeighborVectorArray[4] = MyMath::mult(a3, -1);
				_firstNeighborArray[5] = ind2 + 1;
				_firstNeighborVectorArray[5] = MyMath::mult(a2, -1);

				_firstNeighborArray[6 * ind2 + 4] = 0;
				_firstNeighborVectorArray[6 * ind2 + 4] = a3;
				_firstNeighborArray[6 * ind2 + 5] = n - 1;
				_firstNeighborVectorArray[6 * ind2 + 5] = a2;

				// last lattice site of first and last row
				ind1 = n - 1; //last site of first row
				ind2 = m * n - 1; //last lattice site of last row
				_firstNeighborArray[6 * ind1 + 4] = ind2;
				_firstNeighborVectorArray[6 * ind1 + 4] = MyMath::mult(a3, -1);
				_firstNeighborArray[6 * ind1 + 5] = (m - 1) * n;
				_firstNeighborVectorArray[6 * ind1 + 5] = MyMath::mult(a2, -1);

				_firstNeighborArray[6 * ind2 + 4] = ind1;
				_firstNeighborVectorArray[6 * ind2 + 4] = a3;
				_firstNeighborArray[6 * ind2 + 5] = ind1 - 1;
				_firstNeighborVectorArray[6 * ind2 + 5] = a2;
			}
			else
			{
				// first lattice site of first and last row
				ind2 = (m - 1) * n; //first lattice site of last row
				_firstNeighborArray[3] = ind2;
				_firstNeighborVectorArray[3] = MyMath::mult(a3, -1);
				_firstNeighborArray[4] = ind2 + 1;
				_firstNeighborVectorArray[4] = MyMath::mult(a2, -1);

				_firstNeighborArray[6 * ind2 + 2] = 0;
				_firstNeighborVectorArray[6 * ind2 + 2] = a3;
				_firstNeighborArray[6 * ind2 + 3] = n - 1;
				_firstNeighborVectorArray[6 * ind2 + 3] = a2;

				// last lattice site of first and last row
				ind1 = n - 1; //last site of first row
				ind2 = m * n - 1; //last lattice site of last row
				_firstNeighborArray[6 * ind1 + 2] = ind2;
				_firstNeighborVectorArray[6 * ind1 + 2] = MyMath::mult(a3, -1);
				_firstNeighborArray[6 * ind1 + 3] = (m - 1) * n;
				_firstNeighborVectorArray[6 * ind1 + 3] = MyMath::mult(a2, -1);

				_firstNeighborArray[6 * ind2 + 3] = ind1;
				_firstNeighborVectorArray[6 * ind2 + 3] = a3;
				_firstNeighborArray[6 * ind2 + 4] = ind1 - 1;
				_firstNeighborVectorArray[6 * ind2 + 4] = a2;
			}

			//the first and last atom of the first and last row are excluded
			for (int i = 1; i < n - 1; i++)
			{
				//first row
				_firstNeighborArray[6 * i + 4] = m * n - n + i;
				_firstNeighborVectorArray[6 * i + 4] = MyMath::mult(a3, -1);
				_firstNeighborArray[6 * i + 5] = m * n - n + i + 1;
				_firstNeighborVectorArray[6 * i + 5] = MyMath::mult(a2, -1);
				//last row
				_firstNeighborArray[6 * (m * n - n + i) + 4] = i - 1;
				_firstNeighborVectorArray[6 * (m * n - n + i) + 4] = a2;
				_firstNeighborArray[6 * (m * n - n + i) + 5] = i;
				_firstNeighborVectorArray[6 * (m * n - n + i) + 5] = a3;
			}
		}
		else
		{
			std::cout
				<< "Caution. You required periodic boundaries which is not possible for uneven numbers of atomic rows."
				<< std::endl;
		}

	}
}

void Lattice::find_center_site(void)
{
	Threedim min = _latticeCoordArray[0];
	Threedim max = _latticeCoordArray[0];
	Threedim center;
	for (int i = 0; i < _numberAtoms; ++i)
	{
		if (min.x > _latticeCoordArray[i].x)
		{
			min.x = _latticeCoordArray[i].x;
		}
		if (min.y > _latticeCoordArray[i].y)
		{
			min.y = _latticeCoordArray[i].y;
		}
		if (min.z > _latticeCoordArray[i].z)
		{
			min.z = _latticeCoordArray[i].z;
		}
		if (max.x < _latticeCoordArray[i].x)
		{
			max.x = _latticeCoordArray[i].x;
		}
		if (max.y < _latticeCoordArray[i].y)
		{
			max.y = _latticeCoordArray[i].y;
		}
		if (max.z < _latticeCoordArray[i].z)
		{
			max.z = _latticeCoordArray[i].z;
		}
	}
	center.x = (max.x + min.x) / 2;
	center.y = (max.y + min.y) / 2;
	center.z = (max.z + min.z) / 2;
	double distance = 0;
	double tmpDistance = 0;
	_centerSite = 0;
	distance = MyMath::norm(MyMath::difference(_latticeCoordArray[_centerSite], center));
	for (int i = 1; i < _numberAtoms; ++i)
	{
		tmpDistance = MyMath::norm(MyMath::difference(_latticeCoordArray[i], center));
		if (tmpDistance < distance)
		{
			distance = tmpDistance;
			_centerSite = i;
		}
	}
}

void Lattice::neighbor_distances(void)
{
	/*
	*  This method reads an given array with a lattice configuration and determines
	* the distances between atom no.0 and all other atoms. That gives all possible
	* distances in the lattice. After that, all different distances are stored into
	* _distanceArray array. The values are stored beginning with the shortest distance.
	* To bring the distances into order, the bubble sort algorithm is used. The number
	* of different distances is returned.
	*/
	int count1, count2, count, count_S2, length = 0;
	int arraymiller_idx_length = _numberAtoms; //TODO: ist das korrekt für alle Gitter? Entspricht dies der maximalen Anzahl verschiedener Abstände?
	double VecNorm;
	Threedim CompareLatticePos, *LatticePntr_S2;
	double* SortArray = new double[arraymiller_idx_length];
	int* SortNeighArray = new int[arraymiller_idx_length];
	double SortValue;
	int SortNeighValue;
	if (!_latticeCoordArray)
	{
		std::cout << "_latticeCoordArray does not exist.Call Createmiller_idx_lattice()!" << std::endl;
		exit(0);
	}
	find_center_site();
	for (count = 0; count < arraymiller_idx_length; ++count)
	{ //Array Eintraege werden auf -1 gesetzt!
		SortArray[count] = -1;
		SortNeighArray[count] = -1;
	}

	CompareLatticePos = _latticeCoordArray[_centerSite];
	LatticePntr_S2 = _latticeCoordArray;
	for (count_S2 = 0; count_S2 < _numberAtoms; ++count_S2, ++LatticePntr_S2)
	{
		if (count_S2 != _centerSite)
		{
			VecNorm = MyMath::norm(MyMath::difference(CompareLatticePos, *LatticePntr_S2));
			for (count = 0; count < arraymiller_idx_length; ++count)
			{
				if (fabs(SortArray[count] - VecNorm) < PRECISION)
				{
					break;
				}
				else if (SortArray[count] == -1)
				{
					SortArray[count] = VecNorm;
					SortNeighArray[count] = count_S2;
					++length;
					break;
				}
			}
		}
	}

	/*Bubble Sort Algorithm*/
	for (count1 = 0; count1 < arraymiller_idx_length; ++count1)
	{
		for (count2 = 0; count2 < arraymiller_idx_length - count1 - 1; ++count2)
			if (SortArray[count2 + 1] < SortArray[count2])
			{
				SortValue = SortArray[count2];
				SortArray[count2] = SortArray[count2 + 1];
				SortArray[count2 + 1] = SortValue;
				SortNeighValue = SortNeighArray[count2];
				SortNeighArray[count2] = SortNeighArray[count2 + 1];
				SortNeighArray[count2 + 1] = SortNeighValue;
			}
	}
	count2 = 0;
	for (count1 = 0; count1 < arraymiller_idx_length; ++count1)
	{
		if (SortArray[count1] > PRECISION)
		{
			count2++;
		}
	}
	delete[] _distanceArray;
	_distanceArray = new double[count2];
	delete[] _distanceNeigh;
	_distanceNeigh = new int[count2];
	count2 = 0;
	for (count1 = 0; count1 < arraymiller_idx_length; ++count1)
	{
		if (SortArray[count1] > PRECISION)
		{
			_distanceArray[count2] = SortArray[count1];
			_distanceNeigh[count2] = SortNeighArray[count1];
			++count2;
		}
	}
	int NumberDistance = count2;
	_radiusMax = NumberDistance;

	delete[] SortArray;
	delete[] SortNeighArray;
}

void Lattice::assign_neighbors(void)
{
	if (_radiusMax == 0)
	{
		std::cout
			<< "The matrix with the nearest distances does not exist or neighGrade is lager than the number of calculated neighbor distances in the distance matrix."
			<< std::endl;
	}
	else
	{
		int* NeighborArrayPntr = NULL;
		int maxNeigh = 0;
		if (_radiusMax > 8)
		{
			maxNeigh = 8;
		}
		else
		{
			maxNeigh = _radiusMax;
		}
		for (int k = 0; k < maxNeigh; ++k)
		{
			double precision = 0.01;
			double norm = 0;
			int numNeigh = 0;
			int count = 0;
			int insert = 0;
			double neighDistance = _distanceArray[k];
			Threedim* _latticeCoordArrayPntr1 = _latticeCoordArray;
			Threedim* _latticeCoordArrayPntr2 = _latticeCoordArray;
			for (int i = 0; i < _numberAtoms; ++i, ++_latticeCoordArrayPntr1)
			{ // find number of kth neighbors
				_latticeCoordArrayPntr2 = _latticeCoordArray;
				for (int j = 0; j < _numberAtoms; ++j, ++_latticeCoordArrayPntr2)
				{
					norm = MyMath::norm(MyMath::difference(*_latticeCoordArrayPntr2,*_latticeCoordArrayPntr1));
					if (fabs(norm - neighDistance) < precision)
					{
						++count;
					}
				}
				if (count > numNeigh)
				{
					numNeigh = count;
				}
				count = 0;
			}
			switch (k)
			{
			case 0:
				delete[] _firstNeighborArray;
				_firstNeighborArray = new int[_numberAtoms * numNeigh];
				NeighborArrayPntr = _firstNeighborArray;
				delete[] _firstNeighborVectorArray;
				_firstNeighborVectorArray = new Threedim[_numberAtoms * numNeigh];
				_numberNthNeighbors[0] = numNeigh;
				break;
			case 1:
				delete[] _secondNeighborArray;
				_secondNeighborArray = new int[_numberAtoms * numNeigh];
				NeighborArrayPntr = _secondNeighborArray;
				delete[] _secondNeighborVectorArray;
				_secondNeighborVectorArray = new Threedim[_numberAtoms * numNeigh];
				_numberNthNeighbors[1] = numNeigh;
				break;
			case 2:
				delete[] _thirdNeighborArray;
				_thirdNeighborArray = new int[_numberAtoms * numNeigh];
				NeighborArrayPntr = _thirdNeighborArray;
				delete[] _thirdNeighborVectorArray;
				_thirdNeighborVectorArray = new Threedim[_numberAtoms * numNeigh];
				_numberNthNeighbors[2] = numNeigh;
				break;
			case 3:
				delete[] _fourthNeighborArray;
				_fourthNeighborArray = new int[_numberAtoms * numNeigh];
				NeighborArrayPntr = _fourthNeighborArray;
				delete[] _fourthNeighborVectorArray;
				_fourthNeighborVectorArray = new Threedim[_numberAtoms * numNeigh];
				_numberNthNeighbors[3] = numNeigh;
				break;
			case 4:
				delete[] _fifthNeighborArray;
				_fifthNeighborArray = new int[_numberAtoms * numNeigh];
				NeighborArrayPntr = _fifthNeighborArray;
				delete[] _fifthNeighborVectorArray;
				_fifthNeighborVectorArray = new Threedim[_numberAtoms * numNeigh];
				_numberNthNeighbors[4] = numNeigh;
				break;
			case 5:
				delete[] _sixthNeighborArray;
				_sixthNeighborArray = new int[_numberAtoms * numNeigh];
				NeighborArrayPntr = _sixthNeighborArray;
				_numberNthNeighbors[5] = numNeigh;
				break;
			case 6:
				delete[] _seventhNeighborArray;
				_seventhNeighborArray = new int[_numberAtoms * numNeigh];
				NeighborArrayPntr = _seventhNeighborArray;
				_numberNthNeighbors[6] = numNeigh;
				break;
			case 7:
				delete[] _eighthNeighborArray;
				_eighthNeighborArray = new int[_numberAtoms * numNeigh];
				NeighborArrayPntr = _eighthNeighborArray;
				_numberNthNeighbors[7] = numNeigh;
				break;
			}
			if (NeighborArrayPntr != NULL) 
			{
				for (int i = 0; i < _numberAtoms * numNeigh; ++i)
				{ // default value is -1 for no neighbor
					NeighborArrayPntr[i] = -1;
				}
			_latticeCoordArrayPntr1 = _latticeCoordArray;
			for (int i = 0; i < _numberAtoms; ++i, ++_latticeCoordArrayPntr1)
			{
				_latticeCoordArrayPntr2 = _latticeCoordArray + 1 + i;
				for (int j = i + 1; j < _numberAtoms; ++j, ++_latticeCoordArrayPntr2)
				{
					norm = MyMath::norm(MyMath::difference(*_latticeCoordArrayPntr2,*_latticeCoordArrayPntr1)); // distance between site i,j
					if (fabs(norm - neighDistance) < precision)
					{
						insert = 0;
						count = 0;
						while (insert == 0)
						{
							if (NeighborArrayPntr[i * numNeigh + count] == -1)
							{
								NeighborArrayPntr[i * numNeigh + count] = j;
								if (k == 0)
								{
									_firstNeighborVectorArray[i * numNeigh + count] =
										MyMath::difference(_latticeCoordArray[j],
											_latticeCoordArray[i]);
								}
								else if (k == 1)
								{
									_secondNeighborVectorArray[i * numNeigh + count] =
										MyMath::difference(_latticeCoordArray[j],
											_latticeCoordArray[i]);
								}
								else if (k == 2)
								{
									_thirdNeighborVectorArray[i * numNeigh + count] =
										MyMath::difference(_latticeCoordArray[j],
											_latticeCoordArray[i]);
								}
								else if (k == 3)
								{
									_fourthNeighborVectorArray[i * numNeigh + count] =
										MyMath::difference(_latticeCoordArray[j],
											_latticeCoordArray[i]);
								}
								else if (k == 4)
								{
									_fifthNeighborVectorArray[i * numNeigh + count] =
										MyMath::difference(_latticeCoordArray[j],
											_latticeCoordArray[i]);
								}
								insert = 1;
							}
							else
							{
								++count;
							}
						}

						insert = 0;
						count = 0;
						while (insert == 0)
						{
							if (NeighborArrayPntr[j * numNeigh + count] == -1)
							{
								NeighborArrayPntr[j * numNeigh + count] = i;
								if (k == 0)
								{
									_firstNeighborVectorArray[j * numNeigh + count] =
										MyMath::difference(_latticeCoordArray[i],
											_latticeCoordArray[j]);
								}
								else if (k == 1)
								{
									_secondNeighborVectorArray[j * numNeigh + count] =
										MyMath::difference(_latticeCoordArray[i],
											_latticeCoordArray[j]);
								}
								else if (k == 2)
								{
									_thirdNeighborVectorArray[j * numNeigh + count] =
										MyMath::difference(_latticeCoordArray[i],
											_latticeCoordArray[j]);
								}
								else if (k == 3)
								{
									_fourthNeighborVectorArray[j * numNeigh + count] =
										MyMath::difference(_latticeCoordArray[i],
											_latticeCoordArray[j]);
								}
								else if (k == 4)
								{
									_fifthNeighborVectorArray[j * numNeigh + count] =
										MyMath::difference(_latticeCoordArray[i],
											_latticeCoordArray[j]);
								}
								insert = 1;
							}
							else
							{
								++count;
							}
						}
					}
				}
			}
			}
		}
	}
}

int Lattice::is_neighbor(const int &pos1, const int &pos2) const
{
	/*
	* This function checks whether two lattice sites are (nearest) neighbors.
	*
	* @param[in] _pos1 index of first lattice site
	* @param[in] _pos2 index of second lattice site
	*
	* @return 0 if not neighbors; 1 if neighbors
	*
	*/
	int isneigh = FALSE;
	// iterate over all neighbors of lattice site with index _pos1
	for (int i = 0; i < _numberNthNeighbors[0]; i++)
	{
		if (pos2 == _firstNeighborArray[_numberNthNeighbors[0] * pos1 + i])
		{
			isneigh = TRUE;
		}
	}
	return isneigh;
}

void Lattice::show_lattice_coordinates(void) const
{
	/*
	* Prints out the Lattice Coordinates of the created or given Lattice.
	*/
	int i;
	if (_latticeCoordArray == NULL)
	{
		std::cout << "_latticeCoordArray does not exist! Run assign_neighbors()!" << std::endl;
		exit(0);
	}
	Threedim *LatticeCoordPntr;
	LatticeCoordPntr = _latticeCoordArray;
	for (i = 0; i < _numberAtoms; ++i, ++LatticeCoordPntr)
	{
		std::cout << i << " " << LatticeCoordPntr->x << " " << LatticeCoordPntr->y << " "
			<< LatticeCoordPntr->z << std::endl;
	}
}

void Lattice::show_neighbor_distances(void) const
{
	if (!_distanceArray)
	{
		std::cout << "DistancesArray does not exist. Call NthNeighborDistances!" << std::endl;
		exit(0);
	}
	int count1;
	std::cout << "Nth neighbor distances:" << std::endl;
	for (count1 = 0; count1 < _radiusMax; ++count1)
	{
		std::cout << count1 << ": " << _distanceArray[count1] << " " << std::endl;
	}
	std::cout << std::endl;
}
void Lattice::show_distance_neigh(void) const
{
	if (!_distanceNeigh)
	{
		std::cout << "_distanceNeigh does not exist. Call NthNeighborDistances!" << std::endl;
		exit(0);
	}
	int count1;
	std::cout << "Nth neighbor atom:" << std::endl;
	for (count1 = 0; count1 < _radiusMax; ++count1)
	{
		std::cout << count1 << ": " << _distanceNeigh[count1] << " " << std::endl;
	}
	std::cout << std::endl;
}

void Lattice::show_neighbors(const int &index) const
{
	if (_firstNeighborArray != NULL)
	{
		std::cout << "1nbors" << std::endl;
		for (int i = 0; i < _numberNthNeighbors[0]; ++i)
		{
			std::cout << _firstNeighborArray[index * _numberNthNeighbors[0] + i] << std::endl;
		}
	}
	if (_secondNeighborArray != NULL)
	{
		std::cout << "2nbors" << std::endl;
		for (int i = 0; i < _numberNthNeighbors[1]; ++i)
		{
			std::cout << _secondNeighborArray[index * _numberNthNeighbors[1] + i] << std::endl;
		}
	}
	if (_thirdNeighborArray != NULL)
	{
		std::cout << "3nbors" << std::endl;
		for (int i = 0; i < _numberNthNeighbors[2]; ++i)
		{
			std::cout << _thirdNeighborArray[index * _numberNthNeighbors[2] + i] << std::endl;
		}
	}
	if (_fourthNeighborArray != NULL)
	{
		std::cout << "4nbors" << std::endl;
		for (int i = 0; i < _numberNthNeighbors[3]; ++i)
		{
			std::cout << _fourthNeighborArray[index * _numberNthNeighbors[3] + i] << std::endl;
		}
	}
	if (_fifthNeighborArray != NULL)
	{
		std::cout << "5nbors" << std::endl;
		for (int i = 0; i < _numberNthNeighbors[4]; ++i)
		{
			std::cout << _fifthNeighborArray[index * _numberNthNeighbors[4] + i] << std::endl;
		}
	}
	if (_sixthNeighborArray != NULL)
	{
		std::cout << "6nbors" << std::endl;
		for (int i = 0; i < _numberNthNeighbors[5]; ++i)
		{
			std::cout << _sixthNeighborArray[index * _numberNthNeighbors[5] + i] << std::endl;
		}
	}
	if (_seventhNeighborArray != NULL)
	{
		std::cout << "7nbors" << std::endl;
		for (int i = 0; i < _numberNthNeighbors[6]; ++i)
		{
			std::cout << _seventhNeighborArray[index * _numberNthNeighbors[6] + i] << std::endl;
		}
	}
	if (_eighthNeighborArray != NULL)
	{
		std::cout << "8nbors" << std::endl;
		for (int i = 0; i < _numberNthNeighbors[7]; ++i)
		{
			std::cout << _eighthNeighborArray[index * _numberNthNeighbors[7] + i] << std::endl;
		}
	}
}

void Lattice::show_skyrmion_number_cells(void) const {
	for (int i = 0; i < _skNcellNum; ++i) {
		std::cout << _skNcells[i].i << " " << _skNcells[i].j << " " << _skNcells[i].k << " " << std::endl;
	}

}

int* Lattice::get_neighbor_array(const int &order) const
{
	switch (order)
	{
	case 1: // first nearest neighbours
		return _firstNeighborArray; //neighbour information
	case 2: //second nearest neighbours
		return _secondNeighborArray;
	case 3: //third nearest neighbours
		return _thirdNeighborArray;
	case 4: //third nearest neighbours
		return _fourthNeighborArray;
	case 5: //third nearest neighbours
		return _fifthNeighborArray;
	case 6:
		return _sixthNeighborArray;
	case 7:
		return _seventhNeighborArray;
	case 8:
		return _eighthNeighborArray;
	default:
		return NULL;
	}
}

int Lattice::get_number_nth_neighbors(const int &order) const
{
	return _numberNthNeighbors[order-1];
}

Threedim* Lattice::get_lattice_coordinate_array(void) const
{
	return _latticeCoordArray;
}

Threedim* Lattice::get_neighbor_vector_array(const int &order) const
{
	switch (order)
	{
	case 1: // first nearest neighbours
		return _firstNeighborVectorArray; //neighbour information
	case 2: //second nearest neighbours
		return _secondNeighborVectorArray;
	case 3: //third nearest neighbours
		return _thirdNeighborVectorArray;
	case 4: //third nearest neighbours
		return _fourthNeighborVectorArray;
	case 5: //third nearest neighbours
		return _fifthNeighborVectorArray;
	default:
		return NULL;
	}
}

double* Lattice::get_distance_array() const
{
	return _distanceArray;
}

int* Lattice::get_distance_neigh() const
{
	return _distanceNeigh;
}

int Lattice::get_radius_max() const
{
	return _radiusMax;
}

TopologicalChargeCell* Lattice::get_skN_cells() const 
{
	return _skNcells;
}

int Lattice::get_skN_cell_number() const 
{
	return _skNcellNum;
}

std::vector<Threedim> Lattice::get_dummy_skN_triangles()
{
	return _dummyskNTriangles;
}

Fourdim* Lattice::get_four_spin_cells(void) const
{
	return _fourSpinCells;
}

int Lattice::get_number_four_spin_cells_per_atom(void) const
{
	return _fourSpinCellsPerAtom;
}

ThreeSite* Lattice::get_three_site_cells(void) const
{
	return _threeSiteCells;
}

int Lattice::get_number_three_site_cells_per_atom(void) const
{
	return _threeSiteCellsPerAtom;
}

int Lattice::get_number_atoms() const
{
	return _numberAtoms;
}

int Lattice::get_center_site()
{
	if (_centerSite == -1)
	{
		find_center_site();
	}
	return _centerSite;
}

std::vector<Threedim> Lattice::get_wigner_seitz_cell(void) const
{
	return _wignerSeitzCell;
}
