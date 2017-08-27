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

bool ListGroupWidget::initEntireWidget(ListGroupData *data)
{
    Q_UNUSED(data);

    if(! initHeadPartWidget(data)) {
        return false;
    }

    if(! initContentPartWidget(data)) {
        return false;
    }

    showHeadWidget(true);
    showContentWidget(true);

    return true;
}

bool ListGroupWidget::initHeadPartWidget(ListGroupData *data)
{
    Q_UNUSED(data);

    showHeadWidget(true);
    showContentWidget(false);

    return true;
}

bool ListGroupWidget::initContentPartWidget(ListGroupData *data)
{
    Q_UNUSED(data);

    showHeadWidget(false);
    showContentWidget(true);

    return true;
}

int ListGroupWidget::headHeight() const
{
    return entireHeight() - contentHeight();
}
