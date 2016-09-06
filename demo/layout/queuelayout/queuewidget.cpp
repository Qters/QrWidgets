#include "queuewidget.h"

#include "layout/qrqueuelayout.h"

QueueWidget::QueueWidget(QWidget* parent)
    : QWidget(parent), layout(this, 4, 4)
{
    setMinimumSize(QSize(500, 500));
    layout.setAlignment(Qt::AlignRight);
}

void QueueWidget::push()
{
    layout.pushWidget(createWidget());
}

void QueueWidget::pop()
{
    layout.popWidget();
}

void QueueWidget::insert(int row, int col)
{
    layout.insertWidget(row, col, createWidget());
}

void QueueWidget::remove(int row, int col)
{
    layout.removeWidget(row, col);
}

void QueueWidget::setAlignment(Qt::AlignmentFlag alignmentFlag)
{
    layout.setAlignment(alignmentFlag);
}

QueueItem *QueueWidget::createWidget()
{
    auto item = new QueueItem(this);
    connect(item, &QueueItem::destroyed, [this, item](){
        this->layout.removeWidget(item);
    });
    return item;
}

