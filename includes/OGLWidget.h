/*
* OGLWidget.h
*
*  Created on: x.2017
*      
*/


#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <vector>
#include <unordered_set>
#include <memory>

// Qt includes
#include <QWidget>
#include <QEvent>
#include <QOpenGLWidget>
#include <QMutex>
#include <QSharedPointer>
#include <QRubberBand>
#include <QOpenGLFunctions_3_3_Core>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

// own
#include "Shader.h"
#include "Mesh.h"
#include "SpinMesh.h"

#include <typedefs.h>

class Hamiltonian;
class ColorMapper;
class MarkedSpinsHandler;
class Lattice;

/// OpenGL widget for graphical output of spin configurations
/** The graphical output is done with openGL to which an excellent tutorial is given by Joey de Vries:
https://learnopengl.com/
The programming regarding the openGL part of this program is based on code from the tutorial
Twitter:
https://twitter.com/JoeyDeVriez
License:
https://creativecommons.org/licenses/by-nc/4.0/
*/

class OGLWidget : public QOpenGLWidget
{
	Q_OBJECT
public:

	QMutex* _mutex; ///< prevent simultaneous access to spin configuration from GUI and simulation thread

	std::vector<int> _points;
	SpinMeshParams _spinMeshParams;

	OGLWidget(QWidget *parent = 0);
	~OGLWidget();

	void set_marked_spins_handler(MarkedSpinsHandler* markedSpinsHandler);
	void delete_rubber_band(void);

	void set_lattice(QSharedPointer<Lattice> lattice);

	void set_hamiltonian(QSharedPointer<Hamiltonian> hamiltonian); ///< set Hamiltonian, needed for color maps

	void set_bool_display_spins(int boolSpins); ///< enable or disable graphical output of spins
	void set_spin_color(glm::vec4 color); ///< set a single spin color
	void set_spin_color_rgb_scale(void); ///< spins will be shown in a rgb scale
	void set_spin_model(SpinMeshParams params);


	void set_wire_frame_spins(int* indexes, int size);
	void set_wire_frame_spins(std::vector<int> &indexes);
	void set_spins_filled(void);
	void set_filled_spins(int* indexes, int size);
	void set_filled_spins(std::vector<int> &indexes);
	void set_visible_spins_order(int order);
	int get_visible_spins_order(void);
	void set_all_spins_visible(void);
	void set_all_spins_invisible(void);
	void set_spin_visible(int index);
	void set_spin_invisible(int index);
	void set_spins(Threedim* spinArray, int size, int boolNew);
	

	void set_background_color(glm::vec4 color);
	glm::vec4 get_background_color(void);

	void set_color_map_type(ColorMapType colorMapType, std::string identifier); ///< Specify which kind of color map shall be shown
	void set_color_map_min_max(double min, double max);///< set min max values of displayed color map property
	void set_color_map_min_max_later(void); ///< set min max values at begin of simulation
	void set_color_map_colors(std::vector<glm::vec4> colorMapColors); ///< set colors for color map
	void set_custom_magnetization_direction_color_map(Threedim direction); ///< set direction for magnetization contrast
	
	int get_bool_display_spins(void) const; ///< get boolean about whether spins are shown or not
	glm::vec4 get_spin_color(void) const; ///< get single spin color
	int get_bool_spin_single_color(void) const; ///< get boolean about whether spins are shown in single color
	void get_color_map_type(ColorMapType &colorMapType, std::string &colorMapID) const; ///< get information about which property is displayed
	void get_color_map_min_max(double& min, double& max) const; ///< get min max values of color map property
	std::vector<glm::vec4> get_color_map_colors(void); ///< get colors of color map
	Threedim get_custom_magnetization_direction_color_map(void); ///< get direction for magnetization contrast

	/// get min max values for specified color map property
	void get_current_min_max(ColorMapType colorMapType, std::string identifier, double & min, double & max);

	void update_color_scale(void);

	public slots:
	void adjust_camera_to_lattice(void); ///< adjust camera so that current lattice is fully displayed
	void receive_save_request(QString qString); ///< save current spin configuration to .png file

signals:
	void mouse_position_changed(Twodim position);
	void color_map_text(QString qString);

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void paint_spins(void);
	void paint_single_spin(int index);
	void paint_color_map_lattice_site_centered(void);
	void paint_color_map_topological_charge(void);
	void paint_points(void);

	void scale_map_value(double value, glm::vec4 & color);
	int search_single_energy(std::string identifier);
	std::vector<int> search_anisotropy_energies(void);

	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mouseMoveEvent(QMouseEvent * event);

	double OGLWidget::get_minimal_distance_to_lattice_site(Threedim vector, int &siteIndex);

	Twodim pixel_to_lattice_coordinates(QPoint point);

	MarkedSpinsHandler* _markedSpinsHandler;

	Shader _shader;
	std::shared_ptr<SpinMesh> _spinMesh;
	std::string _spinMeshParamsFname;

	QSharedPointer<Lattice> _lattice;
	Threedim* _latticeCoordArray; ///< lattice coordinates; redundant with _lattice
	int _numberAtoms; ///< number of lattice sites
	std::vector<Threedim> _wignerSeitzCell;
	TopologicalChargeCell* _skNcells;
	int _skNcellNum;
	std::vector<Threedim> _dummyskNTriangles;

	Threedim* _spinArray; ///< spin configuration; shared with simulation thread
	std::unordered_set<int> _wireFrameSpins;
	std::unordered_set<int> _visibleWireFrameSpins;
	std::unordered_set<int> _filledSpins;
	std::unordered_set<int> _visibleFilledSpins;

	int _showSpins;
	int _spinsSingleColor;
	glm::vec4 _spinColor;
	glm::vec3 _spinScaleFactor;
	int _visibleSpinOrder;

	glm::vec4 _backgroundColor;

	ColorMapType _colorMapType;
	int _energyIndex;
	std::vector<int> _anisotropyIndexes;
	std::vector<glm::vec4> _colorMapColors;
	std::string _colorMapID;
	Threedim _customMagnetizationDirection;
	double _minValueMap;
	double _maxValueMap;
	int _boolSetMinMaxLater;
	Twodim _redScaleMap;
	Twodim _greenScaleMap;
	Twodim _blueScaleMap;
	double _colorMapOffset;

	QSharedPointer<Hamiltonian> _hamiltonian;

	// Camera
	glm::vec3 _cameraPos = glm::vec3(17.0f, 3.5f, 30.0f); ///< position of camera
	glm::vec3 _cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); ///< camera orientation
	glm::vec3 _cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); ///< camera orientation

	float _viewX = 5; ///< view frame in x-direction; set by adjust_camera_to_lattice()
	float _viewY = 20; ///< view frame in y-direction; set by adjust_camera_to_lattice()

	Twodim _mousePosition = { 0,0 };
	QRubberBand* _rubberBand;
	QPoint _rubberBandOrigin;

	//Light
	glm::vec3 _lightPos = glm::vec3(0.0f, 0.0f, 1000.0f); ///< light source

	QOpenGLFunctions_3_3_Core* _glf;
};

#endif // OGLWIDGET_H