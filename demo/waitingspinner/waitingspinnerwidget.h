#ifndef WAITINGSPINNERWIDGET_H
#define WAITINGSPINNERWIDGET_H

#include <QWidget>

namespace Ui {
class WaitingSpinnerWidget;
}

class WaitingSpinnerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WaitingSpinnerWidget(QWidget *parent = 0);
    ~WaitingSpinnerWidget();

Q_SIGNALS:
    void timeout();

private:
    Ui::WaitingSpinnerWidget *ui;
};

#endif // WAITINGSPINNERWIDGET_H
