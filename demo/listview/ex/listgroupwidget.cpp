#include "listgroupwidget.h"

class ListGroupWidgetPrivate {
    QR_DECLARE_PUBLIC(ListGroupWidget)

public:
    ListGroupWidgetPrivate(ListGroupWidget *q) : q_ptr(q) {}

public:

};

ListGroupWidget::ListGroupWidget(QWidget *parent)
    : QWidget(parent),
      d_ptr(new ListGroupWidgetPrivate(this))
{
}

ListGroupWidget::~ListGroupWidget()
{

}
