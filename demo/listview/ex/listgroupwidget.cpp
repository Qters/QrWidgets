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

bool ListGroupWidget::initOnlyHeadWidget(ListGroupData *data)
{
    if(! initHeadWidget(data)) {
        return false;
    }

    showContentWidget(false);
}

bool ListGroupWidget::initHeadWidget(ListGroupData *data)
{
    Q_UNUSED(data);

    showHeadWidget(true);
    showContentWidget(true);

    return true;
}

bool ListGroupWidget::initNormalWidget(ListGroupData *data)
{
    Q_UNUSED(data);

    showHeadWidget(false);
    showContentWidget(true);

    return true;
}
