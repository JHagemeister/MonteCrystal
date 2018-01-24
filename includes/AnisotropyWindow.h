#ifndef ANISOTROPYWINDOW_H
#define ANISOTROPYWINDOW_H

//standard includes
#include <memory>

// Qt includes
#include <QDialog>
#include <QStringList>
#include <QStringListModel>

// Form include
#include "ui_anisotropy.h"

// own
#include "typedefs.h"

/// GUI window to specify anisotropy energies
class AnisotropyWindow : public QDialog
{
Q_OBJECT

public:
	AnisotropyWindow(std::shared_ptr<std::vector<double>> hexAnisotropies,
		std::shared_ptr<std::vector<UniaxialAnisotropyStruct>> anisotropies, QWidget *parent = 0);
	~AnisotropyWindow();

	public slots:
	void on_add_clicked(); ///< add anisotropy energy
	void on_delete_clicked(); ///< remove selected anisotropy energy
	void on_ok_clicked(); ///< read parameters and close window

protected:
	Ui::Dialog _ui; ///< ui elements
	QStringListModel* _model;
	QStringList* _list;

	std::shared_ptr<std::vector<double>> _hexAnisotropies; ///< should contain three elements
	std::shared_ptr<std::vector<UniaxialAnisotropyStruct>> _anisotropies; ///< anisotropy energies	
};

#endif // ANISOTROPYWINDOW_H
