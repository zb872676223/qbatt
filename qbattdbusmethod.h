#ifndef QBATTDBUSMETHOD_H
#define QBATTDBUSMETHOD_H

#include <QtDBus/QtDBus>
#include <QString>
#include "qbattpsu.h"

class QBattDBusMethod
{
public:
    QBattDBusMethod(psuinfo_t *psu);
    bool initPowerSupply();
    void updatePowerSupply();

private:
    void parseDBusData(QMap<QString, QVariant> &value);

private:
    QDBusMessage        dbus_msg;
    psuinfo_t           *psu;
    bool                request_in_progress;
};

#endif // QBATTDBUSMETHOD_H
