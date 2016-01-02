#include <QDebug>
#include "qbattmain.h"
#include "qbattstats.h"
#include "ui_qbattmain.h"

QBattMain::QBattMain(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::QBattMain)
{
	ui->setupUi(this);

	this->model = new QBattModel(ui->tableWidget);
	this->stats = new QBattStats();

	this->updateTableContents();
}

QBattMain::~QBattMain()
{
	delete ui;
}

void QBattMain::updateTableContents()
{
	if (this->stats->checkDirExists()) {
		this->model->setValue(ROW_CAPACITY,
			this->stats->getCapacity());
		this->model->setValue(ROW_CAPACITY_LEVEL,
			this->stats->getCapacityLevel());
		this->model->setValue(ROW_CHARGE_FULL,
			this->stats->getChargeFull());
		this->model->setValue(ROW_CHARGE_FULL_DESIGN,
			this->stats->getChargeFullDesign());
		this->model->setValue(ROW_CHARGE_NOW,
			this->stats->getChargeNow());
		this->model->setValue(ROW_CYCLE_COUNT,
			this->stats->getCycleCount());
		this->model->setValue(ROW_MANUFACTURER,
			this->stats->getManufacturer());
		this->model->setValue(ROW_MODEL_NAME,
			this->stats->getModelName());
		this->model->setValue(ROW_PRESENT,
			this->stats->getPresent());
		this->model->setValue(ROW_SERIAL_NUMBER,
			this->stats->getSerialNumber());
		this->model->setValue(ROW_STATUS,
			this->stats->getStatus());
		this->model->setValue(ROW_TECHNOLOGY,
			this->stats->getTechnology());
		this->model->setValue(ROW_TYPE,
			this->stats->getType());
		this->model->setValue(ROW_VOLTAGE_MIN_DESIGN,
			this->stats->getVoltageMinDesign());
		this->model->setValue(ROW_VOLTAGE_NOW,
			this->stats->getVoltageNow());
	}

	ui->tableWidget->resizeColumnsToContents();
}
