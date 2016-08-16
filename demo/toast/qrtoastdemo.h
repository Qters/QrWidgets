#ifndef QRTOASTDEMO_H
#define QRTOASTDEMO_H


#include <QWidget>

namespace Ui {
class ToastWidget;
}

class QrToastDemo : public QWidget
{
    Q_OBJECT

public:
    explicit QrToastDemo(QWidget *parent = 0);
    ~QrToastDemo();

private:
    Ui::ToastWidget *ui;
};

#endif // QRTOASTDEMO_H
