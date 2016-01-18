#ifndef QBATTSYSFSMETHOD_H
#define QBATTSYSFSMETHOD_H

#include <QString>
#include "qbattpsu.h"

class QBattSysFSMethod
{
public:
	QBattSysFSMethod(psuinfo_t *psu);
	void initPowerSupply();
	void updatePowerSupply();

private:
	bool checkDirExists(QString path);
	QString getFileContents(QString fpath, QString fname);
	bool searchSysFs_Battery();
	bool searchSysFs_AC();

private:
	psuinfo_t *psu;
};

#endif // QBATTSYSFSMETHOD_H
