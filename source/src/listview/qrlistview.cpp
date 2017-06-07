#include "listview/qrlistview.h"

#include <QtCore/qdebug.h>
#include <QtWidgets/qscrollbar.h>
#include <QtGui/qevent.h>
#include <QtCore/qpoint.h>

#include "listview/qrlistdelegate.h"

NS_QRWIDGETS_BEGIN

///////////////////////////////////////////////////////

class QrListViewPrivate {
    QR_DECLARE_PUBLIC(QrListView)

public:
    QMenu *menu = nullptr;

    QMap<QWidget*, int> widgetItemIndex;
    std::list<QWidget*> itemWidgets;
    QrListVidewDelegate *delegate = nullptr;

    int itemHeight = 1;
    int itemsHeight = 1;
    int oldDatasetSize = 0;

public:
    QrListViewPrivate(QrListView *q);
    QWidget *createWidget();
};

QrListViewPrivate::QrListViewPrivate(QrListView *q)
    : q_ptr(q)
{

}

QWidget* QrListViewPrivate::createWidget()
{
    Q_Q(QrListView);

    QWidget *itemWidget = delegate->createItemWidget();
    itemWidget->setParent(q);
    itemWidget->installEventFilter(q);
    itemWidgets.push_back(itemWidget);

    return itemWidget;
}

NS_QRWIDGETS_END

USING_NS_QRWIDGETS;

///////////////////////////////////////////////////////

QrListView::QrListView(QWidget *parent)
    : QAbstractScrollArea(parent),
      d_ptr(new QrListViewPrivate(this))
{
    setAttribute(Qt::WA_AlwaysShowToolTips);
}

QrListView::~QrListView()
{
    Q_D(QrListView);
    d->itemWidgets.clear();
    d->widgetItemIndex.clear();
    d->delegate = nullptr;
}

void QrListView::scrollContentsBy(int dx, int dy)
{
    Q_UNUSED(dx);
    Q_UNUSED(dy);
    dataChanged();
}

bool QrListView::event(QEvent *event)
{
    Q_D(QrListView);

    switch(event->type()) {
    case QEvent::Wheel:
    {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        if(nullptr == wheelEvent) {
            break;
        }

        int vscorllbarValue = verticalScrollBar()->value();
        vscorllbarValue += (wheelEvent->delta() > 0 ? -d->itemHeight : d->itemHeight);
        if(vscorllbarValue < 0) {
            vscorllbarValue = 0;
        }
        if(vscorllbarValue > verticalScrollBar()->maximum()) {
            vscorllbarValue = verticalScrollBar()->maximum();
        }

        verticalScrollBar()->setValue(vscorllbarValue);

        wheelEvent->accept();
        break;
    }
    }

    return QAbstractScrollArea::event(event);
}

void QrListView::resizeEvent(QResizeEvent * event)
{
    Q_D(QrListView);

    if(nullptr == d->delegate) {
        return QAbstractScrollArea::resizeEvent(event);
    }

    if(d->itemWidgets.empty()) {
        d->createWidget();
    }

    int listviewHeight = event->size().height();
    d->itemHeight = d->itemWidgets.front()->size().height();
    Q_ASSERT(0 != d->itemHeight);
    int itemCount = listviewHeight/d->itemHeight + 2;

    for(;d->itemWidgets.size() < itemCount;){
        d->createWidget();
    }

    Q_FOREACH(QWidget* itemWidget, d->itemWidgets) {
        itemWidget->show();
    }

    d->itemsHeight = d->itemHeight * itemCount;

    dataChanged();

    if(0 == d->delegate->rawSize()) {
        emit d->delegate->dataEmpty(true);
    }

    return QAbstractScrollArea::resizeEvent(event);
}

bool QrListView::eventFilter(QObject *watched, QEvent *event)
{
    switch(event->type()) {
    case QEvent::MouseButtonDblClick:
    {
        Q_D(QrListView);
        QWidget* widget = static_cast<QWidget*>(watched);
        if(nullptr == widget) {
            break;
        }
        d->delegate->itemDoubleClickByIndex(d->widgetItemIndex[widget]);
    }
        break;
    }

    return QAbstractScrollArea::eventFilter(watched, event);
}

void QrListView::setDelegate(QrListVidewDelegate *delegate)
{
    Q_D(QrListView);
    if(nullptr != d->delegate) {
        disconnect(d->delegate, &QrListVidewDelegate::dataChanged,
                   this, &QrListView::dataChanged);
    }
    if(delegate->hasMenu()) {
        delegate->initMenu(menu());
    }
    delegate->setListview(this);
    d->delegate = delegate;
    connect(d->delegate, &QrListVidewDelegate::dataChanged,
            this, &QrListView::dataChanged);
}

QrListVidewDelegate *QrListView::delegate()
{
    Q_D(QrListView);
    return d->delegate;
}

QMenu *QrListView::menu()
{
    Q_D(QrListView);
    if(nullptr == d->menu) {
        d->menu = new QMenu(this);
    }
    return d->menu;
}

void QrListView::dataChanged()
{
    Q_D(QrListView);

    if(nullptr == d->delegate) {
        return;
    }

    if(d->oldDatasetSize != d->delegate->itemsSize()) {
        verticalScrollBar()->setRange(0, d->delegate->itemsSize() * d->itemHeight - size().height());
        d->oldDatasetSize = d->delegate->itemsSize();
    }

    int vscorllbarValue = verticalScrollBar()->value();
    int currentItemIndex = vscorllbarValue / d->itemHeight;
    int itemWidgetOffset =(vscorllbarValue % d->itemHeight) * -1;

    Q_FOREACH(QWidget* itemWidget, d->itemWidgets) {
        itemWidget->move(0, itemWidgetOffset);
        d->widgetItemIndex[itemWidget] = currentItemIndex;

        if(currentItemIndex < d->delegate->itemsSize()) {
            d->delegate->setItemWidgetByIndex(currentItemIndex, itemWidget);
            itemWidget->show();
        } else {
            itemWidget->hide();
        }

        itemWidgetOffset += d->itemHeight;
        currentItemIndex += 1;
    }
}
