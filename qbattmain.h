#ifndef QBATTMAIN_H
#define QBATTMAIN_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include "qbatttray_t.h"
#include "qbattstats.h"

class QBattMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit QBattMain(QWidget *parent = 0);
    ~QBattMain();

private:
    QBattStats *stats;
    QTimer *trayTimer;

    QBattTray_T *trayIcon;
private:
    QString trayText;
    QString trayToolTipText;
    QFont trayFont;
    QPixmap trayPixmap;
    QPainter *trayPainter;

private slots:
    void updateTrayLabel();
    void exitApplication(QSystemTrayIcon::ActivationReason reason);
};

#endif // QBATTMAIN_H
