#ifndef QUEUECONTROLWIDGET_H
#define QUEUECONTROLWIDGET_H

#include <QWidget>

#include "queuewidget.h"

namespace Ui {
class QueueControlWidget;
}

class QueueControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QueueControlWidget(QWidget *parent = 0);
    ~QueueControlWidget();

    // QObject interface
public:
    virtual bool eventFilter(QObject *, QEvent *) override;

private:
    void switchWidgets(bool);

private:
    QueueWidget videosWidget;
    Ui::QueueControlWidget *ui;
};

#endif // QUEUECONTROLWIDGET_H
