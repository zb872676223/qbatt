#ifndef QBATTSTATS_H
#define QBATTSTATS_H

#include <QString>

class QBattStats
{
	public:
		QBattStats();

	public:
		typedef enum {
			// Battery
			BATT_CAPACITY,
			BATT_CAPACITY_LEVEL,
			BATT_CHARGE_FULL,
			BATT_CHARGE_FULL_DESIGN,
			BATT_CHARGE_NOW,
			BATT_CURRENT_NOW,
			BATT_CYCLE_COUNT,
			BATT_MANUFACTURER,
			BATT_MODEL_NAME,
			BATT_PRESENT,
			BATT_SERIAL_NUMBER,
			BATT_STATUS,
			BATT_TECHNOLOGY,
			BATT_TYPE,
			BATT_VOLTAGE_MIN_DESIGN,
			BATT_VOLTAGE_NOW,
			// ACAD
			ACAD_ONLINE,
			ACAD_TYPE
		} tPSUStat;

	public:
		QString getStats(tPSUStat type);
		QString getTimeLeft();

	private:
		bool checkDirExists(QString path);
		QString getFileContents(QString fpath, QString fname);
};

static const QString BATT_STATUS_UNKNOWN		= "Unknown";
static const QString BATT_STATUS_DISCHARGING	= "Discharging";
static const QString BATT_STATUS_CHARGING		= "Charging";

#endif // QBATTSTATS_H
