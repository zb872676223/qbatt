#ifndef QBATTMAIN_H
#define QBATTMAIN_H

#include <QMainWindow>
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

	private:
		void updateTableContents();
};

#endif // QBATTMAIN_H
