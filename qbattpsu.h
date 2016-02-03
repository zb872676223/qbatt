#ifndef QBATTPSU
#define QBATTPSU

#include <QString>

typedef struct _acad_info {
	QString		path;
	QString		psu_name;
	int			online;
} acad_t;

typedef struct _batt_info {
	QString		path;
	QString		psu_status;
	int			psu_present;
	QString		psu_technology;
	QString		psu_type;
	int			psu_cycle_count;
	int			psu_voltage_min_design;
	int			psu_voltage_now;
	int			psu_current_now;
	int			psu_charge_full_design;
	int			psu_charge_full;
	int			psu_charge_now;
	int			psu_capacity;
	int			psu_energy_full;
	int			psu_energy_full_design;
	int			psu_energy_now;
	int			psu_power_now;
	QString		psu_capacity_level;
	QString		psu_model_name;
	QString		psu_manufacturer;
	QString		psu_serial_number;
} battinfo_t;

typedef struct _psu_info {
	acad_t		ac_adapter;
	battinfo_t	battery;
} psuinfo_t;

static const QString BATT_STATUS_UNKNOWN		= "Unknown";
static const QString BATT_STATUS_DISCHARGING	= "Discharging";
static const QString BATT_STATUS_CHARGING		= "Charging";

#endif // QBATTPSU

