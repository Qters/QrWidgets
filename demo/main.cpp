#include <QApplication>

#include "qrwidgetdemo.h"
#include "qrautorun.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Qters");
    QCoreApplication::setApplicationName("chaos.widgets");

    Qters::QrCommon::QrAutoRun autoRun("qrwidgets.demo");
    autoRun.autoRun(false);

    QrWidgetDemo w;
    w.show();

    return a.exec();
}
