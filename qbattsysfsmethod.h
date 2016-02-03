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
	bool checkFileExists(QString fpath, QString fname);
	QString getFileContents(QString fpath, QString fname);
	bool searchSysFs_Battery();
	bool searchSysFs_AC();
	// AC adapter
	int getACADOnline();
	QString getACADName();
	// Battery
	QString getPSUStatus();
	int getPSUPresent();
	QString getPSUTechnology();
	QString getPSUType();
	int getPSUCycleCount();
	int getPSUVoltageMinDesign();
	int getPSUVoltageNow();
	int getPSUCurrentNow();
	int getPSUChargeFullDesign();
	int getPSUChargeFull();
	int getPSUChargeNow();
	int getPSUCapacity();
	int getPSUEnergyFull();
	int getPSUEnergyFullDesign();
	int getPSUEnergyNow();
	int getPSUPowerNow();
	QString getPSUCapacityLevel();
	QString getPSUModelName();
	QString getPSUManufacturer();
	QString getPSUSerialNumber();

private:
	psuinfo_t *psu;
};

#endif // QBATTSYSFSMETHOD_H
