#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include "qbattmain.h"
#include "qbattstats.h"

QBattMain::QBattMain(QWidget *parent) :
	QMainWindow(parent)
{
	trayFont = QFont("Hack", 10);
	trayPixmap = QPixmap(16, 16);
	trayPixmap.fill(Qt::white);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setVisible(false);

	stats = new QBattStats();

	trayTimer = new QTimer(this);
	trayTimer->setInterval(3000);
	trayTimer->start();

	QObject::connect(trayTimer, SIGNAL(timeout()),
					 this, SLOT(updateTrayLabel()));
	QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
					 this, SLOT(exitApplication(QSystemTrayIcon::ActivationReason)));
}

QBattMain::~QBattMain()
{
	if (trayTimer->isActive())
		trayTimer->stop();

	// Release memory
	delete stats;
	delete trayTimer;
	delete trayIcon;
}

void QBattMain::updateTrayLabel()
{
	// Update PSU information first
	stats->updatePowerSupplyInfo();
	qint8 trayCapacity = stats->getBatteryCapacity();
	qint16 currentRate = stats->getBatteryCurrentNow() / 1000;
	QString battStatus = stats->getBatteryStatus();
	bool adapterStatus = stats->getACOnline();

	trayToolTipText.clear();

	trayToolTipText.append("Status: ");
	if ((!QString().compare(battStatus, BATT_STATUS_UNKNOWN)) and
			(adapterStatus)) {
		trayToolTipText.append("On-line");
	} else {
		trayToolTipText.append(battStatus);
	}

	trayText.clear();

	if (trayCapacity == 100)
		trayText.append("F");
	else {
		if ((!QString().compare(battStatus, BATT_STATUS_CHARGING)) or
				(!QString().compare(battStatus, BATT_STATUS_DISCHARGING))) {
			trayToolTipText.append(QString().sprintf("\nRate: %d mAh",
													 currentRate));
			trayToolTipText.append("\nTime left: ");
			trayToolTipText.append(stats->getTimeLeft());
		}

		trayText.sprintf("%d", trayCapacity);
	}

	trayToolTipText.append(QString().sprintf("\nCapacity: %d%%", trayCapacity));

	trayPainter = new QPainter(&trayPixmap);
	trayPainter->setFont(trayFont);

	trayPainter->eraseRect(trayPixmap.rect());
	trayPainter->drawText(trayPixmap.rect(), Qt::AlignCenter, trayText);
	trayIcon->setIcon(trayPixmap);

	trayIcon->setToolTip(trayToolTipText);
	trayIcon->setVisible(true);

	delete trayPainter;
}

void QBattMain::exitApplication(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick) {
		QMessageBox *msg = new QMessageBox(QMessageBox::Information,
										   "Exit qbatt:", "Do you really want to exit?",
										   QMessageBox::Yes | QMessageBox::Cancel, NULL);

		int ret = msg->exec();
		delete msg;

		switch (ret) {
		case QMessageBox::Yes:
			// Stop the timer
			trayTimer->stop();
			// Release memory
			delete stats;
			delete trayTimer;
			delete trayIcon;

			exit(0);
			break;
		case QMessageBox::Cancel:
			break;
		}
	}

	return;
}
