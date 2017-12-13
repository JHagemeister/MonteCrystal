/*
* GUIProgramTypeElements.h
*
*      
*/

#ifndef GUIPROGRAMTYPEELEMENT_H
#define GUIPROGRAMTYPEELEMENT_H

// Form include
#include "ui_mainWindow.h"

#include <memory>

#include "qobject.h"
#include "qsharedpointer.h"

class Configuration;

/// Management of GUI element concerning program type

class GUIProgramTypeElement : public QObject
{
	Q_OBJECT
public:
	GUIProgramTypeElement(Ui::QtMainWindow* ui);

	void setup_elements(void); ///< setup energy elements
	void read_parameter(QSharedPointer<Configuration> &config); ///< read selected program type from GUI

protected:
	Ui::QtMainWindow* _ui; ///< pointer to object with ui elements
};

#endif // GUIPROGRAMTYPEELEMENTS_H


