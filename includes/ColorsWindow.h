/*
* ColorsWindow.h
*
* 
*
*/

#ifndef COLORSWINDOW_H
#define COLORSWINDOW_H

#include <QDialog>
#include "ui_colors.h"

class QCheckBox;
class OGLWidget;

#include <glm/vec4.hpp>


/// Specification of parameters for spins and color maps. 
/**
* This class implements a window that allows the user to change the representation of the spins and choose
* color maps.
*/
class ColorsWindow : public QDialog {
	Q_OBJECT

public:
	ColorsWindow(OGLWidget* oglWidget, QWidget * parent = Q_NULLPTR);
	~ColorsWindow();

	void error_box(void);

	public slots:
	void change_in_background_color_table_widget(int row, int column);
	void change_in_color_map_table_widget(int row, int column);
	void change_in_spin_color_table_widget(int row, int column);
	
	void change_in_magnetization_table_theta_color_map(int row, int column);
	void change_in_magnetization_table_phi_color_map(int row, int column);
	void change_in_spin_model_table(int row, int column);

	void check_state_changed_checkBoxSpinVectors(int state);
	void check_state_changed_checkBoxSpinVectorSingleColor(int state);
	void check_state_changed_checkBoxSpinVectorRGB(int state);

	void pushButton_add_spins(void);
	void pushButton_remove_spins(void);
	
	// react to changes in check boxes for color map
	void check_state_changed_checkBox_E(int state);
	void check_state_changed_checkBox_J1(int state);
	void check_state_changed_checkBox_J2(int state);
	void check_state_changed_checkBox_J3(int state);
	void check_state_changed_checkBox_J4(int state);
	void check_state_changed_checkBox_J5(int state);
	void check_state_changed_checkBox_J6(int state);
	void check_state_changed_checkBox_J7(int state);
	void check_state_changed_checkBox_J8(int state);
	void check_state_changed_checkBox_D1(int state);
	void check_state_changed_checkBox_D2(int state);
	void check_state_changed_checkBox_D3(int state);
	void check_state_changed_checkBox_D4(int state);
	void check_state_changed_checkBox_D5(int state);
	void check_state_changed_checkBox_Dipol(int state);
	void check_state_changed_checkBox_Anisotropy(int state);
	void check_state_changed_checkBox_BiQ(int state);
	void check_state_changed_checkBox_4Spin(int state);
	void check_state_changed_checkBox_Zeeman(int state);
	void check_state_changed_checkBox_M(int state);
	void check_state_changed_checkBox_Skn(int state);

	void apply_button(void); ///< apply user specifications to widget
	void apply_and_close_button(void); ///< apply user specifications and close window

signals:
	void send_repaint_request(void);
	

private:
	Ui::Colors _ui;

	OGLWidget* _oglWidget; ///< oglWidget object 

	/** 4 colors for color map scaling; first and last color are smaller min and larger max colors. Two other
	 colors are used for scaling values between min, max which are specified in _oglWidget*/
	std::vector<glm::vec4> _colorMapColors; 
	ColorMapType _colorMapType;
	std::string _colorMapIdentifier;
	glm::vec4 _spinColor;

	int _visibleSpinOrder;

	// color map check boxes
	QCheckBox* _checkBox_E;
	QCheckBox* _checkBox_J1;
	QCheckBox* _checkBox_J2;
	QCheckBox* _checkBox_J3;
	QCheckBox* _checkBox_J4;
	QCheckBox* _checkBox_J5;
	QCheckBox* _checkBox_J6;
	QCheckBox* _checkBox_J7;
	QCheckBox* _checkBox_J8;
	QCheckBox* _checkBox_D1;
	QCheckBox* _checkBox_D2;
	QCheckBox* _checkBox_D3;
	QCheckBox* _checkBox_D4;
	QCheckBox* _checkBox_D5;
	QCheckBox* _checkBox_Dipol;
	QCheckBox* _checkBox_Anisotropy;
	QCheckBox* _checkBox_BiQ;
	QCheckBox* _checkBox_4Spin;
	QCheckBox* _checkBox_Zeeman;
	QCheckBox* _checkBox_M;
	QCheckBox* _checkBox_Skn;

	int _boolWorkingColorMapCheckBoxes;
	int _boolWorkingCheckBoxes;

	void set_values(void);

	void set_spin_model_params(SpinMeshParams params);

	void set_current_min_max(QString min, QString max);

	void set_selected_min_max(QString min, QString max);

	void uncheck_all_color_map_check_boxes(void);
	void set_magnetization_table_color_map(QString theta, QString phi);

	void setup_min_max_tables(void);

	void setup_color_map_checkBoxes(void);
	void setup_magnetization_tables_color_map(void);

	void setup_background_color(void);

	void setup_spin_color(void);
	void read_spin_color(void);
	void setup_spin_model(void);
	void update_display_color_table_widget_spin_color(void);

	void setup_color_map(void);
	void color_map_gray_scale(void);
	void set_color_map_colors(std::vector<glm::vec4> colorMapColors);
	void read_color_map_colors(void);
	void update_display_colors_table_widget_color_map(void);
};

#endif //COLORSWINDOW_H
