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
	trayTimer->setInterval(1000);
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
	qint8 trayCapacity = stats->getCapacity().toInt();;
	qint16 currentRate = stats->getCurrentNow().toInt() / 1000;
	QString battStatus = stats->getStatus().trimmed();

	trayToolTipText.clear();

	trayToolTipText.append("Status: ");
	trayToolTipText.append(battStatus);

	trayText.clear();

	if (trayCapacity == 100)
		trayText.append("F");
	else {
		trayToolTipText.append(QString().sprintf("\nRate: %dmAh", currentRate));
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
		QMessageBox *msg = new QMessageBox(QMessageBox::Information, "Exit qbatt:", "Do you really want to exit?",
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
