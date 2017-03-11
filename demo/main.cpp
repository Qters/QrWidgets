#include <QApplication>

#include "qrwidgetdemo.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Qters");
    QCoreApplication::setApplicationName("chaos.widgets");

    QrWidgetDemo w;
    w.show();

    return a.exec();
}
