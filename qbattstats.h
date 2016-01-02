#ifndef QBATTSTATS_H
#define QBATTSTATS_H

#include <QString>

class QBattStats
{
	public:
		QBattStats();

	public:
		bool checkDirExists();
		QString getCapacity();
		QString getCapacityLevel();
		QString getChargeFull();
		QString getChargeFullDesign();
		QString getChargeNow();
		QString getCycleCount();
		QString getManufacturer();
		QString getModelName();
		QString getPresent();
		QString getSerialNumber();
		QString getStatus();
		QString getTechnology();
		QString getType();
		QString getVoltageMinDesign();
		QString getVoltageNow();

	private:
		QString getFileContents(QString f);
};

#endif // QBATTSTATS_H
