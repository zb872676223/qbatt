#ifndef QBATTTRAY_T_H
#define QBATTTRAY_T_H

#include <QObject>
#include <QString>
#include <QSystemTrayIcon>
#include <QIcon>
#include <qbatticon_t.h>

struct _BatteryIcon_T {
  quint8 min;
  QIcon icon;
} ;

#define ICON_ROOT_PATH ":/icons/"

// icon names
namespace qBatIcons {
  static const QString noBattery = "battery-missing.png";
  namespace battery {
    static const QString icon_100 = "battery-100.png";
    static const QString icon_080 = "battery-080.png";
    static const QString icon_060 = "battery-060.png";
    static const QString icon_040 = "battery-040.png";
    static const QString icon_020  = "battery-caution.png";
    static const QString icon_010 = "battery-low.png";
  }
  namespace charging {
    static const QString icon_100 = "battery-charging.png";
    static const QString icon_080 = "battery-charging-080.png";
    static const QString icon_060 = "battery-charging-060.png";
    static const QString icon_040 = "battery-charging-040.png";
    static const QString icon_020 = "battery-charging-caution.png";
    static const QString icon_010 = "battery-charging-low.png";
  }
}

typedef QList <QBattIcon_T> IconsList_T;


class QBattTray_T : public QObject {
    Q_OBJECT
  public:
    explicit QBattTray_T (QObject *parent = 0);
    ~QBattTray_T();
    void setIcon (const QBattIcon_T &icon);
    QSystemTrayIcon* getSystemTray ();

  private:
    QSystemTrayIcon *trayIcon;
    int currentLowerLimit;

    QBattIcon_T *iconNoBattery;

    IconsList_T iconsBattery;
    IconsList_T iconsBatteryCharging;
    IconsList_T *iconsSet;
  signals:

  public slots:
    void batteryLevelChanged (int percent);
    void batteryStatusChanged (bool charging);
};

#endif // QBATTTRAY_T_H
