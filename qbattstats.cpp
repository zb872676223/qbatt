#include <QFile>
#include <QDir>
#include <QDebug>
#include "qbattstats.h"

#define DEF_CAPACITY			"capacity"
#define DEF_CAPACITY_LEVEL		"capacity_level"
#define DEF_CHARGE_FULL			"charge_full"
#define DEF_CHARGE_FULL_DESIGN	"charge_full_design"
#define DEF_CHARGE_NOW			"charge_now"
#define DEF_CYCLE_COUNT			"cycle_count"
#define DEF_MANUFACTURER		"manufacturer"
#define DEF_MODEL_NAME			"model_name"
#define DEF_PRESENT				"present"
#define DEF_SERIAL_NUMBER		"serial_number"
#define DEF_STATUS				"status"
#define DEF_TECHNOLOGY			"technology"
#define DEF_TYPE				"type"
#define DEF_VOLTAGE_MIN_DESIGN	"voltage_min_design"
#define DEF_VOLTAGE_NOW			"voltage_now"

#define DEF_SYS_PATH			"/sys/class/power_supply/BAT1"
#define RESPONSE_N_A			"n/a"

QBattStats::QBattStats()
{

}

bool QBattStats::checkDirExists()
{
	return QDir(DEF_SYS_PATH).exists();
}

QString QBattStats::getFileContents(QString f)
{
	QString path;
	QString result;

	path.append(DEF_SYS_PATH);
	path.append('/');
	path.append(f);

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

QString QBattStats::getCapacity()
{
	return (getFileContents(DEF_CAPACITY));
}

QString QBattStats::getCapacityLevel()
{
	return (getFileContents(DEF_CAPACITY_LEVEL));
}

QString QBattStats::getChargeFull()
{
	return (getFileContents(DEF_CHARGE_FULL));
}

QString QBattStats::getChargeFullDesign()
{
	return (getFileContents(DEF_CHARGE_FULL_DESIGN));
}

QString QBattStats::getChargeNow()
{
	return (getFileContents(DEF_CHARGE_NOW));
}

QString QBattStats::getCycleCount()
{
	return (getFileContents(DEF_CYCLE_COUNT));
}

QString QBattStats::getManufacturer()
{
	return (getFileContents(DEF_MANUFACTURER));
}

QString QBattStats::getModelName()
{
	return (getFileContents(DEF_MODEL_NAME));
}

QString QBattStats::getPresent()
{
	return (getFileContents(DEF_PRESENT));
}

QString QBattStats::getSerialNumber()
{
	return (getFileContents(DEF_SERIAL_NUMBER));
}

QString QBattStats::getStatus()
{
	return (getFileContents(DEF_STATUS));
}

QString QBattStats::getTechnology()
{
	return (getFileContents(DEF_TECHNOLOGY));
}

QString QBattStats::getType()
{
	return (getFileContents(DEF_TYPE));
}

QString QBattStats::getVoltageMinDesign()
{
	return (getFileContents(DEF_VOLTAGE_MIN_DESIGN));
}

QString QBattStats::getVoltageNow()
{
	return (getFileContents(DEF_VOLTAGE_NOW));
}
