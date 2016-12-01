#include <QFile>
#include <QDir>
#include <QDebug>
#include "qbattstats.h"
#include "qbattsysfsmethod.h"
#include "qbattdbusmethod.h"

QBattStats::QBattStats()
{
	// Init ACAD's fields
	this->psu.ac_adapter.online					= -1;
	this->psu.ac_adapter.path					= "";
	this->psu.ac_adapter.psu_name				= "";
	// Init battery's fields
	this->psu.battery.path						= "";
	this->psu.battery.psu_capacity				= -1;
	this->psu.battery.psu_capacity_level		= "";
	this->psu.battery.psu_charge_full			= -1;
	this->psu.battery.psu_charge_full_design	= -1;
	this->psu.battery.psu_charge_now			= -1;
	this->psu.battery.psu_current_now			= -1;
	this->psu.battery.psu_cycle_count			= -1;
	this->psu.battery.psu_energy_full			= -1;
	this->psu.battery.psu_energy_full_design	= -1;
	this->psu.battery.psu_energy_now			= -1;
	this->psu.battery.psu_manufacturer			= "";
	this->psu.battery.psu_model_name			= "";
	this->psu.battery.psu_power_now				= -1;
	this->psu.battery.psu_present				= -1;
	this->psu.battery.psu_serial_number			= "";
	this->psu.battery.psu_status				= "";
	this->psu.battery.psu_technology			= "";
	this->psu.battery.psu_type					= "";
	this->psu.battery.psu_voltage_min_design	= -1;
	this->psu.battery.psu_voltage_now			= -1;

    // Try DBus, otherwise, if error - use SysFS
    this->dbus_method = new QBattDBusMethod(&this->psu);
    bool success = this->dbus_method->initPowerSupply();
    if (success) {
        this->method = METHOD_DBUS;
    } else {
        delete this->dbus_method;
        this->sysfs_method = new QBattSysFSMethod(&this->psu);
        this->sysfs_method->initPowerSupply();
        this->method = METHOD_SYSFS;
    }
}

QBattStats::~QBattStats()
{
    if (this->method == METHOD_DBUS)
        delete this->dbus_method;
    else if (this->method == METHOD_SYSFS)
        delete this->sysfs_method;
}

void QBattStats::updatePowerSupplyInfo()
{
    if (this->method == METHOD_DBUS)
        this->dbus_method->updatePowerSupply();
    else if (this->method == METHOD_SYSFS)
        sysfs_method->updatePowerSupply();
}

int QBattStats::getBatteryCapacity()
{
	return psu.battery.psu_capacity;
}

QString QBattStats::getBatteryCapacityLevel()
{
	return psu.battery.psu_capacity_level;
}

int QBattStats::getBatteryChargeFull()
{
	return psu.battery.psu_charge_full;
}

int QBattStats::getBatteryChargeFullDesign()
{
	return psu.battery.psu_charge_full_design;
}

int QBattStats::getBatteryChargeNow()
{
	return psu.battery.psu_charge_now;
}

int QBattStats::getBatteryCurrentNow()
{
	return psu.battery.psu_current_now;
}

int QBattStats::getBatteryCycleCount()
{
	return psu.battery.psu_cycle_count;
}

int QBattStats::getBatteryEnergyFull()
{
	return psu.battery.psu_energy_full;
}

int QBattStats::getBatteryEnergyFullDesign()
{
	return psu.battery.psu_energy_full_design;
}

int QBattStats::getBatteryEnergyNow()
{
	return psu.battery.psu_energy_now;
}

QString QBattStats::getBatteryManufacturer()
{
	return psu.battery.psu_manufacturer;
}

QString QBattStats::getBatteryModelName()
{
	return psu.battery.psu_model_name;
}

int QBattStats::getBatteryPowerNow()
{
	return psu.battery.psu_power_now;
}

int QBattStats::getBatteryPresent()
{
	return psu.battery.psu_present;
}

QString QBattStats::getBatterySerialNumber()
{
	return psu.battery.psu_serial_number;
}

QString QBattStats::getBatteryStatus()
{
	return psu.battery.psu_status;
}

QString QBattStats::getBatteryTechnology()
{
	return psu.battery.psu_technology;
}

QString QBattStats::getBatteryType()
{
	return psu.battery.psu_type;
}

int QBattStats::getBatteryVoltageMinDesign()
{
	return psu.battery.psu_voltage_min_design;
}

int QBattStats::getBatteryVoltageNow()
{
	return psu.battery.psu_voltage_now;
}

int QBattStats::getACOnline()
{
	return psu.ac_adapter.online;
}

QString QBattStats::getACType()
{
	return psu.ac_adapter.psu_name;
}

QString QBattStats::getTimeLeft()
{
	int hours = -1;
	int minutes = -1;
	int seconds = -1;
	int remainingCapacity = -1;
	int lastCapacity = -1;
	int current = -1;
	QString battery_status;
	QString ret;

	current = getBatteryCurrentNow();
	if (current != -1)
		current /= 1000;
	else
		current = (getBatteryPowerNow() / (getBatteryVoltageNow() / 1000));

	remainingCapacity = getBatteryChargeNow();
	if (remainingCapacity != -1)
		remainingCapacity /= 1000;
	else
		remainingCapacity = (getBatteryEnergyNow() / (getBatteryVoltageNow() / 1000));

	lastCapacity = getBatteryChargeFull();
	if (lastCapacity != -1)
		lastCapacity /= 1000;
	else
		lastCapacity = (getBatteryEnergyFull() / (getBatteryVoltageNow() / 1000));

	battery_status = getBatteryStatus();
	/*
	 * This idea has been taken from acpi utility by
	 * Grahame Bowland <grahame@angrygoats.net> and
	 * Michael Meskes  <meskes@debian.org>
	 */
	if (current > 0) {
		if (!QString().compare(battery_status,
							   BATT_STATUS_CHARGING))
			seconds = 3600 * (lastCapacity - remainingCapacity) / current;
		else if (!QString().compare(battery_status,
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
