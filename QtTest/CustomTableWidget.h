#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

// Qt includes
#include <qtablewidget.h>

#include <typedefs.h>

/// Table widget that allows for exact specification of column and row ratios.
class CustomTableWidget : public QTableWidget
{
	Q_OBJECT
public:
	CustomTableWidget(QWidget *parent = 0);
	~CustomTableWidget();

	void resizeEvent(QResizeEvent *event);

	void set_column_ratio(std::vector<int> columnRatio); ///< integer numbers defining column width ratios
	void set_row_ratio(std::vector<int> rowRatio); ///< integer numbers defining row height ratios
	void set_horizontal_header_height(int height); ///< integer for horizontal header height ratio 
	void set_vertical_header_width(int width); ///< integer for vertical header width ratio
	void custom_resize(void); ///< apply specified width and height ratios

private:
	std::vector<int> _columnRatio; ///< integer values for column width ratios
	std::vector<int> _rowRatio; ///< integer values for row height ratios
	int _height; ///< horizontal header height
	int _width; ///< vertical header width
};

#endif // CUSTOMTABLEWIDGET_H