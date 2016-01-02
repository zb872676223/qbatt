#ifndef QBATTMAIN_H
#define QBATTMAIN_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "qbattmodel.h"
#include "qbattstats.h"

namespace Ui {
	class QBattMain;
}

class QBattMain : public QMainWindow
{
		Q_OBJECT

	public:
		explicit QBattMain(QWidget *parent = 0);
		~QBattMain();

	private:
		Ui::QBattMain *ui;
		QBattModel *model;
		QTableWidget *widget;
		QBattStats *stats;
		QTimer *timer;

	private:
		void updateStaticTableContents();

	private slots:
		void updateDynamicTableContents();
};

#endif // QBATTMAIN_H
