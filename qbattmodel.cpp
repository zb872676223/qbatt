#include "qbattmodel.h"

QBattModel::QBattModel(QTableWidget *wdgt)
{
	QStringList colHeader;
	QStringList rowHeader;
	this->wdgt = wdgt;

	// Fill the headers
	colHeader << "Value";
	rowHeader << "Capacity"
			<< "Capacity level"
			<< "Charge full"
			<< "Charge full (design)"
			<< "Charge now"
			<< "Current now"
			<< "Cycle count"
			<< "Manufacturer"
			<< "Model name"
			<< "Present"
			<< "Serial number"
			<< "Status"
			<< "Technology"
			<< "Type"
			<< "Voltage min (design)"
			<< "Voltage now";

	wdgt->setRowCount(rowHeader.count());
	wdgt->setColumnCount(colHeader.count());
	wdgt->setHorizontalHeaderLabels(colHeader);
	wdgt->setVerticalHeaderLabels(rowHeader);
	wdgt->setShowGrid(true);
	wdgt->resizeRowsToContents();

	wdgt->show();
}

void QBattModel::setValue(tRowListItem item, QString value)
{
	wdgt->setItem(item - 1, 1, new QTableWidgetItem(value));
}
