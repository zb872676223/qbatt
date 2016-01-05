#include <QFile>
#include <QDir>
#include <QDebug>
#include "qbattstats.h"

#define DEF_BATT_CAPACITY			"capacity"
#define DEF_BATT_CAPACITY_LEVEL		"capacity_level"
#define DEF_BATT_CHARGE_FULL		"charge_full"
#define DEF_BATT_CHARGE_FULL_DESIGN	"charge_full_design"
#define DEF_BATT_CHARGE_NOW			"charge_now"
#define DEF_BATT_CURRENT_NOW		"current_now"
#define DEF_BATT_CYCLE_COUNT		"cycle_count"
#define DEF_BATT_MANUFACTURER		"manufacturer"
#define DEF_BATT_MODEL_NAME			"model_name"
#define DEF_BATT_PRESENT			"present"
#define DEF_BATT_SERIAL_NUMBER		"serial_number"
#define DEF_BATT_STATUS				"status"
#define DEF_BATT_TECHNOLOGY			"technology"
#define DEF_BATT_TYPE				"type"
#define DEF_BATT_VOLTAGE_MIN_DESIGN	"voltage_min_design"
#define DEF_BATT_VOLTAGE_NOW		"voltage_now"

#define DEF_ACAD_ONLINE				"online"
#define DEF_ACAD_TYPE				"type"

#define DEF_SYS_PATH_BATT1			"/sys/class/power_supply/BAT1"
#define DEF_SYS_PATH_ACAD			"/sys/class/power_supply/ACAD"
#define RESPONSE_N_A				"n/a"

QBattStats::QBattStats()
{

}

bool QBattStats::checkDirExists(QString path)
{
	return QDir(path).exists();
}

QString QBattStats::getFileContents(QString fpath, QString fname)
{
	QString path;
	QString result;

	if (!checkDirExists(fpath))
		return RESPONSE_N_A;

	path.append(fpath);
	path.append('/');
	path.append(fname);

	QFile file(path);

	if (file.open(QIODevice::ReadOnly)) {
		char buf[256];
		qint64 lineLength = file.readLine(buf, sizeof(buf));
		if (lineLength != -1) {
			result = QString(buf);

			file.close();
			return result;
		}
	}

	return RESPONSE_N_A;
}

QString QBattStats::getStats(tPSUStat type)
{
	switch (type) {
		case BATT_CAPACITY:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_CAPACITY));
			break;
		case BATT_CAPACITY_LEVEL:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_CAPACITY_LEVEL));
			break;
		case BATT_CHARGE_FULL:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_CHARGE_FULL));
			break;
		case BATT_CHARGE_FULL_DESIGN:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_CHARGE_FULL_DESIGN));
			break;
		case BATT_CHARGE_NOW:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_CHARGE_NOW));
			break;
		case BATT_CURRENT_NOW:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_CURRENT_NOW));
			break;
		case BATT_CYCLE_COUNT:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_CYCLE_COUNT));
			break;
		case BATT_MANUFACTURER:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_MANUFACTURER));
			break;
		case BATT_MODEL_NAME:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_MODEL_NAME));
			break;
		case BATT_PRESENT:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_PRESENT));
			break;
		case BATT_SERIAL_NUMBER:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_SERIAL_NUMBER));
			break;
		case BATT_STATUS:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_STATUS));
			break;
		case BATT_TECHNOLOGY:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_TECHNOLOGY));
			break;
		case BATT_TYPE:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_TYPE));
			break;
		case BATT_VOLTAGE_MIN_DESIGN:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_VOLTAGE_MIN_DESIGN));
			break;
		case BATT_VOLTAGE_NOW:
			return (getFileContents(DEF_SYS_PATH_BATT1,
					DEF_BATT_VOLTAGE_NOW));
			break;
		case ACAD_ONLINE:
			return (getFileContents(DEF_SYS_PATH_ACAD,
					DEF_ACAD_ONLINE));
			break;
		case ACAD_TYPE:
			return (getFileContents(DEF_SYS_PATH_ACAD,
					DEF_ACAD_TYPE));
			break;
		default:
			return RESPONSE_N_A;
			break;
	}
}

QString QBattStats::getTimeLeft()
{
	int hours = -1;
	int minutes = -1;
	int seconds = -1;
	int remainingCapacity = -1;
	int lastCapacity = -1;
	int current = -1;

	QString ret;

	current = getStats(BATT_CURRENT_NOW).toInt() / 1000;
	remainingCapacity = getStats(BATT_CHARGE_NOW).toInt() / 1000;
	lastCapacity = getStats(BATT_CHARGE_FULL).toInt() / 1000;

	/*
	 * This idea has been taken from acpi utility by
	 * Grahame Bowland <grahame@angrygoats.net> and
	 * Michael Meskes  <meskes@debian.org>
	 */
	if (current > 0) {
		if (!QString().compare(getStats(BATT_STATUS).trimmed(),
								BATT_STATUS_CHARGING))
			seconds = 3600 * (lastCapacity - remainingCapacity) / current;
		else if (!QString().compare(getStats(BATT_STATUS).trimmed(),
								BATT_STATUS_DISCHARGING))
			seconds = 3600 * remainingCapacity / current;
	}

	ret.clear();

	if (seconds > 0) {
		hours = seconds / 3600;
		seconds -= 3600 * hours;
		minutes = seconds / 60;
		seconds -= 60 * minutes;

		ret.sprintf("%02d:%02d h", hours, minutes);
	} else {
		ret.append("n/a");
	}

	return ret;
}
