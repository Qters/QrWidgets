#ifndef QRWIDGETDEMO_H
#define QRWIDGETDEMO_H

#include <QWidget>

namespace Ui {
class QrWidgetDemo;
}

class QrWidgetDemo : public QWidget
{
    Q_OBJECT

public:
    explicit QrWidgetDemo(QWidget *parent = 0);
    ~QrWidgetDemo();

private:
    Ui::QrWidgetDemo *ui;
};

#endif // QRWIDGETDEMO_H
