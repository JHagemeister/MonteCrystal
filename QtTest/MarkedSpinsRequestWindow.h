#ifndef MARKEDSPINSREQUESTWINDOW_H
#define MARKEDSPINSREQUESTWINDOW_H

#include <QDialog>
#include "ui_markedspinsrequestwindow.h"

class MarkedSpinsHandler;

class MarkedSpinsRequestWindow : public QDialog
{
	Q_OBJECT

public:
	MarkedSpinsRequestWindow(MarkedSpinsHandler* markedSpinsHandler, QWidget *parent = Q_NULLPTR);
	~MarkedSpinsRequestWindow();

	void get_exchange_defect_parameters(int &order, double &parameter);
	void get_dm_defect_parameters(int & order, double & parameter);
	void get_anisotropy_defect_parameters(UniaxialAnisotropyStruct & anisotropy);

	void set_indexes(std::vector<int> &indexes);

	void clear_tables(void);

	public slots:
	void on_lock_button(void);
	void on_unlock_button(void);
	void on_table_direction(int row, int column);
	void on_button_show_defects(void);
	void on_button_show_locked_spins(void);
	void on_button_undo(void);

private:
	Ui::MarkedSpinsRequestWindow ui;

	MarkedSpinsHandler* _markedSpinsHandler;
};

#endif // MARKEDSPINSREQUESTWINDOW_H
