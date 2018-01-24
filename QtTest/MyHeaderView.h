#ifndef MYHEADERVIEW_H
#define MYHEADERVIEW_H

#include <QHeaderView>

class QPainter;

/// Custom QHeaderView for rich text
/** QHeaderView does not support richt text/ html text. This derived class realizes that.
*/
class MyHeaderView : public QHeaderView
{
Q_OBJECT

public:
	MyHeaderView(Qt::Orientation orientation, double leftMargin = 0.5, double topMargin = 0.2, 
		QWidget *parent = 0);
	~MyHeaderView();

protected:
	virtual void paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const;

	double _xMargin; ///< set margin to the left
	double _yMargin; ///< set margin to the top
};

#endif // MYHEADERVIEW_H
