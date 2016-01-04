#include <QApplication>
#include "qbattmain.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QBattMain w;

	a.setQuitOnLastWindowClosed(false);

	return a.exec();
}
