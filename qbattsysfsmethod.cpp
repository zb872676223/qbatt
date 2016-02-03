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
static const QString FILE_BATT_ENERGY_FULL			= "energy_full";
static const QString FILE_BATT_ENERGY_FULL_DESIGN	= "energy_full_design";
static const QString FILE_BATT_ENERGY_NOW			= "energy_now";
static const QString FILE_BATT_MANUFACTURER			= "manufacturer";
static const QString FILE_BATT_MODEL_NAME			= "model_name";
static const QString FILE_BATT_POWER_NOW			= "power_now";
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

bool QBattSysFSMethod::checkFileExists(QString fpath, QString fname)
{
	QString path;

	if (!checkDirExists(fpath))
		return false;

	path.append(fpath);
	path.append('/');
	path.append(fname);

	QFile file(path);

	return file.exists();
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

int QBattSysFSMethod::getACADOnline()
{
	int ret = -1;

	if (checkFileExists(psu->ac_adapter.path, FILE_ACAD_ONLINE))
		ret = getFileContents(psu->ac_adapter.path, FILE_ACAD_ONLINE).toInt();

	return ret;
}

QString QBattSysFSMethod::getACADName()
{
	if (checkFileExists(psu->ac_adapter.path, PSU_FILE_TYPE))
		return getFileContents(psu->ac_adapter.path, PSU_FILE_TYPE);

	return RESPONSE_N_A;
}

QString QBattSysFSMethod::getPSUStatus()
{
	if (checkFileExists(psu->battery.path, FILE_BATT_STATUS))
		return getFileContents(psu->battery.path, FILE_BATT_STATUS);

	return RESPONSE_N_A;
}

int QBattSysFSMethod::getPSUPresent()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_PRESENT))
		ret = getFileContents(psu->battery.path, FILE_BATT_PRESENT).toInt();

	return ret;
}

QString QBattSysFSMethod::getPSUTechnology()
{
	if (checkFileExists(psu->battery.path, FILE_BATT_TECHNOLOGY))
		return getFileContents(psu->battery.path, FILE_BATT_TECHNOLOGY);

	return RESPONSE_N_A;
}

QString QBattSysFSMethod::getPSUType()
{
	if (checkFileExists(psu->battery.path, PSU_FILE_TYPE))
		return getFileContents(psu->battery.path, PSU_FILE_TYPE);

	return RESPONSE_N_A;
}

int QBattSysFSMethod::getPSUCycleCount()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_CYCLE_COUNT))
		ret = getFileContents(psu->battery.path, FILE_BATT_CYCLE_COUNT).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUVoltageMinDesign()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_VOLTAGE_MIN_DESIGN))
		ret = getFileContents(psu->battery.path, FILE_BATT_VOLTAGE_MIN_DESIGN).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUVoltageNow()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_VOLTAGE_NOW))
		ret = getFileContents(psu->battery.path, FILE_BATT_VOLTAGE_NOW).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUCurrentNow()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_CURRENT_NOW))
		ret = getFileContents(psu->battery.path, FILE_BATT_CURRENT_NOW).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUChargeFull()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_CHARGE_FULL))
		ret = getFileContents(psu->battery.path, FILE_BATT_CHARGE_FULL).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUChargeFullDesign()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_CHARGE_FULL_DESIGN))
		ret = getFileContents(psu->battery.path, FILE_BATT_CHARGE_FULL_DESIGN).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUChargeNow()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_CHARGE_NOW))
		ret = getFileContents(psu->battery.path, FILE_BATT_CHARGE_NOW).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUCapacity()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_CAPACITY))
		ret = getFileContents(psu->battery.path, FILE_BATT_CAPACITY).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUEnergyFull()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_ENERGY_FULL))
		ret = getFileContents(psu->battery.path, FILE_BATT_ENERGY_FULL).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUEnergyFullDesign()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_ENERGY_FULL_DESIGN))
		ret = getFileContents(psu->battery.path, FILE_BATT_ENERGY_FULL_DESIGN).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUEnergyNow()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_ENERGY_NOW))
		ret = getFileContents(psu->battery.path, FILE_BATT_ENERGY_NOW).toInt();

	return ret;
}

int QBattSysFSMethod::getPSUPowerNow()
{
	int ret = -1;

	if (checkFileExists(psu->battery.path, FILE_BATT_POWER_NOW))
		ret = getFileContents(psu->battery.path, FILE_BATT_POWER_NOW).toInt();

	return ret;
}

QString QBattSysFSMethod::getPSUCapacityLevel()
{
	if (checkFileExists(psu->battery.path, FILE_BATT_CAPACITY_LEVEL))
		return getFileContents(psu->battery.path, FILE_BATT_CAPACITY_LEVEL);

	return RESPONSE_N_A;
}

QString QBattSysFSMethod::getPSUModelName()
{
	if (checkFileExists(psu->battery.path, FILE_BATT_MODEL_NAME))
		return getFileContents(psu->battery.path, FILE_BATT_MODEL_NAME);

	return RESPONSE_N_A;
}

QString QBattSysFSMethod::getPSUManufacturer()
{
	if (checkFileExists(psu->battery.path, FILE_BATT_MANUFACTURER))
		return getFileContents(psu->battery.path, FILE_BATT_MANUFACTURER);

	return RESPONSE_N_A;
}

QString QBattSysFSMethod::getPSUSerialNumber()
{
	if (checkFileExists(psu->battery.path, FILE_BATT_SERIAL_NUMBER))
		return getFileContents(psu->battery.path, FILE_BATT_MANUFACTURER);

	return RESPONSE_N_A;
}

void QBattSysFSMethod::initPowerSupply()
{
	if (!searchSysFs_AC()) {
		psu->ac_adapter.online				= -1;
		psu->ac_adapter.psu_name			= "n/a";
	}

	if (!searchSysFs_Battery()) {
		psu->battery.psu_status				= "n/a";
		psu->battery.psu_present			= -1;
		psu->battery.psu_technology			= "n/a";
		psu->battery.psu_type				= "n/a";
		psu->battery.psu_cycle_count		= -1;
		psu->battery.psu_voltage_min_design	= -1;
		psu->battery.psu_voltage_now		= -1;
		psu->battery.psu_current_now		= -1;
		psu->battery.psu_charge_full_design	= -1;
		psu->battery.psu_charge_full		= -1;
		psu->battery.psu_charge_now			= -1;
		psu->battery.psu_capacity			= -1;
		psu->battery.psu_energy_full		= -1;
		psu->battery.psu_energy_full_design	= -1;
		psu->battery.psu_energy_now			= -1;
		psu->battery.psu_power_now			= -1;
		psu->battery.psu_capacity_level		= "n/a";
		psu->battery.psu_model_name			= "n/a";
		psu->battery.psu_manufacturer		= "n/a";
		psu->battery.psu_serial_number		= "n/a";
	}
}

void QBattSysFSMethod::updatePowerSupply()
{
	// Update battery stats
	psu->battery.psu_capacity			= getPSUCapacity();
	psu->battery.psu_capacity_level		= getPSUCapacityLevel();
	psu->battery.psu_charge_full		= getPSUChargeFull();
	psu->battery.psu_charge_full_design	= getPSUChargeFullDesign();
	psu->battery.psu_charge_now			= getPSUChargeNow();
	psu->battery.psu_current_now		= getPSUCurrentNow();
	psu->battery.psu_cycle_count		= getPSUCycleCount();
	psu->battery.psu_energy_full		= getPSUEnergyFull();
	psu->battery.psu_energy_full_design	= getPSUEnergyFullDesign();
	psu->battery.psu_energy_now			= getPSUEnergyNow();
	psu->battery.psu_manufacturer		= getPSUManufacturer();
	psu->battery.psu_model_name			= getPSUModelName();
	psu->battery.psu_power_now			= getPSUPowerNow();
	psu->battery.psu_present			= getPSUPresent();
	psu->battery.psu_serial_number		= getPSUSerialNumber();
	psu->battery.psu_status				= getPSUStatus();
	psu->battery.psu_technology			= getPSUTechnology();
	psu->battery.psu_type				= getPSUType();
	psu->battery.psu_voltage_min_design	= getPSUVoltageMinDesign();
	psu->battery.psu_voltage_now		= getPSUVoltageNow();

	// Update AC stats
	psu->ac_adapter.online				= getACADOnline();
	psu->ac_adapter.psu_name			= getACADName();
}
