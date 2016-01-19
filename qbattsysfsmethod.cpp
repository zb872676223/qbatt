#include <QDebug>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include "qbattpsu.h"
#include "qbattsysfsmethod.h"

static const QString FILE_BATT_CAPACITY				= "capacity";
static const QString FILE_BATT_CAPACITY_LEVEL		= "capacity_level";
static const QString FILE_BATT_CHARGE_FULL			= "charge_full";
static const QString FILE_BATT_CHARGE_FULL_DESIGN	= "charge_full_design";
static const QString FILE_BATT_CHARGE_NOW			= "charge_now";
static const QString FILE_BATT_CURRENT_NOW			= "current_now";
static const QString FILE_BATT_CYCLE_COUNT			= "cycle_count";
static const QString FILE_BATT_MANUFACTURER			= "manufacturer";
static const QString FILE_BATT_MODEL_NAME			= "model_name";
static const QString FILE_BATT_PRESENT				= "present";
static const QString FILE_BATT_SERIAL_NUMBER		= "serial_number";
static const QString FILE_BATT_STATUS				= "status";
static const QString FILE_BATT_TECHNOLOGY			= "technology";
static const QString FILE_BATT_VOLTAGE_MIN_DESIGN	= "voltage_min_design";
static const QString FILE_BATT_VOLTAGE_NOW			= "voltage_now";
static const QString FILE_ACAD_ONLINE				= "online";

static const QString PSU_SYSTEM_PATH				= "/sys/class/power_supply";
static const QString PSU_SYSTEM_PATH_NONE			= "none";
static const QString PSU_FILE_TYPE					= "type";

static const QString DEF_ACAD_TYPE					= "Mains";
static const QString DEF_BATTERY_TYPE				= "Battery";

static const QString RESPONSE_N_A					= "n/a";

QBattSysFSMethod::QBattSysFSMethod(psuinfo_t *psu)
{
	this->psu = psu;
}

bool QBattSysFSMethod::checkDirExists(QString path)
{
	return QDir(path).exists();
}

QString QBattSysFSMethod::getFileContents(QString fpath, QString fname)
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

			return result.trimmed();
		}
	}

	return RESPONSE_N_A;
}

bool QBattSysFSMethod::searchSysFs_Battery()
{
	QDir searchDir(PSU_SYSTEM_PATH);
	QString path;

	if (!checkDirExists(PSU_SYSTEM_PATH))
		return false;

	searchDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QDirIterator qdi(searchDir, QDirIterator::Subdirectories);

	while (qdi.hasNext()) {
		path = qdi.next();
		if (!(QString::compare(getFileContents(path, PSU_FILE_TYPE),
							   DEF_BATTERY_TYPE, Qt::CaseSensitive))) {
			psu->battery.path = path;
			return true;
		}
	}

	// Initialize path with "none"
	psu->battery.path = PSU_SYSTEM_PATH_NONE;

	return false;
}

bool QBattSysFSMethod::searchSysFs_AC()
{
	QDir searchDir(PSU_SYSTEM_PATH);
	QString path;

	if (!checkDirExists(PSU_SYSTEM_PATH))
		return false;

	searchDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QDirIterator qdi(searchDir, QDirIterator::Subdirectories);

	while (qdi.hasNext()) {
		path = qdi.next();
		if (!(QString::compare(getFileContents(path, PSU_FILE_TYPE),
							   DEF_ACAD_TYPE, Qt::CaseSensitive))) {
			psu->ac_adapter.path = path;
			return true;
		}
	}

	// Initialize path with "none"
	psu->ac_adapter.path = PSU_SYSTEM_PATH_NONE;

	return false;
}

void QBattSysFSMethod::initPowerSupply()
{
	if (!searchSysFs_AC()) {
		psu->ac_adapter.online				= 0;
		psu->ac_adapter.psu_name			= "n/a";
	}

	if (!searchSysFs_Battery()) {
		psu->battery.psu_status				= "n/a";
		psu->battery.psu_present			= 0;
		psu->battery.psu_technology			= "n/a";
		psu->battery.psu_type				= "n/a";
		psu->battery.psu_cycle_count		= 0;
		psu->battery.psu_voltage_min_design	= 0;
		psu->battery.psu_voltage_now		= 0;
		psu->battery.psu_current_now		= 0;
		psu->battery.psu_charge_full_design	= 0;
		psu->battery.psu_charge_full		= 0;
		psu->battery.psu_charge_now			= 0;
		psu->battery.psu_capacity			= 0;
		psu->battery.psu_capacity_level		= "n/a";
		psu->battery.psu_model_name			= "n/a";
		psu->battery.psu_manufacturer		= "n/a";
		psu->battery.psu_serial_number		= "n/a";
	}
}

void QBattSysFSMethod::updatePowerSupply()
{
	// Update battery stats
	psu->battery.psu_capacity =
			getFileContents(psu->battery.path,
							FILE_BATT_CAPACITY).toInt();
	psu->battery.psu_capacity_level =
			getFileContents(psu->battery.path,
							FILE_BATT_CAPACITY_LEVEL);
	psu->battery.psu_charge_full =
			getFileContents(psu->battery.path,
							FILE_BATT_CHARGE_FULL).toInt();
	psu->battery.psu_charge_full_design =
			getFileContents(psu->battery.path,
							FILE_BATT_CHARGE_FULL_DESIGN).toInt();
	psu->battery.psu_charge_now =
			getFileContents(psu->battery.path,
							FILE_BATT_CHARGE_NOW).toInt();
	psu->battery.psu_current_now =
			getFileContents(psu->battery.path,
							FILE_BATT_CURRENT_NOW).toInt();
	psu->battery.psu_cycle_count =
			getFileContents(psu->battery.path,
							FILE_BATT_CYCLE_COUNT).toInt();
	psu->battery.psu_manufacturer =
			getFileContents(psu->battery.path,
							FILE_BATT_MANUFACTURER);
	psu->battery.psu_model_name =
			getFileContents(psu->battery.path,
							FILE_BATT_MODEL_NAME);
	psu->battery.psu_present =
			getFileContents(psu->battery.path,
							FILE_BATT_PRESENT).toInt();
	psu->battery.psu_serial_number =
			getFileContents(psu->battery.path,
							FILE_BATT_SERIAL_NUMBER);
	psu->battery.psu_status =
			getFileContents(psu->battery.path,
							FILE_BATT_STATUS);
	psu->battery.psu_technology =
			getFileContents(psu->battery.path,
							FILE_BATT_TECHNOLOGY);
	psu->battery.psu_type =
			getFileContents(psu->battery.path,
							PSU_FILE_TYPE);
	psu->battery.psu_voltage_min_design =
			getFileContents(psu->battery.path,
							FILE_BATT_VOLTAGE_MIN_DESIGN).toInt();
	psu->battery.psu_voltage_now =
			getFileContents(psu->battery.path,
							FILE_BATT_VOLTAGE_NOW).toInt();

	// Update AC stats
	psu->ac_adapter.online =
			getFileContents(psu->ac_adapter.path,
							FILE_ACAD_ONLINE).toInt();
	psu->ac_adapter.psu_name =
			getFileContents(psu->ac_adapter.path,
							PSU_FILE_TYPE);
}
