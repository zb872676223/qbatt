#include <QSystemTrayIcon>
#include <QTimer>
#include <QDebug>
#include "qbattmain.h"
#include "qbattstats.h"
#include "ui_qbattmain.h"

QBattMain::QBattMain(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::QBattMain)
{
	ui->setupUi(this);

	model = new QBattModel(ui->tableWidget);
	stats = new QBattStats();
	timer = new QTimer(this);
	timer->setInterval(1000);
	timer->start();

	// Initial data update
	updateDynamicTableContents();
	updateStaticTableContents();

	QObject::connect(timer, SIGNAL(timeout()),
			this, SLOT(updateDynamicTableContents()));
}

QBattMain::~QBattMain()
{
	timer->stop();

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
