#include "qbattmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QBattMain w;

	return a.exec();
}
