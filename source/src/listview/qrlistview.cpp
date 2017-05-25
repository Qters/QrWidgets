#include "listview/qrlistview.h"

#include <QtCore/qdebug.h>
#include <QtCore/qtimer.h>
#include <QtCore/qqueue.h>
#include <QtWidgets/qscrollbar.h>
#include <QtGui/qevent.h>
#include <QWheelEvent>
#include <QPoint>

NS_QRWIDGETS_BEGIN

///////////////////////////////////////////////////////

class QrListViewPrivate {
public:
    std::list<QWidget*> itemWidgets;
    QrListVidewDelegate *delegate = nullptr;

    int itemHeight = 1;
    int itemsHeight = 1;
    int oldDatasetSize = 0;
};

///////////////////////////////////////////////////////

class QrListViewDataPrivate {
};

///////////////////////////////////////////////////////

class QrListVidewDelegatePrivate {
public:
    bool watingFilter = false;
    QQueue<QRegExp> filterQueue;

    QVector<QrListViewData*> rawDataset;
    QVector<QrListViewData*> viewDataset;
    QMap<int, QrListViewData*> mapDataset;

public:
    virtual ~QrListVidewDelegatePrivate();
};

///////////////////////////////////////////////////////

QrListVidewDelegatePrivate::~QrListVidewDelegatePrivate()
{
    Q_FOREACH(QrListViewData* data, rawDataset) {
        delete data;
        data = nullptr;
    }
}

NS_QRWIDGETS_END

USING_NS_QRWIDGETS;

///////////////////////////////////////////////////////

QrListVidewDelegate::QrListVidewDelegate()
    : d_ptr(new QrListVidewDelegatePrivate)
{

}

void QrListVidewDelegate::managerData(QrListViewData *data)
{
    if(nullptr == data) {
        qDebug() << "list view can't mananger nullptr data";
        return;
    }

    Q_D(QrListVidewDelegate);
    d->rawDataset.push_back(data);
    d->mapDataset[data->key()] = data;
    d->viewDataset.push_back(data);
}

QrListViewData *QrListVidewDelegate::getData(int key)
{
    Q_D(QrListVidewDelegate);
    if(! d->mapDataset.contains(key)) {
        qDebug() << "listview's delegate havn't data of " << key;
        return nullptr;
    }

    return d->mapDataset[key];
}

void QrListVidewDelegate::filter(const QRegExp &regExp)
{
    Q_D(QrListVidewDelegate);
    int limitSize = 10;
    if(d->filterQueue.size() > limitSize) {
        for(int i=0; i<limitSize/2; i++) {
            d->filterQueue.removeAt(i);
        }
    }
    d->filterQueue.append(regExp);
    if (d->watingFilter) {
        qDebug() << "waiting filter";
        return;
    }

    d->watingFilter = true;
    QTimer::singleShot(500, this, [this](){
        qDebug() << "filter!";

        Q_D(QrListVidewDelegate);
        d->viewDataset.clear();
        Q_FOREACH(QrListViewData* data, d->rawDataset) {
            if(! data->filter(d->filterQueue.last())) {
                continue;
            }
            d->viewDataset.push_back(data);
        }
        emit dataChanged();

        d->watingFilter = false;
        qDebug() << "filter finish!";
    });
}

int QrListVidewDelegate::itemsSize() const
{
    Q_D(const QrListVidewDelegate);
    return d->viewDataset.size();
}

void QrListVidewDelegate::setItemWidgetByIndex(int index, QWidget *itemWidget)
{
    Q_D(QrListVidewDelegate);
    if(index > d->viewDataset.size()) {
        qDebug() << "list size is smaller than " << index;
        return;
    }

    QrListViewData* data = d->viewDataset[index];
    if(nullptr == data) {
        qDebug() << "cell's data is nullptr";
        return;
    }

    setItemWidgetByData(data, itemWidget);
}

///////////////////////////////////////////////////////

QrListViewData::QrListViewData()
    : d_ptr(new QrListViewDataPrivate)
{

}

int Qters::QrWidgets::QrListViewData::key() const
{
    return -1;
}

bool QrListViewData::filter(const QRegExp& regExp) const
{
    Q_UNUSED(regExp);
    return true;
}

///////////////////////////////////////////////////////

QrListView::QrListView(QWidget *parent)
    : QAbstractScrollArea(parent),
      d_ptr(new QrListViewPrivate())
{
    setAttribute(Qt::WA_AlwaysShowToolTips);
}

QrListView::~QrListView()
{
    Q_D(QrListView);
    d->itemWidgets.clear();
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
        QWidget *itemWidget = d->delegate->createItemWidget();
        itemWidget->setParent(this);
        d->itemWidgets.push_back(itemWidget);
    }

    int listviewHeight = event->size().height();
    d->itemHeight = d->itemWidgets.front()->size().height();
    Q_ASSERT(0 != d->itemHeight);
    int itemCount = listviewHeight/d->itemHeight + 2;

    for(;d->itemWidgets.size() < itemCount;){
        QWidget *itemWidget = d->delegate->createItemWidget();
        itemWidget->setParent(this);
        d->itemWidgets.push_back(itemWidget);
    }

    Q_FOREACH(QWidget* itemWidget, d->itemWidgets) {
        itemWidget->show();
    }

    d->itemsHeight = d->itemHeight * itemCount;

    dataChanged();

    return QAbstractScrollArea::resizeEvent(event);
}

void QrListView::setDelegate(QrListVidewDelegate *delegate)
{
    Q_D(QrListView);
    if(nullptr != d->delegate) {
        disconnect(d->delegate, &QrListVidewDelegate::dataChanged,
                   this, &QrListView::dataChanged);
    }
    d->delegate = delegate;
    connect(d->delegate, &QrListVidewDelegate::dataChanged,
            this, &QrListView::dataChanged);
}

QrListVidewDelegate *QrListView::delegate()
{
    Q_D(QrListView);
    return d->delegate;
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
