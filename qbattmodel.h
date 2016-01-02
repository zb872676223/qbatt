#ifndef QBATTMODEL_H
#define QBATTMODEL_H

#include <QTableWidget>
#include <QTableWidgetItem>

typedef enum _rowList {
	ROW_CAPACITY = 0,
	ROW_CAPACITY_LEVEL,
	ROW_CHARGE_FULL,
	ROW_CHARGE_FULL_DESIGN,
	ROW_CHARGE_NOW,
	ROW_CURRENT_NOW,
	ROW_CYCLE_COUNT,
	ROW_MANUFACTURER,
	ROW_MODEL_NAME,
	ROW_PRESENT,
	ROW_SERIAL_NUMBER,
	ROW_STATUS,
	ROW_TECHNOLOGY,
	ROW_TYPE,
	ROW_VOLTAGE_MIN_DESIGN,
	ROW_VOLTAGE_NOW
} tRowListItem;

typedef enum _colList {
	COL_TYPE = 0,
	COL_VALUE
} tColListItem;

class QBattModel
{
	public:
		QBattModel(QTableWidget *wdgt);
		void setValue(tRowListItem item, QString value);

	private:
		QTableWidget *wdgt;
};

#endif // QBATTMODEL_H
