#ifndef QUEUEWIDGET_H
#define QUEUEWIDGET_H

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlabel.h>

#include "layout/qrqueuelayout.h"
#include "queueitem.h"

class QueueWidget : public QWidget
{
    Q_OBJECT

public:
    QueueWidget(QWidget* parent = 0);

public:
    void push();
    void pop();
    void insert(int row, int col);
    void remove(int row, int col);
    void setAlignment(Qt::AlignmentFlag alignmentFlag);

private:
    QueueItem* createWidget();

private:
    Qters::QrWidgets::QrQueueLayout layout;
};

#endif // QUEUEWIDGET_H
