#include <QApplication>
#include "qbattmain.h"
#include "qbattsingle.h"

int main(int argc, char *argv[])
{
    QBattSingle single_instance("qbatt_single_instance");

    if (!single_instance.tryToRun())
        return EXIT_FAILURE;

    QApplication a(argc, argv);
    QBattMain w;

    a.setQuitOnLastWindowClosed(false);

    return a.exec();
}
