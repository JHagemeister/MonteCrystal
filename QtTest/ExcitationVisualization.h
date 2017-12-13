#ifndef EXCITATIONVISUALIZATION_H_
#define EXCITATIONVISUALIZATION_H_

#include <qobject.h>

#include <vector>

#include <gsl/gsl_matrix.h>

#include "typedefs.h"

class ExcitationVisualization : public QObject
{
Q_OBJECT
public:
	ExcitationVisualization(Threedim* spinArray, Threedim* _spinArrayBackup, int numberAtoms, 
		std::vector<double>* evalues, gsl_matrix_complex* evec, ExcitationFrequencyParameters parameters);
	~ExcitationVisualization();

	public slots:
	void visualize();

signals:
	void send_repaint_request();
	void send_save_image_request(int imageIndex);
	void send_video_sequence_finished();
	void send_points(std::vector<int> points);
	void send_finished();

private:
	void update(void);
	void segmentation(void);

	Threedim* _spinArray;
	Threedim* _spinArrayBackup;
	int _numberAtoms;
	ExcitationFrequencyParameters _parameters;

	std::vector<double>* _evalues;
	gsl_matrix_complex* _evec;

	double _eval_i;
	double _eval_ip1;
	gsl_vector_complex_view _evec_i;
	gsl_vector_complex_view _evec_ip1;
	double _scalingValue;
};

#endif // EXCITATIONVISUALIZATION_H_