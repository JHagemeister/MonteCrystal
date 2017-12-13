
#ifndef WORKFOLDER_H
#define WORKFOLDER_H

#include <QDialog>
#include "ui_workfolder.h"

/// Window to display and specify workfolder.
class WorkfolderWindow : public QDialog {
	Q_OBJECT

public:
	WorkfolderWindow(QWidget * parent = Q_NULLPTR);
	~WorkfolderWindow();

	void set_text_edit(QString qString);

	public slots:
	void on_push_button_select_other(void);

signals:
	void send_select_other(void);

private:
	Ui::Workfolder ui;
};

#endif // WORKFOLDER_H
