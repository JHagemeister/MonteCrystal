/*
* GUIProgramTypeElements.h
*
*      
*/

#ifndef GUIPROGRAMTYPEELEMENT_H
#define GUIPROGRAMTYPEELEMENT_H

#include <QObject>
#include <QSharedPointer>

class Configuration;
class MainWindow;

/// Management of GUI element concerning program type

class GUIProgramTypeElement : public QObject
{
	Q_OBJECT
public:
	GUIProgramTypeElement(MainWindow* mw);

	void setup_elements(void); ///< setup energy elements
	void read_parameter(QSharedPointer<Configuration> &config); ///< read selected program type from GUI

protected:
	MainWindow* _mw; ///< pointer to object with ui elements
};

#endif // GUIPROGRAMTYPEELEMENTS_H


