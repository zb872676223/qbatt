#include <QDebug>
#include <QtDBus/QtDBus>
#include <QStringList>
#include "qbattdbusmethod.h"

// Battery fields
static const QString DBUS_BATT_CAPACITY             = "Percentage";         // Double (DBus)
static const QString DBUS_BATT_CAPACITY_LEVEL       = "Capacity";           // Double (DBus)
static const QString DBUS_BATT_ENERGY_FULL          = "EnergyFull";         // Double (DBus)
static const QString DBUS_BATT_ENERGY_FULL_DESIGN   = "EnergyFullDesign";   // Double (DBus)
static const QString DBUS_BATT_ENERGY_NOW           = "Energy";             // Double (DBus)
static const QString DBUS_BATT_MANUFACTURER         = "Vendor";             // String (DBus)
static const QString DBUS_BATT_MODEL_NAME           = "Model";              // String (DBus)
static const QString DBUS_BATT_POWER_NOW            = "EnergyRate";         // Double (DBus)
static const QString DBUS_BATT_PRESENT              = "IsPresent";          // Boolean (DBus)
static const QString DBUS_BATT_SERIAL_NUMBER        = "Serial";             // String (DBus)
static const QString DBUS_BATT_TECHNOLOGY           = "Technology";         // Uint32 (DBus)
static const QString DBUS_BATT_VOLTAGE_NOW          = "Voltage";            // Double (DBus)
static const QString DBUS_BATT_STATUS               = "State";              // Uint32 (DBus)
static const QString DBUS_BATT_TIME_TO_EMPTY        = "TimeToEmpty";        // Int64 (DBus)
static const QString DBUS_BATT_TIME_TO_FULL         = "TimeToFull";         // Int64 (DBus)
// ACAD fields
static const QString DBUS_MAINS_ACAD_ONLINE         = "Online";             // Boolean (DBus)
// General fields
static const QString DBUS_SUPPLY_TYPE               = "Type";               // Uint32 (DBus)

static const QList<QVariant> dbus_args = QList<QVariant>() << QString("org.freedesktop.UPower.Device");

QBattDBusMethod::QBattDBusMethod(psuinfo_t *psu)
{
    this->psu = psu;
}

bool QBattDBusMethod::initPowerSupply()
{
    QDBusMessage msg;

    // First we need to list available devices on the bus
    msg = QDBusMessage::createMethodCall("org.freedesktop.UPower",
                                         "/org/freedesktop/UPower",
                                         "org.freedesktop.UPower",
                                         "EnumerateDevices");
    QDBusPendingCall ret = QDBusConnection::systemBus().asyncCall(msg);
    ret.waitForFinished();
    if (ret.isValid()) {
        bool status = this->parseDBusPath(ret.reply());
        if (!status)
            return status;
    } else {
        return false;
    }

    return true;
}

bool QBattDBusMethod::parseDBusPath(const QDBusMessage &msg)
{
    QDBusMessage request;
    QMap<QString, QVariant> ret;
    QVariant arg = msg.arguments().at(0);
    const QDBusArgument value(arg.value<QDBusArgument>());

    value.beginArray();
    while (!value.atEnd()) {
        QDBusObjectPath item;
        value >> item;

        /*
         * Now we need to dig each item looking for its type
         * to detect either we are trying to access a battery
         * or main PSU
         */
        request = QDBusMessage::createMethodCall("org.freedesktop.UPower",
                                                 item.path(),
                                                 "org.freedesktop.DBus.Properties",
                                                 "GetAll");
        request.setArguments(dbus_args);
        ret = this->parseDBusArgs(request);

        if (!ret.isEmpty()) {
            tSupplyType type = (tSupplyType)ret.value(DBUS_SUPPLY_TYPE).toInt();
            switch (type) {
            case SUPPLY_ACAD:
                this->psu->ac_adapter.path = item.path();
                break;
            case SUPPLY_BATT:
                this->psu->battery.path = item.path();
                break;
            default:
                break;
            }
        }
    }
    value.endArray();

    return true;
}

QMap<QString, QVariant> QBattDBusMethod::parseDBusArgs(const QDBusMessage &msg)
{
    QMap<QString, QVariant> ret;

    QDBusPendingCall call = QDBusConnection::systemBus().asyncCall(msg);
    call.waitForFinished();

    if (call.isValid()) {
        QVariant arg = call.reply().arguments().at(0);
        const QDBusArgument argument(arg.value<QDBusArgument>());

        /*
         * The data received consists of an array of elements, each
         * containing a dict entry (key, value), where 'key' is represented
         * as a string value, while 'value' holds the data of different types.
         */
        argument.beginArray();
        while (!argument.atEnd()) {
            QString key;
            QVariant val;

            argument.beginMapEntry();
            argument >> key >> val;
            argument.endMapEntry();

            ret.insert(key, val);
        }
        argument.endArray();
    }

    return ret;
}

void QBattDBusMethod::parseDBusData(const QMap<QString, QVariant> &value, tSupplyType type)
{
    tBatteryChargeState state;

    switch (type) {
    case SUPPLY_ACAD:
        this->psu->ac_adapter.online = value.value(DBUS_MAINS_ACAD_ONLINE).toInt();
        break;
    case SUPPLY_BATT:
        // Fill the data into the psuinfo_t structure
        this->psu->battery.psu_capacity             = value.value(DBUS_BATT_CAPACITY).toInt();
        this->psu->battery.psu_capacity_level       = value.value(DBUS_BATT_CAPACITY_LEVEL).toString();
        this->psu->battery.psu_energy_full          = QVariant(value.value(DBUS_BATT_ENERGY_FULL).toDouble() * 100000UL).toInt();
        this->psu->battery.psu_energy_full_design   = QVariant(value.value(DBUS_BATT_ENERGY_FULL_DESIGN).toDouble() * 100000UL).toInt();
        this->psu->battery.psu_energy_now           = QVariant(value.value(DBUS_BATT_ENERGY_NOW).toDouble() * 100000UL).toInt();
        this->psu->battery.psu_manufacturer         = value.value(DBUS_BATT_MANUFACTURER).toString();
        this->psu->battery.psu_model_name           = value.value(DBUS_BATT_MODEL_NAME).toString();
        this->psu->battery.psu_power_now            = QVariant(value.value(DBUS_BATT_POWER_NOW).toDouble() * 100000UL).toInt();
        this->psu->battery.psu_present              = value.value(DBUS_BATT_PRESENT).toInt();
        this->psu->battery.psu_serial_number        = value.value(DBUS_BATT_SERIAL_NUMBER).toString();
        this->psu->battery.psu_technology           = value.value(DBUS_BATT_TECHNOLOGY).toString();
        this->psu->battery.psu_voltage_now          = QVariant(value.value(DBUS_BATT_VOLTAGE_NOW).toDouble() * 100000UL).toInt();

        state = (tBatteryChargeState)value.value(DBUS_BATT_STATUS).toInt();
        switch (state) {
        case BATT_CHARGING:
            this->psu->battery.psu_status = BATT_STATUS_CHARGING;
            break;
        case BATT_DISCHARGING:
            this->psu->battery.psu_status = BATT_STATUS_DISCHARGING;
            break;
        case BATT_FULL:
            this->psu->battery.psu_status = BATT_STATUS_FULL;
            break;
        default:
            this->psu->battery.psu_status = BATT_STATUS_UNKNOWN;
            break;
        }
        break;
    default:
        break;
    }
}

void QBattDBusMethod::updatePowerSupply()
{
    if (!this->request_in_progress) {
        // Set the flag
        this->request_in_progress = true;

        // Issue Refresh() method call for AC adapter
        QDBusMessage msg;
        msg = QDBusMessage::createMethodCall("org.freedesktop.UPower",
                                             this->psu->ac_adapter.path,
                                             "org.freedesktop.UPower.Device",
                                             "Refresh");
        QDBusPendingCall ret = QDBusConnection::systemBus().asyncCall(msg);
        ret.waitForFinished();

        // Update ACAD
        msg = QDBusMessage::createMethodCall("org.freedesktop.UPower",
                                             this->psu->ac_adapter.path,
                                             "org.freedesktop.DBus.Properties",
                                             "GetAll");
        msg.setArguments(dbus_args);
        this->parseDBusData(this->parseDBusArgs(msg), SUPPLY_ACAD);

        // Issue Refresh() method call for battery
        msg = QDBusMessage::createMethodCall("org.freedesktop.UPower",
                                             this->psu->battery.path,
                                             "org.freedesktop.UPower.Device",
                                             "Refresh");
        ret = QDBusConnection::systemBus().asyncCall(msg);
        ret.waitForFinished();

        // Update Battery
        msg = QDBusMessage::createMethodCall("org.freedesktop.UPower",
                                             this->psu->battery.path,
                                             "org.freedesktop.DBus.Properties",
                                             "GetAll");
        msg.setArguments(dbus_args);
        this->parseDBusData(this->parseDBusArgs(msg), SUPPLY_BATT);

        // Release the flag
        this->request_in_progress = false;
    }
}
