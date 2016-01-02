#include <QSystemTrayIcon>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include "qbattmain.h"
#include "qbattstats.h"
#include "ui_qbattmain.h"

QBattMain::QBattMain(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::QBattMain)
{
	ui->setupUi(this);

	trayFont = QFont("Hack", 10);
	trayPixmap = QPixmap(16, 16);
	trayPixmap.fill(Qt::white);
	trayPainter = new QPainter(&trayPixmap);
	trayPainter->setFont(trayFont);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setVisible(false);

	model = new QBattModel(ui->tableWidget);
	stats = new QBattStats();

	contentsTimer = new QTimer(this);
	contentsTimer->setInterval(1000);

	// Initial data update
	updateDynamicTableContents();
	updateStaticTableContents();

	trayTimer = new QTimer(this);
	trayTimer->setInterval(1000);
	trayTimer->start();

	QObject::connect(contentsTimer, SIGNAL(timeout()),
			this, SLOT(updateDynamicTableContents()));
	QObject::connect(trayTimer, SIGNAL(timeout()),
			this, SLOT(updateTrayLabel()));
	QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(systemExit()));
}

QBattMain::~QBattMain()
{
	if (contentsTimer->isActive())
		contentsTimer->stop();
	if (trayTimer->isActive())
		trayTimer->stop();

	delete ui;
}

void QBattMain::updateStaticTableContents()
{
	if (stats->checkDirExists()) {
		model->setValue(ROW_MANUFACTURER,
			stats->getManufacturer());
		model->setValue(ROW_MODEL_NAME,
			stats->getModelName());
		model->setValue(ROW_SERIAL_NUMBER,
			stats->getSerialNumber());
		model->setValue(ROW_TECHNOLOGY,
			stats->getTechnology());
		model->setValue(ROW_TYPE,
			stats->getType());
		model->setValue(ROW_VOLTAGE_MIN_DESIGN,
			stats->getVoltageMinDesign());
	}
}

void QBattMain::updateDynamicTableContents()
{
	if (stats->checkDirExists()) {
		model->setValue(ROW_CAPACITY,
			stats->getCapacity());
		model->setValue(ROW_CAPACITY_LEVEL,
			stats->getCapacityLevel());
		model->setValue(ROW_CHARGE_FULL,
			stats->getChargeFull());
		model->setValue(ROW_CHARGE_FULL_DESIGN,
			stats->getChargeFullDesign());
		model->setValue(ROW_CHARGE_NOW,
			stats->getChargeNow());
		model->setValue(ROW_CURRENT_NOW,
			stats->getCurrentNow());
		model->setValue(ROW_CYCLE_COUNT,
			stats->getCycleCount());
		model->setValue(ROW_PRESENT,
			stats->getPresent());
		model->setValue(ROW_STATUS,
			stats->getStatus());
		model->setValue(ROW_VOLTAGE_NOW,
			stats->getVoltageNow());
	}

	ui->tableWidget->resizeColumnsToContents();
}

void QBattMain::updateTrayLabel()
{
	qint8 trayCapacity = stats->getCapacity().toInt();;
	qint16 dischargeRate = stats->getCurrentNow().toInt() / 1000;
	QString battStatus = stats->getStatus().trimmed();

	if (trayCapacity == 100) {
		trayIcon->setVisible(false);
		return;
	}

	trayToolTipText.clear();

	trayToolTipText.append("Status: ");
	trayToolTipText.append(battStatus);
	trayToolTipText.append(QString().sprintf("\nRate: %dmAh", dischargeRate));
	trayToolTipText.append(QString().sprintf("\nCapacity: %d%%", trayCapacity));

	trayText.sprintf("%d", trayCapacity);

	trayPainter->eraseRect(trayPixmap.rect());
	trayPainter->drawText(trayPixmap.rect(), Qt::AlignCenter, trayText);
	trayIcon->setIcon(trayPixmap);

	trayIcon->setToolTip(trayToolTipText);

	trayIcon->setVisible(true);

}

void QBattMain::systemExit()
{
	exit(0);
}
