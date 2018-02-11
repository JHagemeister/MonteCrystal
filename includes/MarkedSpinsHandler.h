#ifndef MARKEDSPINSHANDLER_H
#define MARKEDSPINSHANDLER_H

class MarkedSpinsRequestWindow;

#include <QSharedPointer>
#include <vector>
#include <unordered_map>

#include "typedefs.h"

#include <QObject>

class OGLWidget;
class SpinOrientation;
class MainWindow;

class MarkedSpinsHandler: public QObject
{
	Q_OBJECT
public:
	MarkedSpinsHandler(MainWindow* _mainWindow, OGLWidget* oglWidget);
	~MarkedSpinsHandler();

	void set_spin_orientation(QSharedPointer<SpinOrientation> spinOrientation, int boolNew);
	void process_spins(std::vector<int> indexes);

	void lock_spins(void);
	void unlock_spins(void);
	void set_direction(Threedim direction);

	void set_as_exchange_defects(int order, double parameter);
	void set_as_dm_defects(int order, double parameter);
	void set_as_anisotropy_defects(UniaxialAnisotropyStruct anisotropy);

	void clear_defects_and_unlock_all(void);

	void show_defects(void) const;
	void show_locked_spins(void) const;

	std::unordered_map<int, std::unordered_map<int, double>> get_exchange_defects(void);
	std::unordered_map<int, std::unordered_map<int, double>> get_dm_defects(void);
	std::unordered_map <int, UniaxialAnisotropyStruct> get_anisotropy_defects(void);

	public slots:
	void request_window_rejected(void);

private:
	QSharedPointer<SpinOrientation> _spinOrientation;
	MainWindow* _mainWindow;
	OGLWidget* _oglWidget;
	
	MarkedSpinsRequestWindow* _requestWindow;
	std::vector<int> _tempIndexes;

	std::unordered_map<int, std::unordered_map<int, double>> _exchangeDefects;
	std::unordered_map<int, std::unordered_map<int, double>> _dmDefects;
	std::unordered_map <int, UniaxialAnisotropyStruct> _anisotropyDefects;
};

#endif // MARKEDSPINSHANDLER_H