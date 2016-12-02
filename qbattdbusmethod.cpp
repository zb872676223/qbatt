#include <QDebug>
#include <QtDBus/QtDBus>
#include <QStringList>
#include "qbattdbusmethod.h"

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
static const QString DBUS_BATT_ACAD_ONLINE          = "Online";             // Boolean (DBus)
static const QString DBUS_BATT_TIME_TO_EMPTY        = "TimeToEmpty";        // Int64 (DBus)
static const QString DBUS_BATT_TIME_TO_FULL         = "TimeToFull";         // Int64 (DBus)

QBattDBusMethod::QBattDBusMethod(psuinfo_t *psu)
{
    this->psu = psu;
}

bool QBattDBusMethod::initPowerSupply()
{
    QDBusObjectPath path;
    QList<QVariant> args;
    QVariant arg;

    // First we need to list available devices on the bus
    this->dbus_msg = QDBusMessage::createMethodCall("org.freedesktop.UPower",
                                         "/org/freedesktop/UPower",
                                         "org.freedesktop.UPower",
                                         "EnumerateDevices");
    QDBusPendingCall ret = QDBusConnection::systemBus().asyncCall(this->dbus_msg);
    ret.waitForFinished();
    if (ret.isValid()) {
        arg = ret.reply().arguments().at(0).value<QVariant>();
        const QDBusArgument argument(arg.value<QDBusArgument>());

        argument.beginArray();
        while (!argument.atEnd()) {
            argument >> path;
        }
        argument.endArray();

        this->psu->battery.path = QString(path.path());
    } else {
        return false;
    }

    this->dbus_msg = QDBusMessage::createMethodCall("org.freedesktop.UPower",
                                         this->psu->battery.path,
                                         "org.freedesktop.DBus.Properties",
                                         "GetAll");
    args.append("org.freedesktop.UPower.Device");
    this->dbus_msg.setArguments(args);

    return true;
}

void QBattDBusMethod::updatePowerSupply()
{
    if (!this->request_in_progress) {
        // Set the flag
        this->request_in_progress = true;

        // Perform and process the request
        QDBusPendingCall ret = QDBusConnection::systemBus().asyncCall(this->dbus_msg);
        ret.waitForFinished();

        if (ret.isValid()) {
            QMap<QString, QVariant> value;
            QVariant arg = ret.reply().arguments().at(0);
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
                value.insert(key, val);
            }
            argument.endArray();

            /*
             * Now we need to parse stored data. The trouble is
             * that each entry's type varies, so we need to define
             * its type first and then perform apropriate conversion
             * steps in order to get needed data.
             */
            this->parseDBusData(value);
        }

        // Release the flag
        this->request_in_progress = false;
    }
}

void QBattDBusMethod::parseDBusData(QMap<QString, QVariant> &value)
{
    tBatteryChargeState state;

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

    this->psu->ac_adapter.online                = value.value(DBUS_BATT_ACAD_ONLINE).toInt();
}
