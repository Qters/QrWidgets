#include <QApplication>

#include "qrwidgetdemo.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QrWidgetDemo w;
    w.show();

    return a.exec();
}
