#include "qbatttray_t.h"
#include "qbatticon_t.h"
#ifdef DEBUG
#include <QDebug>
#endif
#include <algorithm>

QBattTray_T::QBattTray_T (QObject *parent) :
  QObject (parent) {

  this->currentLowerLimit = -2; // reset icon

  trayIcon = new QSystemTrayIcon (this);

  QString iconFullPath = ICON_ROOT_PATH;

  // default icon and icon if no battery
  iconNoBattery = new QBattIcon_T (iconFullPath + qBatIcons::noBattery, -1);
  this->setIcon (*iconNoBattery);

  iconsBattery.append (QBattIcon_T (iconFullPath + qBatIcons::battery::icon_100, 95));
  iconsBattery.append (QBattIcon_T (iconFullPath + qBatIcons::battery::icon_080, 80));
  iconsBattery.append (QBattIcon_T (iconFullPath + qBatIcons::battery::icon_060, 60));
  iconsBattery.append (QBattIcon_T (iconFullPath + qBatIcons::battery::icon_040, 40));
  iconsBattery.append (QBattIcon_T (iconFullPath + qBatIcons::battery::icon_020, 20));
  iconsBattery.append (QBattIcon_T (iconFullPath + qBatIcons::battery::icon_010, 10));

  iconsBatteryCharging.append (QBattIcon_T (iconFullPath + qBatIcons::charging::icon_100, 95));
  iconsBatteryCharging.append (QBattIcon_T (iconFullPath + qBatIcons::charging::icon_060, 60));
  iconsBatteryCharging.append (QBattIcon_T (iconFullPath + qBatIcons::charging::icon_040, 40));
  iconsBatteryCharging.append (QBattIcon_T (iconFullPath + qBatIcons::charging::icon_020, 20));
  iconsBatteryCharging.append (QBattIcon_T (iconFullPath + qBatIcons::charging::icon_010, 10));

  std::sort (iconsBattery.begin(), iconsBattery.end(), std::greater<QBattIcon_T>());
  std::sort (iconsBatteryCharging.begin(), iconsBatteryCharging.end(), std::greater<QBattIcon_T>());

  iconsSet = &iconsBattery;

  trayIcon->setVisible (true);
}

QBattTray_T::~QBattTray_T() {
  if (trayIcon)
    delete trayIcon;

  if (iconNoBattery)
    delete iconNoBattery;
}

void QBattTray_T::setIcon (const QBattIcon_T &icon) {
#ifdef DEBUG
  qDebug ("check icon..");
#endif
  if (currentLowerLimit != icon.min) {
#ifdef DEBUG
    qDebug ("icon changed...");
#endif
    this->currentLowerLimit = icon.min;
    this->trayIcon->setIcon (icon);
  };
}

QSystemTrayIcon *QBattTray_T::getSystemTray() {
  return trayIcon;
}

void QBattTray_T::batteryLevelChanged (int percent) {
  for (int i = 0; i < iconsSet->count(); i++) {
    if (percent >= iconsSet->at (i).min) {
      this->setIcon (iconsSet->at (i));
      return;
    };
  }
  return;
}

void QBattTray_T::batteryStatusChanged (bool charging) {
  int currentLimit = this->currentLowerLimit;
  this->currentLowerLimit = -2; // reset icon
  if (charging)
    this->iconsSet = &this->iconsBatteryCharging;
  else
    this->iconsSet = &this->iconsBattery;
  this->batteryLevelChanged (currentLimit);
  return;
}
