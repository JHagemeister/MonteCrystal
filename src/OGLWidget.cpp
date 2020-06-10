/*
* OGLWidget.cpp
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

#include "OGLWidget.h"

// forward and further includes
#include "Hamiltonian.h"
#include "MyMath.h"
#include "Functions.h"
#include "MarkedSpinsHandler.h"
#include "Lattice.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <GL/glut.h>

#include <QMouseEvent>
#include <QMutex>
#include <QRubberBand>
#include <QApplication>
#include <QMessageBox>
OGLWidget::OGLWidget(QWidget *parent): 
	QOpenGLWidget(parent),
    _spinColor(1.0f,0.0f,0.0f,0.0f),_backgroundColor(0.0f, 0.0f, 0.0f, 1.0f)
{
	_markedSpinsHandler = NULL;

	_spinArray = NULL;
	_latticeCoordArray = NULL;
	_numberAtoms = 0;

	_mutex = new QMutex();

	_rubberBand = NULL;
	
	_showSpins = TRUE;
	_spinsSingleColor = FALSE;
	_visibleSpinOrder = 1;
	
    _showBase = FALSE;
    _baseOffset= glm::vec3{2.0f,2.0f,1.0f};
    _baseColor=glm::vec4(0.7f,0.7f,0.7f,1.0f);

	_colorMapType = NoMap;
	_colorMapColors.push_back(glm::vec4{ 0.0f,0.0f,0.8f,1.0f });
	_colorMapColors.push_back(glm::vec4{ 0.0f,0.0f,0.0f,1.0f });
	_colorMapColors.push_back(glm::vec4{ 1.0f,1.0f,1.0f,1.0f });
	_colorMapColors.push_back(glm::vec4{ 0.8f,0.0f,0.0f,1.0f });
	_energyIndex = 0;
	_minValueMap = 0;
	_maxValueMap = 0;
	_boolSetMinMaxLater = FALSE;
	_redScaleMap = Twodim{ 0,0 };
	_greenScaleMap = Twodim{ 0,0 };
    _blueScaleMap = Twodim{ 0,0 };
    _colorMapOffset =  -0.5;

	
	_hamiltonian = NULL;

    _spinMeshParams = { 15, 2.f, 3.f, 0.7f, 0.7f };
	_spinMeshParamsFname = "spin_model_params";
	Functions::get_spin_model_params(_spinMeshParams, _spinMeshParamsFname);
	_spinScaleFactor.x = _spinMeshParams.scale;
	_spinScaleFactor.y = _spinMeshParams.scale;
	_spinScaleFactor.z = _spinMeshParams.scale;
}

OGLWidget::~OGLWidget()
{
	delete _mutex;
	delete _rubberBand;
}

void OGLWidget::set_marked_spins_handler(MarkedSpinsHandler * markedSpinsHandler)
{
	_markedSpinsHandler = markedSpinsHandler;
}

void OGLWidget::delete_rubber_band(void)
{
	delete _rubberBand;
	_rubberBand = NULL;
}

void OGLWidget::set_lattice(QSharedPointer<Lattice> lattice)
{
	_lattice = lattice;
	_numberAtoms = lattice->get_number_atoms();
	_latticeCoordArray = lattice->get_lattice_coordinate_array();
	_wignerSeitzCell = lattice->get_wigner_seitz_cell();
	_skNcells = lattice->get_skN_cells();
	_skNcellNum = lattice->get_skN_cell_number();
	_dummyskNTriangles = lattice->get_dummy_skN_triangles();
}

void OGLWidget::set_hamiltonian(QSharedPointer<Hamiltonian> hamiltonian)
{
	/**
	* @param[in] hamiltonian Provide a pointer to the current Hamiltonian object. Possibly needed for energy 
	* color map output.
	*/

	_hamiltonian = hamiltonian;

	_energyIndex = search_single_energy(_colorMapID);
	if (_energyIndex < 0 && _colorMapType == SingleEnergyMap)
	{
		// disable color map output if energy map of single energy was specified which is not part of the 
		// Hamiltonian
		_colorMapType = NoMap;
	}

	_anisotropyIndexes = search_anisotropy_energies();
	if (_anisotropyIndexes.size() == 0 && _colorMapType == AnisotropyEnergyMap)
	{
		// disable color map if anisotropy color map was required but Hamiltonian does not contain any 
		// anisotropy energies
		_colorMapType = NoMap;
	}

	// if _boolSetMinMaxLater is true, the minimum and maximum values of the desired energy property for color
	// map output is determined at this point
	if (_boolSetMinMaxLater == TRUE) // don't do anything if _boolSetMinMaxLater is false
	{
		if (_colorMapType == TotalEnergyMap)
		{
			double* values = new double[_numberAtoms];
			for (int i = 0; i < _numberAtoms; i++)
			{
				values[i] = _hamiltonian->total_energy(i);
			}
			MyMath::min_max_double(values, _numberAtoms, _minValueMap, _maxValueMap);
			delete[] values;
		}

		if (_colorMapType == SingleEnergyMap)
		{
			double* values = new double[_numberAtoms];
			for (int i = 0; i < _numberAtoms; i++)
			{
				values[i] = _hamiltonian->single_part_energy(_energyIndex, i);
			}
			MyMath::min_max_double(values, _numberAtoms, _minValueMap, _maxValueMap);
			delete[] values;
		}

		if (_colorMapType == AnisotropyEnergyMap)
		{
			double* values = new double[_numberAtoms];
			for (int j = 0; j < _numberAtoms; j++)
			{
				values[j] = 0;
			}
			for (int i = 0; i < _anisotropyIndexes.size(); i++)
			{
				for (int j = 0; j < _numberAtoms; j++)
				{
					values[j] += _hamiltonian->single_part_energy(_anisotropyIndexes[i], j);
				}
				MyMath::min_max_double(values, _numberAtoms, _minValueMap, _maxValueMap);
			}
			delete[] values;
		}
	}
}

void OGLWidget::set_bool_display_spins(int boolSpins)
{
	/**
	* @param[in] boolSpins 0 to disable spin output and 1 for display of spin output
	*/

	_showSpins = boolSpins;
}

void OGLWidget::set_spin_color(glm::vec4 color)
{
	/**
	* @param[in] color Four component color vector (rgba).
	*/

	_spinsSingleColor = 1;
	_spinColor = color;
}

void OGLWidget::set_spin_color_rgb_scale(void)
{
	/**
	* Spin color set in rgb-color scale.
	*/

	_spinsSingleColor = FALSE;
}

void OGLWidget::set_spin_model(SpinMeshParams params)
{
	_spinMeshParams = params;
	_spinScaleFactor.x = _spinMeshParams.scale;
	_spinScaleFactor.y = _spinMeshParams.scale;
	_spinScaleFactor.z = _spinMeshParams.scale;
	Functions::save_spin_model_params(params,_spinMeshParamsFname);
	_spinMesh->update(params);
}

void OGLWidget::set_wire_frame_spins(int * indexes, int size)
{
	for (int i = 0; i < size; ++i)
	{
		_wireFrameSpins.insert(indexes[i]);
		_filledSpins.erase(indexes[i]);
		if (_visibleFilledSpins.count(indexes[i]) > 0)
		{
			_visibleFilledSpins.erase(indexes[i]);
			_visibleWireFrameSpins.insert(indexes[i]);
		}
	}
}

void OGLWidget::set_wire_frame_spins(std::vector<int>& indexes)
{
	for (auto it = indexes.begin(); it != indexes.end(); ++it)
	{
		_wireFrameSpins.insert(*it);
		_filledSpins.erase(*it);
		if (_visibleFilledSpins.count(*it) > 0)
		{
			_visibleFilledSpins.erase(*it);
			_visibleWireFrameSpins.insert(*it);
		}
	}
}

void OGLWidget::set_spins_filled(void)
{
	_wireFrameSpins.clear();
	for (int i = 0; i < _numberAtoms; ++i)
	{
		_filledSpins.insert(i);
	}
	for (auto it = _visibleWireFrameSpins.begin(); it != _visibleWireFrameSpins.end(); ++it)
	{
		_visibleFilledSpins.insert(*it);
	}
	_visibleWireFrameSpins.clear();
}

void OGLWidget::set_filled_spins(int * indexes, int size)
{
	for (int i = 0; i < size; ++i)
	{
		_filledSpins.insert(indexes[i]);
		_wireFrameSpins.erase(indexes[i]);
		if (_visibleWireFrameSpins.count(indexes[i]) > 0)
		{
			_visibleWireFrameSpins.erase(indexes[i]);
			_visibleFilledSpins.insert(indexes[i]);
		}
	}
}

void OGLWidget::set_filled_spins(std::vector<int>& indexes)
{
	for (auto it = indexes.begin(); it != indexes.end(); ++it)
	{
		_filledSpins.insert(*it);
		_wireFrameSpins.erase(*it);
		if (_visibleWireFrameSpins.count(*it) > 0)
		{
			_visibleWireFrameSpins.erase(*it);
			_visibleFilledSpins.insert(*it);
		}
	}
}

void OGLWidget::set_visible_spins_order(int order)
{
	if (order == 1)
	{
		_visibleSpinOrder = order;
		set_all_spins_visible();
	}
	if (order > 1 && order < 20)
	{
		int numberNeighbors = _lattice->get_number_nth_neighbors(1);

		Threedim min{0,0,0};
		Threedim max{0,0,0};
		MyMath::min_max_threedim(_latticeCoordArray, _numberAtoms, min, max);

		if (abs(max.z - min.z) < PRECISION)
		{
			_visibleSpinOrder = order;
			set_all_spins_invisible();
			std::vector<Threedim> sites;

			int effectiveNumberNeighbors = 0;
			int* neighbors = _lattice->get_neighbor_array(1);
			for (int i = 0; i < _numberAtoms; ++i)
			{
				int tempCount = 0;
				for (int j = 0; j < numberNeighbors; ++j)
				{
					if (neighbors[i*numberNeighbors + j] != -1)
					{
						++tempCount;
					}
				}
				if (tempCount > effectiveNumberNeighbors) 
				{ 
					effectiveNumberNeighbors = tempCount;

				}
			}

			if (effectiveNumberNeighbors == 4)
			{
				sites = Lattice::get_sites_square_lattice(max.y-min.y + 1, max.x-min.x + 1);
			}
			else if (effectiveNumberNeighbors == 6)
			{
				int m = (max.y - min.y) * 2. / sqrt(3.);
				sites = Lattice::get_sites_hexagonal_lattice(m , max.x - min.x);
			}
			for (auto it = sites.begin(); it != sites.end(); ++it)
			{
				*it = MyMath::mult(*it,order);
				*it = MyMath::add(*it,min);
			}
			int centerSite = _lattice->get_center_site();
			Threedim diffVec = MyMath::difference(_latticeCoordArray[centerSite], *sites.begin());
			double dist = MyMath::norm(diffVec);
			for (auto it = sites.begin(); it != sites.end(); ++it)
			{
				Threedim diffVec1 = MyMath::difference(_latticeCoordArray[centerSite], *it);
				double dist1 = MyMath::norm(diffVec1);
				if (dist1 < dist)
				{
					dist = dist1;
					diffVec = diffVec1;
				}
			}

			for (auto it = sites.begin(); it != sites.end(); ++it)
			{
				*it = MyMath::add(*it,diffVec);
				if ((*it).x < max.x + PRECISION && (*it).x > min.x - PRECISION
					&& (*it).y < max.y + PRECISION && (*it).y > min.y - PRECISION)
				{
					int found = FALSE;
					int index = 0;
					while (!found && index < _numberAtoms)
					{
						double dist = MyMath::norm(MyMath::difference(_latticeCoordArray[index],*it));
						if (dist < PRECISION)
						{
							set_spin_visible(index);
							found = TRUE;
						}
						++index;
					}
				}	
			}
		}
	}
}

double OGLWidget::get_minimal_distance_to_lattice_site(Threedim vector, int &siteIndex)
{
    double distance = 10; // arbitrary
	for (int i = 0; i < _numberAtoms; ++i)
	{
		double test = MyMath::norm(MyMath::difference(_latticeCoordArray[i], vector));
		if (test < distance)
		{
			siteIndex = i;
			distance = test;
		}
	}
	return distance;
}

int OGLWidget::get_visible_spins_order(void)
{
	return _visibleSpinOrder;
}

void OGLWidget::set_all_spins_visible(void)
{
	for (auto it = _wireFrameSpins.begin(); it != _wireFrameSpins.end(); ++it)
	{
		_visibleWireFrameSpins.insert(*it);
	}
	for (auto it = _filledSpins.begin(); it != _filledSpins.end(); ++it)
	{
		_visibleFilledSpins.insert(*it);
	}
}

void OGLWidget::set_all_spins_invisible(void)
{
	_visibleWireFrameSpins.clear();
	_visibleFilledSpins.clear();
}

void OGLWidget::set_spin_visible(int index)
{
	if (_wireFrameSpins.count(index))
	{
		_visibleWireFrameSpins.insert(index);
	}
	if (_filledSpins.count(index))
	{
		_visibleFilledSpins.insert(index);
	}
}

void OGLWidget::set_spin_invisible(int index)
{
	_visibleWireFrameSpins.erase(index);
	_visibleFilledSpins.erase(index);
}

void OGLWidget::set_spins(Threedim * spinArray, int size, int boolNew)
{
	_spinArray = spinArray;
	_numberAtoms = size;

	if (boolNew)
	{
		_filledSpins.clear();
		_visibleFilledSpins.clear();
		_wireFrameSpins.clear();
		_visibleWireFrameSpins.clear();
		for (int i = 0; i < size; ++i)
		{
			_filledSpins.insert(i);
			_visibleFilledSpins.insert(i);
		}
	}
}

void OGLWidget::set_background_color(glm::vec4 color)
{
	_backgroundColor = color;
}

glm::vec4 OGLWidget::get_background_color(void)
{
	return _backgroundColor;
}

void OGLWidget::set_color_map_type(ColorMapType colorMapType, std::string identifier)
{
	/**
	* @param[in] colorMapType Specify type of color map.
	* @param[in] identifier Somewhat redundant to colorMapType but provides more specific information.
	*/

	_colorMapType = colorMapType;
	if (colorMapType == AnisotropyEnergyMap)
	{
		_anisotropyIndexes = search_anisotropy_energies();
	}

	_colorMapID = identifier;
	if (_colorMapType == SingleEnergyMap)
	{
		int index = search_single_energy(identifier);
		if (index < 0)
		{
			_colorMapType = NoMap;
		}
		else
		{
			_energyIndex = index;
		}
	}
	emit color_map_text(QString::fromStdString(""));
}

void OGLWidget::set_color_map_min_max(double min, double max)
{
	/**
	* Set minimum and maximum value of property to be shown as color map.
	*
	* @param[in] min minimum value
	* @param[in] max maximum value
	*/

	_minValueMap = min;
	_maxValueMap = max;
	_boolSetMinMaxLater = FALSE;
}

void OGLWidget::set_color_map_min_max_later(void)
{
	/**
	* Enable that minimum and maximum values for property to be shown as color map will be determined upon
	* start of simulation.
	*/

	_boolSetMinMaxLater = TRUE;
}

void OGLWidget::set_color_map_colors(std::vector<glm::vec4> colorMapColors)
{
	/**
	* Set colors that will be used for display of color map consisting of four rgb colors. The first will be
	* used if local value of property is smaller than previously defined minimum value. The last will be used
	* if local value of property is larger than previously defined maximum value. The two remaining colors are
	* used for color scaling between previously defined minimum and maximum values.
	*/

	_colorMapColors = colorMapColors;
}

void OGLWidget::set_custom_magnetization_direction_color_map(Threedim direction)
{
	/**
	* Specifiy a direction for magnetization contrast for color map.
	*/

	_customMagnetizationDirection = direction;
}

int OGLWidget::get_bool_display_spins(void) const
{
	/**
	* @return boolean value about whether spins are currently displayed
	*/

	return _showSpins;
}

glm::vec4 OGLWidget::get_spin_color(void) const
{
	/**
	* @return current spin color
	*/

	return _spinColor;
}

int OGLWidget::get_bool_spin_single_color(void) const
{
	/**
	* @return boolean about whether spins are mono-colored or rgb-colored
	*/

	return _spinsSingleColor;
}

void OGLWidget::get_color_map_type(ColorMapType &colorMapType, std::string &colorMapID) const
{
	/**
	* @param[out] colorMapType Currently chosen type of color map
	* @param[out] colorMapID Current string identifier for color map (somewhat redundant to colorMapType)
	*/

	colorMapType = _colorMapType;
	colorMapID = _colorMapID;
}

void OGLWidget::get_color_map_min_max(double & min, double & max) const
{
	/**
	* @param[out] min Current specified minimum value of color map property
	* @param[out] max Current specified maximum value of color map property
	*/

	min = _minValueMap;
	max = _maxValueMap;
}

std::vector<glm::vec4> OGLWidget::get_color_map_colors(void)
{
	/**
	* @return Colors used for color coding of color map.
	*/

	return _colorMapColors;
}

Threedim OGLWidget::get_custom_magnetization_direction_color_map(void)
{
	/**
	* Get specified direction for magnetization contrast for color map.
	*
	* @return spatial direction vector
	*/

	return _customMagnetizationDirection;
}

void OGLWidget::get_current_min_max(ColorMapType colorMapType, std::string identifier, double& min, 
	double& max)
{
	/**
	* Obtain current minimum and maximum values for a given color map type.
	*
	* @param[in] colorMapType Specification of color map type
	* @param[in] identifier Specification of color map type
	* @param[out] minimum value of color map property
	* @param[out] maximum value of color map property
	*/

	min = 0;
	max = 0;
	if (colorMapType == TotalEnergyMap && _hamiltonian != NULL)
	{
		double* values = new double[_numberAtoms];
		for (int i = 0; i < _numberAtoms; i++)
		{
			values[i] = _hamiltonian->total_energy(i);
		}
		MyMath::min_max_double(values, _numberAtoms, min, max);
		delete[] values;
	}
	else if (colorMapType == SingleEnergyMap && _hamiltonian != NULL)
	{
		int index = search_single_energy(identifier);
		if (index > -0.5)
		{
			double* values = new double[_numberAtoms];
			for (int i = 0; i < _numberAtoms; i++)
			{
				values[i] = _hamiltonian->single_part_energy(index, i);
			}
			MyMath::min_max_double(values, _numberAtoms, min, max);
			delete[] values;
		}
	}
	else if (colorMapType == AnisotropyEnergyMap && _hamiltonian != NULL)
	{
		std::vector<int> anisotropyIndexes = search_anisotropy_energies();
		double* values = new double[_numberAtoms];
		for (int j = 0; j < _numberAtoms; j++)
		{
			values[j] = 0;
		}
		for (int i = 0; i < anisotropyIndexes.size(); i++)
		{
			for (int j = 0; j < _numberAtoms; j++)
			{
				values[j] += _hamiltonian->single_part_energy(anisotropyIndexes[i], j);
			}
			MyMath::min_max_double(values, _numberAtoms, min, max);	
		}
		delete[] values;
	}
	else if (colorMapType == TopologicalChargeMap && _skNcellNum > 0)
	{
		double* values = new double[_skNcellNum];
		for (int i = 0; i < _skNcellNum; i++)
		{
			TopologicalChargeCell cell = _skNcells[i];
			values[i] = MyMath::topological_charge(_spinArray[_skNcells[i].i], _spinArray[_skNcells[i].j],
				_spinArray[_skNcells[i].k]);
		}
		MyMath::min_max_double(values, _skNcellNum, min, max);
		delete[] values;
	}
}

void OGLWidget::update_color_scale(void)
{
	/**
	* Update color scale for previously defined color map colors and minimum and maximum values of color map
	* property
	*/

	_redScaleMap = MyMath::two_point_equation(_minValueMap, _colorMapColors[1].r, _maxValueMap, 
		_colorMapColors[2].r);
	_greenScaleMap = MyMath::two_point_equation(_minValueMap, _colorMapColors[1].g, _maxValueMap, 
		_colorMapColors[2].g);
	_blueScaleMap = MyMath::two_point_equation(_minValueMap, _colorMapColors[1].b, _maxValueMap, 
		_colorMapColors[2].b);
}

void OGLWidget::initializeGL()
{
    std::cout << "OGL "  << format().majorVersion() << "."  << format().minorVersion() << std::endl;
	_glf = context()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	if (!_glf) {
	    std::cout << "Failed to obtain QOpenGLFunctions_3_3_Core" << std::endl;
  //  	    exit(1);
	}

	// Define the viewport dimensions
	_glf->glViewport(0, 0, width(), height());

	// Setup some OpenGL options
	_glf->glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	_shader.setup_shader(":/src/lighting.vs", ":/src/lighting.frag", _glf);

	// Load arrow model for representation of spin direction
	_spinMesh = std::make_shared<SpinMesh>(_glf, _spinMeshParams);

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Draw filled 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OGLWidget::paintGL()
{
	_mutex->lock();
	// Clear the colorbuffer
	_glf->glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	_glf->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// paint color map
	switch(_colorMapType)
	{
	case NoMap:
		break;
	case TopologicalChargeMap:
		paint_color_map_topological_charge();
		break;
	default:
		paint_color_map_lattice_site_centered();
		break;
	}

	// paint spins
	if (_showSpins)
	{
		paint_spins();
	}


    if( _showBase){
        paint_base();
    }

	_mutex->unlock();
}

void OGLWidget::paint_spins(void)
{
	/**
	* Paint spins.
	*/

	_shader.use(_glf);   // <-- Don't forget this one!

    // Transformation matrices

    _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "projection"), 1, GL_FALSE,
        glm::value_ptr(_projection));
    _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "view"), 1, GL_FALSE, glm::value_ptr(_view));

	for (auto it = _visibleFilledSpins.begin(); it != _visibleFilledSpins.end(); ++it)
	{
		paint_single_spin(*it);
	}

	_glf->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (auto it = _visibleWireFrameSpins.begin(); it != _visibleWireFrameSpins.end(); ++it)
	{
		paint_single_spin(*it);
	}
	_glf->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}




void OGLWidget::paint_single_spin(int index)
{
    glm::mat4 model (1.0f);

	// translation to lattice site
	model = glm::translate(model, glm::vec3(_latticeCoordArray[index].x, _latticeCoordArray[index].y,
		_latticeCoordArray[index].z));

	glm::vec3 ez(0.0f, 0.0f, 1.0f);
	glm::vec3 direction(_spinArray[index].x, _spinArray[index].y, _spinArray[index].z);
	glm::vec3 rotDir = glm::cross(ez, direction);
	if (glm::l2Norm(rotDir) < 0.001)
	{
		rotDir.x = 1.0f;
		rotDir.y = 0.0f;
		rotDir.z = 0.0f;
	}
	else
	{
		glm::vec3 rotDir = glm::normalize(rotDir);
	}

	float dotProd = glm::dot(ez, direction);

	// inaccuracies can lead to dot products larger 1 and smaller -1
	if (dotProd > 1)
	{
		dotProd = 1;
	}
	else
	{
		if (dotProd < -1)
		{
			dotProd = -1;
		}
	}
	float angle = acos(dotProd);
	// rotate arrow dummy which points in z-direction into spin direction
	model = glm::rotate(model, angle, rotDir);
	model = glm::scale(model, _spinScaleFactor);

	_glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "model"), 1, GL_FALSE,
		glm::value_ptr(model));

	GLint objectColorLoc = _glf->glGetUniformLocation(_shader._program, "objectColor");
	GLint lightColorLoc = _glf->glGetUniformLocation(_shader._program, "lightColor");
	GLint lightPosLoc = _glf->glGetUniformLocation(_shader._program, "lightPos");

	if (_spinsSingleColor)
	{
		// mono-colored spins
		_glf->glUniform4f(objectColorLoc, _spinColor.x, _spinColor.y, _spinColor.z, 1.0f);
	}
	else
	{
		// rgb-colored spins
		glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
		if (_spinArray[index].z > 0)
		{
			color.r = _spinArray[index].z;
			color.g = 1 - _spinArray[index].z;
		}
		else
		{
			color.g = 1 + _spinArray[index].z;
			color.b = -_spinArray[index].z;
		}
		_glf->glUniform4f(objectColorLoc, color.r, color.g, color.b, color.a);
	}

	_glf->glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	_glf->glUniform3f(lightPosLoc, _lightPos.x, _lightPos.y, _lightPos.z);

	_spinMesh->draw();
}

void OGLWidget::paint_color_map_lattice_site_centered(void)
{
	/**
	* Paint lattice-site centered color maps. This is used for e.g. energy maps or magnetization maps.
	*/

	if (_wignerSeitzCell.size() > 0)
	{
		_shader.use(_glf);

        _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "projection"), 1, GL_FALSE,
            glm::value_ptr(_projection));
        _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "view"), 1, GL_FALSE, glm::value_ptr(_view));

		std::vector<Vertex> vertices;
		vertices.push_back({ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f } });
		for (int i = 0; i < _wignerSeitzCell.size(); i++)
		{
			vertices.push_back({ glm::vec3{ (float)_wignerSeitzCell[i].x, (float)_wignerSeitzCell[i].y,
                (float)_wignerSeitzCell[i].z }, glm::vec3{ 0.0f, 0.0f, 1.0f } });
		}

		std::vector<GLuint> indices;
		for (GLuint i = 0; i < _wignerSeitzCell.size() - 1; i++)
		{
			indices.push_back((GLuint)0);
			indices.push_back((GLuint)i + 1);
			indices.push_back((GLuint)i + 2);
		}
		indices.push_back((GLuint)0);
		indices.push_back((GLuint)_wignerSeitzCell.size());
		indices.push_back((GLuint)1);

		Mesh mesh(_glf, vertices, indices);
		

		double colorMapValue = 0;

		for (int i = 0; i < _numberAtoms; ++i)
		{
            glm::mat4 model (1.0f);
			model = glm::translate(model, glm::vec3(_latticeCoordArray[i].x, _latticeCoordArray[i].y,
				_latticeCoordArray[i].z + _colorMapOffset));

			_glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "model"), 1, GL_FALSE,
				glm::value_ptr(model));

			GLint objectColorLoc = _glf->glGetUniformLocation(_shader._program, "objectColor");
			GLint lightColorLoc = _glf->glGetUniformLocation(_shader._program, "lightColor");
			GLint lightPosLoc = _glf->glGetUniformLocation(_shader._program, "lightPos");

			glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
			double value = 0;
			switch(_colorMapType)
			{
			case TotalEnergyMap:
				value = _hamiltonian->total_energy(i);
				scale_map_value(value, color);
				colorMapValue += value;
				break;
			case SingleEnergyMap:
				value = _hamiltonian->single_part_energy(_energyIndex, i);
				scale_map_value(value, color);
				colorMapValue += value;
				break;
			case AnisotropyEnergyMap:
				value = 0;
				for (int j = 0; j < _anisotropyIndexes.size(); j++)
				{
					value += _hamiltonian->single_part_energy(_anisotropyIndexes[j], i);
				}
				scale_map_value(value, color);
				colorMapValue += value;
				break;
			case MagnetizationMap:
			{
				color.r = (float)(_spinArray[i].x + 1) / 2.;
				color.g = (float)(_spinArray[i].y + 1) / 2.;
				color.b = (float)(_spinArray[i].z + 1) / 2.;
				break;
			}
			case CustomDirectionMagnetizationMap:
				value = MyMath::dot_product(_spinArray[i], _customMagnetizationDirection);
				scale_map_value(value, color);
				break;
			}

			_glf->glUniform4f(objectColorLoc, color.r, color.g, color.b, color.a);
			_glf->glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
			_glf->glUniform3f(lightPosLoc, _lightPos.x, _lightPos.y, _lightPos.z);

			mesh.draw();
		}
		
		if (_colorMapType == TotalEnergyMap || _colorMapType == SingleEnergyMap ||
			_colorMapType == AnisotropyEnergyMap)
		{
			colorMapValue /= _numberAtoms;
			emit color_map_text(QString::fromStdString(_colorMapID) + ": " + QString::number(colorMapValue));
		}
	}
}
void OGLWidget::paint_base(void)
{
    /**
    * Paint a base plane.
    */
    _shader.use(_glf);
    glm::mat4 model (1.0f);
    _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "model"), 1, GL_FALSE,
        glm::value_ptr(model));
    _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "projection"), 1, GL_FALSE,
        glm::value_ptr(_projection));
    _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "view"), 1, GL_FALSE, glm::value_ptr(_view));


    Threedim min = { 0,0,0 };
    Threedim max = { 0,0,0 };
    if (_latticeCoordArray != NULL){
    MyMath::min_max_threedim(_latticeCoordArray, _numberAtoms, min, max);
        }


    std::vector<Vertex> vertices;
    vertices.push_back({glm::vec3{min.x-_baseOffset.x,min.y-_baseOffset.y,min.z-_baseOffset.z},glm::vec3{0.0f,0.0f,1.0f}});
    vertices.push_back({glm::vec3{max.x+_baseOffset.x,min.y-_baseOffset.y,min.z-_baseOffset.z},glm::vec3{0.0f,0.0f,1.0f}});
    vertices.push_back({glm::vec3{max.x+_baseOffset.x,max.y+_baseOffset.y,min.z-_baseOffset.z},glm::vec3{0.0f,0.0f,1.0f}});
    vertices.push_back({glm::vec3{min.x-_baseOffset.x,max.y+_baseOffset.y,min.z-_baseOffset.z},glm::vec3{0.0f,0.0f,1.0f}});
    std::vector<GLuint> indices = std::vector<GLuint>({(GLuint)0,(GLuint)1,(GLuint)2,(GLuint)2,(GLuint)3,(GLuint)0,});


    Mesh mesh(_glf, vertices, indices);
    GLint objectColorLoc = _glf->glGetUniformLocation(_shader._program, "objectColor");
    GLint lightColorLoc = _glf->glGetUniformLocation(_shader._program, "lightColor");
    GLint lightPosLoc = _glf->glGetUniformLocation(_shader._program, "lightPos");

    glm::vec4 color=_baseColor;
    _glf->glUniform4f(objectColorLoc, color.r, color.g, color.b, color.a);
    _glf->glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
    _glf->glUniform3f(lightPosLoc, _lightPos.x, _lightPos.y, _lightPos.z);

    mesh.draw();

}


void OGLWidget::paint_color_map_topological_charge(void)
{
	/**
	* Paint color map of the topological charge.
	*/
	
	if (_skNcellNum > 0)
	{
		_shader.use(_glf);

;
        glm::mat4 model (1.0f);
        _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "model"), 1, GL_FALSE,
            glm::value_ptr(model));

        _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "projection"), 1, GL_FALSE,
            glm::value_ptr(_projection));
        _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "view"), 1, GL_FALSE, glm::value_ptr(_view));

		// triangles that are used to plot the color map
		std::vector<Mesh> meshes;
		for (int i = 0; i < _dummyskNTriangles.size()/3; i++)
		{
			std::vector<Vertex> vertices;
			vertices.push_back({ glm::vec3{ _dummyskNTriangles[3*i].x,  _dummyskNTriangles[3 * i].y,
				_dummyskNTriangles[3 * i].z }, glm::vec3{ 0.0f, 0.0f, 1.0f } });

			vertices.push_back({ glm::vec3{ _dummyskNTriangles[3 * i + 1].x,  _dummyskNTriangles[3 * i + 1].y,
				_dummyskNTriangles[3 * i + 1].z }, glm::vec3{ 0.0f, 0.0f, 1.0f } });

			vertices.push_back({ glm::vec3{ _dummyskNTriangles[3 * i + 2].x,  _dummyskNTriangles[3 * i + 2].y,
				_dummyskNTriangles[3 * i + 2].z }, glm::vec3{ 0.0f, 0.0f, 1.0f } });

			std::vector<GLuint> indices;
			indices.push_back((GLuint) 0);
			indices.push_back((GLuint) 1);
			indices.push_back((GLuint) 2);

			meshes.push_back(Mesh(_glf, vertices, indices));
		}

		double colorMapValue = 0; // used for the sum of single color map values

		for (int i = 0; i < _skNcellNum; ++i) // sum over all skyrmion cells
		{
            glm::mat4 model (1.0f);
			model = glm::translate(model, glm::vec3(_skNcells[i].position.x, _skNcells[i].position.y,
				_skNcells[i].position.z + _colorMapOffset));

			_glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "model"), 1, GL_FALSE,
				glm::value_ptr(model));

			GLint objectColorLoc = _glf->glGetUniformLocation(_shader._program, "objectColor");
			GLint lightColorLoc = _glf->glGetUniformLocation(_shader._program, "lightColor");
			GLint lightPosLoc = _glf->glGetUniformLocation(_shader._program, "lightPos");

			glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
			double value = MyMath::topological_charge(_spinArray[_skNcells[i].i], _spinArray[_skNcells[i].j],
				_spinArray[_skNcells[i].k]);
			scale_map_value(value, color);
			colorMapValue += value;

			_glf->glUniform4f(objectColorLoc, color.r, color.g, color.b, color.a);
			_glf->glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
			_glf->glUniform3f(lightPosLoc, _lightPos.x, _lightPos.y, _lightPos.z);

			meshes[_skNcells[i].triangleIndex].draw();
		}
		emit color_map_text(QString::fromStdString(_colorMapID) + ": " + QString::number(colorMapValue));
	}
}

void OGLWidget::paint_points(void)
{
	if (_points.size() > 0)
	{
		_shader.use(_glf);


        _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "projection"), 1, GL_FALSE,
            glm::value_ptr(_projection));
        _glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "view"), 1, GL_FALSE, glm::value_ptr(_view));

		std::vector<Vertex> vertices;
		vertices.push_back({ glm::vec3{ -0.25f, (float) -sqrt(3)/12., -0.499f }, glm::vec3{ 0.0f, 0.0f, 1.0f } });
		vertices.push_back({ glm::vec3{ 0.25f, (float) -sqrt(3)/12., -0.499f }, glm::vec3{ 0.0f, 0.0f, 1.0f } });
		vertices.push_back({ glm::vec3{ 0.0f, (float) sqrt(3)*2/12., -0.499f }, glm::vec3{ 0.0f, 0.0f, 1.0f } });

		std::vector<GLuint> indices;
		indices.push_back((GLuint)0);
		indices.push_back((GLuint)1);
		indices.push_back((GLuint)2);

		Mesh mesh(_glf, vertices, indices);

		for (std::vector<int>::iterator it = _points.begin(); it != _points.end(); ++it)
		{
            glm::mat4 model (1.0f);
			Threedim point = _latticeCoordArray[*it];
			model = glm::translate(model, glm::vec3(point.x, point.y, point.z));

			_glf->glUniformMatrix4fv(_glf->glGetUniformLocation(_shader._program, "model"), 1, GL_FALSE,
				glm::value_ptr(model));

			GLint objectColorLoc = _glf->glGetUniformLocation(_shader._program, "objectColor");
			GLint lightColorLoc = _glf->glGetUniformLocation(_shader._program, "lightColor");
			GLint lightPosLoc = _glf->glGetUniformLocation(_shader._program, "lightPos");

			glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);

			_glf->glUniform4f(objectColorLoc, color.r, color.g, color.b, color.a);
			_glf->glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
			_glf->glUniform3f(lightPosLoc, _lightPos.x, _lightPos.y, _lightPos.z);

			mesh.draw();
		}
	}
}

void OGLWidget::scale_map_value(double value, glm::vec4 & color)
{
	/**
	* @param[in] value A local value of the color map property
	* @param[out] color The color value which corresponds to the other input parameter.
	*/

	if (value < _minValueMap)
	{
		color = _colorMapColors[0];
	}
	else if (value > _maxValueMap)
	{
		color = _colorMapColors[3];
	}
	else
	{
		color.r = _redScaleMap.x*value + _redScaleMap.y;
		color.g = _greenScaleMap.x*value + _greenScaleMap.y;
		color.b = _blueScaleMap.x*value + _blueScaleMap.y;
	}
}

int OGLWidget::search_single_energy(std::string identifier)
{
	/**
	* Search index of single energy in Hamiltonian by a string identifier.
	*
	* @param[in] identifier String by which index of single energy in Hamiltonian will be determined
	* @return energy index (-1 if not found)
	*/

	int index = -1;
	std::vector<std::shared_ptr<Energy>> energies = _hamiltonian->get_energies();
	for (int i = 0; i < energies.size(); i++)
	{
		if (energies[i]->get_string_id().compare(identifier) == 0)
		{
			index = i;
		}
	}
	return index;
}

std::vector<int> OGLWidget::search_anisotropy_energies(void)
{
	/**
	* Search indexes of all anisotropy energies.
	*
	* @return vector with indexes of anisotropy energies in Hamiltonian.
	*/

	std::vector<int> indices;
	for (int i = 0; i < _hamiltonian->get_number_energies(); i++)
	{
		std::string identifier(_hamiltonian->get_energies()[i]->get_string_id());
		if (identifier.compare(0, 10, "E_HexAniso") == 0 || identifier.compare(0, 12, "E_UniaxAniso") == 0)
		{
			indices.push_back(i);
		}
	}
	return indices;
}

void OGLWidget::resizeGL(int width, int height)
{
	_glf->glViewport(0, 0, width, height);
    adjust_camera();
}

void OGLWidget::adjust_camera_to_lattice(void){

        //get size of the lattice
        Threedim min = { 0,0,0 };
        Threedim max = { 0,0,0 };
        MyMath::min_max_threedim(_latticeCoordArray, _numberAtoms, min, max);
       float windowAspectRatio = (float)height() / (float)width();
       float offset = 1.5; // smallest distance between lattice and widget edge in crystal lattice constants
       float latticeAspectRatio = (max.y - min.y + 2.*offset) / (max.x - min.x + 2.*offset);
       // for orthogonal projection: scale view so that lattice optimally fills out widget size
       if (latticeAspectRatio > windowAspectRatio)
       {
           _viewY = (max.y - min.y) / 2. + offset;
           _viewX = _viewY / windowAspectRatio;

       }
       else
       {
           _viewX = (max.x - min.x) / 2. + offset;
           _viewY = _viewX * windowAspectRatio;
       }

       //for perspective projection: calculate distance between lattice center and camera so that lattice optimally fills out widget size
       float rho_x = ((max.x - min.x)/2+offset)/tan(M_PI/8) * windowAspectRatio ;
       float rho_y = ((max.y - min.y)/2+offset)/tan(M_PI/8) ;
       if (rho_x >rho_y){
           _cameraAngle.z = rho_x;
       }
       else {
           _cameraAngle.z = rho_y;
       }

    adjust_camera();
}
void OGLWidget::adjust_camera()
{
    /**
        Adjust camera position, orientation and projection/view matrix according to values from  adjust_camera_to_lattice() and CameraWindow.
    */

    _mutex->lock();
    if (_latticeCoordArray == NULL)
    {
        _cameraPos = glm::vec3(17.0f, 3.5f, 30.0f); // random default
    }
    else
    {
        //get center of lattice
        Threedim min = { 0,0,0 };
        Threedim max = { 0,0,0 };
        MyMath::min_max_threedim(_latticeCoordArray, _numberAtoms, min, max);
        glm::vec3 center= {(max.x + min.x)/2,(max.y + min.y)/2,(max.z + min.z)/2};


        //setup camera coordinate system and projection matrix for orthogonal projection
        if (_orthoView){
           _cameraPos= _viewOffset+center;      //place camera above center
           _cameraPos.z =_cameraAngle.z;
           _cameraFront = glm::vec3(0.0,0.0,-1.0);  // coordinatsystem of camera and projection is independent of lattice
           _cameraUp = glm::vec3(0.0,1.0,0);
           _projection = glm::ortho(-_viewX, _viewX, -_viewY, _viewY, 0.1f, 100.0f);
        }
        else {
            //setup camera coordinate system and projection matrix for persepective projection

            // use the spherical coordinates given in _cameraAngle to set coordinate system of camera in carteseane coordinates
            _cameraFront=glm::vec3{sin((_cameraAngle.x/180)*M_PI)* sin(_cameraAngle.y/180*M_PI) * fabs(_cameraAngle.z),
                                               cos((_cameraAngle.x/180)*M_PI)* sin(_cameraAngle.y/180*M_PI) * fabs(_cameraAngle.z),
                                               -cos((_cameraAngle.y/180)*M_PI)* fabs(_cameraAngle.z)} ;


            _cameraUp   =glm::vec3{sin((_cameraAngle.x/180)*M_PI) * cos(_cameraAngle.y/180*M_PI) ,
                                               cos((_cameraAngle.x/180)*M_PI) * cos(_cameraAngle.y/180*M_PI),
                                               sin((_cameraAngle.y/180)*M_PI)};


            // place camera so that the center of view is at _viewOffset relative to the center of the lattice
            _cameraPos  =glm::vec3{_viewOffset.x + center.x-_cameraFront.x,
                                   _viewOffset.y + center.y-_cameraFront.y,
                                   _viewOffset.z + center.z-_cameraFront.z} ;

            // set perspective projection matrix. vertical angle of view pi/4
            _projection = glm::perspective(glm::radians(45.0f), float(width())/float(height()), 0.1f, float(_cameraAngle.z*2));


        }
        // set view matrix for given camera coordinate system
         _view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
    }


    _mutex->unlock();


}

void OGLWidget::receive_save_request(QString qString)
{
	/**
	* Store the current widget state as an image file.
	*
	* @param[in] qString File name for image storage.
	*/

	grab().save(qString);
}

void OGLWidget::mousePressEvent(QMouseEvent* event)
{
	/**
	* Reimplementation of mousePressEvent. Generate position within lattice when click into widget area 
	* occured.
	*/

	if (event->type() == QEvent::MouseButtonPress)
	{
		if (event->button() == Qt::LeftButton)
		{
			_mousePosition = pixel_to_lattice_coordinates(event->pos());
			emit mouse_position_changed(_mousePosition);
		}

		if (event->button() == Qt::RightButton)
        {
            // Selection is only allowed without rotation or in orthogonal projection.
            if (_orthoView |(_cameraAngle.x==0 & _cameraAngle.y==0)){
                if (_rubberBand != NULL)
                {
                    _rubberBand->hide();
                }
                delete _rubberBand;
                _rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

                _rubberBandOrigin = event->pos();
                _rubberBand->setGeometry(QRect(event->pos(), QSize()));
                _rubberBand->show();
            }
            else{
                QMessageBox msgBox;
                msgBox.setText("Selection is only allowed without rotation or in orthogonal projection.\nPlease go to Camera-Menu. ");
                msgBox.exec();
            }
		}



	}
}

void OGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (_rubberBand!= NULL)
	{
		if (event->button() == Qt::RightButton)
        {
            Twodim pos1 = pixel_to_lattice_coordinates(_rubberBandOrigin);
            Twodim pos2 = pixel_to_lattice_coordinates(event->pos());

                if (pos1.x > pos2.x)
                {
                    std::swap(pos1.x, pos2.x);
                }

                if (pos1.y > pos2.y)
                {
                    std::swap(pos1.y, pos2.y);
                }
                std::vector<int> indexes;

                for (int i = 0; i < _numberAtoms; ++i)
                {
                    if (_latticeCoordArray[i].x < pos2.x && _latticeCoordArray[i].x > pos1.x
                        && _latticeCoordArray[i].y < pos2.y && _latticeCoordArray[i].y > pos1.y)
                    {
                        indexes.push_back(i);
                    }
                }
                if (_markedSpinsHandler != NULL)
                {
                    _markedSpinsHandler->process_spins(indexes);
                }
            }

        }
	}	


void OGLWidget::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

    double factor = (100. + numSteps) / 100.;
    _cameraAngle.z *=factor;
    _viewX *=factor;
    _viewY *=factor;

	event->accept();
    adjust_camera();
	repaint();
}

void OGLWidget::mouseMoveEvent(QMouseEvent * event)
{
    // move the camera relative to lattice center. in directions of camera system (left mouse button)
    if (event->buttons() & Qt::LeftButton & not QApplication::keyboardModifiers())
    {
		QPoint point = event->pos();
		Twodim mousePosition = pixel_to_lattice_coordinates(point);
        _viewOffset -= glm::float32(mousePosition.y - _mousePosition.y)*glm::normalize(_cameraUp) ;
        _viewOffset += glm::float32(mousePosition.x - _mousePosition.x)*glm::normalize(glm::cross(_cameraUp,_cameraFront));
        _mousePosition = mousePosition;
        adjust_camera();
        repaint();
    }
    //rotate camera around lattice (left mouse button + Ctrl)
    if (event->buttons() & Qt::LeftButton &  QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
    {
        QPoint point = event->pos();
        Twodim mousePosition = pixel_to_lattice_coordinates(point);
        _cameraAngle.x += glm::float32(mousePosition.x - _mousePosition.x)/_cameraAngle.z*50;
        _cameraAngle.y += glm::float32(mousePosition.y - _mousePosition.y)/_cameraAngle.z*50;
        _mousePosition = mousePosition;
        adjust_camera();
        repaint();
    }
    if (event->buttons() & Qt::RightButton )
    {
		_rubberBand->setGeometry(QRect(_rubberBandOrigin, event->pos()).normalized());
	}
}


Twodim OGLWidget::pixel_to_lattice_coordinates(QPoint point)
{
    /**
    * Translates pixel coordinates into "real space coordinates" according to lattice configuration.
    *
    * @param[in] point Pixels defining point in widget.
    * @return x,y coordinates of "real space" position
    */

    Twodim coordinates =  {0,0};
    if(_orthoView){
        coordinates.x = _cameraPos.x +(point.x()/float(width()) -0.5) *2*_viewX;
        coordinates.y = _cameraPos.y -(point.y()/float(height()) -0.5)*2*_viewY;
    }
    else{
        coordinates.x = _cameraPos.x +(point.x()/float(width()) -0.5) *2 * tan(M_PI/8)*_cameraAngle.z  *float(width())/float(height());
        coordinates.y = _cameraPos.y -(point.y()/float(height())-0.5) *2 * tan(M_PI/8)*_cameraAngle.z  ;

    }
    return coordinates;
}
