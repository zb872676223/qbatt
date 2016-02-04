#ifndef QBATTSTATS_H
#define QBATTSTATS_H

#include <QString>
#include "qbattpsu.h"
#include "qbattsysfsmethod.h"

class QBattStats
{
public:
	QBattStats();
	~QBattStats();

public:
	void		updatePowerSupplyInfo();
	int			getBatteryCapacity();
	QString		getBatteryCapacityLevel();
	int			getBatteryChargeFull();
	int			getBatteryChargeFullDesign();
	int			getBatteryChargeNow();
	int			getBatteryCurrentNow();
	int			getBatteryCycleCount();
	int			getBatteryEnergyFull();
	int			getBatteryEnergyFullDesign();
	int			getBatteryEnergyNow();
	QString		getBatteryManufacturer();
	QString		getBatteryModelName();
	int			getBatteryPowerNow();
	int			getBatteryPresent();
	QString		getBatterySerialNumber();
	QString		getBatteryStatus();
	QString		getBatteryTechnology();
	QString		getBatteryType();
	int			getBatteryVoltageMinDesign();
	int			getBatteryVoltageNow();
	int			getACOnline();
	QString		getACType();
	QString		getTimeLeft();

private:
	QBattSysFSMethod *sysfs_method;

private:
	psuinfo_t	psu;
};

#endif // QBATTSTATS_H
