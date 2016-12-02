#ifndef QBATTDBUSMETHOD_H
#define QBATTDBUSMETHOD_H

#include <QtDBus/QtDBus>
#include <QString>
#include "qbattpsu.h"

typedef enum {
        SUPPLY_ACAD = 1,
        SUPPLY_BATT = 2,
        SUPPLY_UNKN
} tSupplyType;

class QBattDBusMethod
{
public:
    QBattDBusMethod(psuinfo_t *psu);
    bool initPowerSupply();
    void updatePowerSupply();
private:
    void parseDBusData(const QMap<QString, QVariant> &value, tSupplyType type);
    bool parseDBusPath(const QDBusMessage &msg);
    QMap<QString, QVariant> parseDBusArgs(const QDBusMessage &msg);

private:
    psuinfo_t           *psu;
    bool                request_in_progress;
};

#endif // QBATTDBUSMETHOD_H
