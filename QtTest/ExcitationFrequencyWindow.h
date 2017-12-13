#ifndef EXCITATIONFREQUENCYWINDOW_H_
#define EXCITATIONFREQUENCYWINDOW_H_

#include <QDialog>
#include "ui_excitationfrequencywindow.h"

#include <gsl/gsl_matrix.h>

class OGLWidget;
class Configuration;

#include <qthread.h>

#include "ExcitationVisualization.h"

class ExcitationFrequencyWindow : public QDialog 
{
Q_OBJECT

public:

	ExcitationFrequencyWindow(OGLWidget* oglWidget, std::string workfolder, QWidget * parent = Q_NULLPTR);
	~ExcitationFrequencyWindow();

	void set_spin_array(Threedim* spinArray, int numberAtoms);

	void start_visualization(void);

	void read_parameters(QSharedPointer<Configuration> &config);

	public slots:
	void on_button_eigenvalues(void);
	void on_button_eigenvectors(void);
	void on_button_next(void);
	void on_button_previous(void);
	void on_button_pause(void);
	void on_button_save_image_sequence(void);
	void on_change_in_table_widget_angle(int row, int column);
	void on_change_in_table_widget_speed(int row, int column);
	void on_change_in_table_widget_segmentation(int row, int column);
	void receive_save_image_request(int imageIndex);
	void receive_points(std::vector<int> points);
	void receive_video_finished(void);
	void stop_visualization(void);

private:
	void setup_elements(void);
	void update_GUI(void);

	Ui::ExcitationFrequencyWindow _ui;

	std::string _workfolder; ///< workfolder of main program as start folder for folder dialogues

	ExcitationFrequencyParameters _parameters;

	QString _outputFolder; ///< this is determined by object of this class when eigenvalues are read in.
	QString _currentSubfolder; ///< unique subfolder in _outputFolder for output of video sequence
	int _outputIndex; ///< number of video sequences done. needed to create unique output folders.

	OGLWidget* _oglWidget;

	Threedim* _spinArray;
	int _numberAtoms;
	Threedim* _spinArrayBackup;

	std::vector<double> _evalues;
	gsl_matrix_complex* _evecs;

	QThread* _visualizationThread;
};

#endif // EXCITATIONFREQUENCYWINDOW_H_
