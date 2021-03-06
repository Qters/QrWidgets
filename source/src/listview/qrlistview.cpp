﻿#include "listview/qrlistview.h"

#include <QtCore/qdebug.h>
#include <QtWidgets/qscrollbar.h>
#include <QtGui/qevent.h>
#include <QtCore/qqueue.h>
#include <QtCore/qpoint.h>

#include "listview/qrlistdelegate.h"

NS_QRWIDGETS_BEGIN

///////////////////////////////////////////////////////

class QrListViewPrivate {
    QR_DECLARE_PUBLIC(QrListView)

public:
    QMenu *menu = nullptr;

    QMap<QWidget*, int> widgetItemIndex;
    QMap<QrListViewData*, QWidget*> dataWidgets;
    QMap<QWidget*, QrListViewData*> widgetDatas;
    QQueue<QWidget*> itemWidgets;
    QrListVidewDelegate *delegate = nullptr;

    int itemHeight = 1;
    int itemsHeight = 1;
    int oldDatasetSize = 0;

    bool isScrollUp = false;

public:
    QrListViewPrivate(QrListView *q);
};

QrListViewPrivate::QrListViewPrivate(QrListView *q)
    : q_ptr(q)
{

}

NS_QRWIDGETS_END

USING_NS_QRWIDGETS;

///////////////////////////////////////////////////////

QrListView::QrListView(QWidget *parent)
    : QAbstractScrollArea(parent),
      d_ptr(new QrListViewPrivate(this))
{
    setAttribute(Qt::WA_AlwaysShowToolTips);

    connect(verticalScrollBar(), &QScrollBar::valueChanged, [this](int value){
        if(value == verticalScrollBar()->maximum()) {
            delegate()->onScrollToLast();
        }
    });
}

QrListView::~QrListView()
{
    Q_D(QrListView);
    d->dataWidgets.clear();
    d->widgetDatas.clear();
    d->itemWidgets.clear();
    d->widgetItemIndex.clear();
    d->delegate = nullptr;
}

void QrListView::scrollContentsBy(int dx, int dy)
{
    Q_UNUSED(dx);
    Q_UNUSED(dy);

    Q_D(QrListView);
    d->isScrollUp = (dy > 0);
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
        createWidget();
    }

    int listviewHeight = event->size().height();
    d->itemHeight = d->itemWidgets.front()->size().height();
    Q_ASSERT(0 != d->itemHeight);
    int itemCount = d->delegate->itemCountToShow(listviewHeight, d->itemHeight);

    for(;d->itemWidgets.size() < itemCount;){
        createWidget();
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
    Q_D(QrListView);
    QWidget* widget = nullptr;
    widget = static_cast<QWidget*>(watched);
    if(nullptr == widget ||
            ! d->itemWidgets.contains(widget)) {
        return QAbstractScrollArea::eventFilter(watched, event);
    }

    QrListViewData *preClickData = nullptr;
    switch(event->type()) {
    case QEvent::MouseButtonDblClick:
        preClickData = d->delegate->doubleClickData();
        d->delegate->itemDoubleClick(d->widgetItemIndex[widget]);
        if(nullptr != d->dataWidgets[preClickData]) {
            d->delegate->onDoubleClick(d->dataWidgets[preClickData], preClickData, false);
        }
        d->delegate->onDoubleClick(widget, d->delegate->doubleClickData(), true);
        onDoubleClick();
        break;
    case QEvent::MouseButtonRelease:
        preClickData = d->delegate->clickData();
        d->delegate->itemClick(d->widgetItemIndex[widget]);
        if(nullptr != d->dataWidgets[preClickData]) {
            d->delegate->onClick(d->dataWidgets[preClickData], preClickData, false);
        }
        d->delegate->onClick(widget, d->delegate->clickData(), true);
        onClick();
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

void QrListView::updateOne(const QString &key)
{
    if(! delegate()->isDataExist(key)) {
        return;
    }

    Q_D(QrListView);
    auto delegateData = delegate()->getData(key);
    if(nullptr == delegateData ||
            ! d->dataWidgets.contains(delegateData)) {
        return;
    }

    QWidget* widget = d->dataWidgets[delegateData];
    delegate()->setItemWidgetByData(delegateData, widget);
    widget->update();
}

void QrListView::onDoubleClick()
{

}

void QrListView::onClick()
{

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

    if(nullptr == d->delegate || d->delegate->rawDataset().isEmpty()) {
        return;
    }

    if(d->oldDatasetSize != d->delegate->itemsSize()) {
        verticalScrollBar()->setRange(0, d->delegate->verScrollBarRangeMaxValue(d->itemHeight));
        d->oldDatasetSize = d->delegate->itemsSize();
    }

    if(itemWidgets().isEmpty()) {
        return;
    }

    dateChangeUpdate();
}

void QrListView::dateChangeUpdate()
{
    Q_D(QrListView);

    int vscorllbarValue = verticalScrollBar()->value();
    int currentItemIndex = vscorllbarValue / d->itemHeight;
    int itemWidgetOffset =(vscorllbarValue % d->itemHeight) * -1;

    clearDataWidget();
    Q_FOREACH(QWidget* itemWidget, itemWidgets()) {
        itemWidget->move(0, itemWidgetOffset);
        setWidgetItemIndex(itemWidget, currentItemIndex);

        if(currentItemIndex < d->delegate->itemsSize()) {
            auto delegateData = d->delegate->setItemWidgetByIndex(currentItemIndex, itemWidget);
            if(nullptr != delegateData) {
                setDataWidget(delegateData, itemWidget);
            }
            itemWidget->show();
        } else {
            itemWidget->hide();
        }

        itemWidgetOffset += d->itemHeight;

        currentItemIndex += 1;
    }
}

void QrListView::clearDataWidget()
{
    Q_D(QrListView);
    d->dataWidgets.clear();
    d->widgetDatas.clear();
}

QQueue<QWidget *> QrListView::itemWidgets()
{
    Q_D(QrListView);
    return d->itemWidgets;
}

void QrListView::setWidgetItemIndex(QWidget* itemWidget, int itemIndex)
{
    Q_D(QrListView);
    d->widgetItemIndex[itemWidget] = itemIndex;
}

int QrListView::getWidgetItemIndex(QWidget* itemWidget) const
{
    Q_D(const QrListView);
    return d->widgetItemIndex[itemWidget];
}

void QrListView::setDataWidget(QrListViewData* delegateData, QWidget* itemWidget)
{
    Q_D(QrListView);
    d->dataWidgets[delegateData] = itemWidget;
    d->widgetDatas[itemWidget] = delegateData;
}

QrListViewData *QrListView::getDataOfWidget(QWidget *itemWidget) const
{
    Q_D(const QrListView);
    return d->widgetDatas[itemWidget];
}

QWidget* QrListView::createWidget()
{
    Q_D(QrListView);

    QWidget *itemWidget = d->delegate->createItemWidget();
    itemWidget->setParent(this);
    itemWidget->installEventFilter(this);
    d->itemWidgets.push_back(itemWidget);

    return itemWidget;
}

bool QrListView::isScrollUp() const
{
    Q_D(const QrListView);
    return d->isScrollUp;
}
