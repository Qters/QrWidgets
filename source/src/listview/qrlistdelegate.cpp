#include "listview/qrlistdelegate.h"

#include <QtCore/qdebug.h>
#include <QtCore/qtimer.h>
#include <QtCore/qqueue.h>
#include <QtCore/qpoint.h>

#include "listview/qrlistview.h"

NS_QRWIDGETS_BEGIN

///////////////////////////////////////////////////////

class QrListViewDataPrivate {
};

///////////////////////////////////////////////////////

class QrListVidewDelegatePrivate {
    QR_DECLARE_PUBLIC(QrListVidewDelegate)

public:
    bool watingSort = false;

    bool watingFilter = false;
    QQueue<std::function<bool (QrListViewData*)> > filterFuncQueue;

    QrListView *listview = nullptr;

    QVector<QrListViewData*> rawDataset;
    QVector<QrListViewData*> viewDataset;
    QMap<long, QrListViewData*> mapDataset;

public:
    QrListViewData* menuData = nullptr;

public:
    QrListVidewDelegatePrivate(QrListVidewDelegate* q);
    virtual ~QrListVidewDelegatePrivate();
};

QrListVidewDelegatePrivate::QrListVidewDelegatePrivate(QrListVidewDelegate *q)
    : q_ptr(q) {}

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
    : d_ptr(new QrListVidewDelegatePrivate(this))
{

}

void QrListVidewDelegate::initMenu(QMenu* menu)
{
    Q_UNUSED(menu);
}

void QrListVidewDelegate::setListview(QrListView *listview)
{
    Q_D(QrListVidewDelegate);
    d->listview = listview;
}

QrListView *QrListVidewDelegate::listview()
{
    Q_D(QrListVidewDelegate);
    return d->listview;
}

void QrListVidewDelegate::addData(QrListViewData *data)
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

QrListViewData *QrListVidewDelegate::getData(long key)
{
    Q_D(QrListVidewDelegate);
    if(! d->mapDataset.contains(key)) {
        qDebug() << "listview's delegate havn't data of " << key;
        return nullptr;
    }

    return d->mapDataset[key];
}

void QrListVidewDelegate::deleteData(QrListViewData *data)
{
    if(nullptr == data) {
        qDebug() << "data is nullptr, would not remove";
        return;
    }
    Q_D(QrListVidewDelegate);
    d->rawDataset.removeOne(data);
    d->mapDataset.remove(data->key());
    d->viewDataset.removeOne(data);

    emit dataChanged();
}

bool QrListVidewDelegate::isDataExist(long key) const
{
    Q_D(const QrListVidewDelegate);
    return d->mapDataset.contains(key);
}

QrListViewData *QrListVidewDelegate::menuData()
{
    Q_D(QrListVidewDelegate);
    return d->menuData;
}

bool QrListVidewDelegate::hasMenu() const
{
    return false;
}

void QrListVidewDelegate::sort()
{
    Q_D(QrListVidewDelegate);
    if (d->watingSort) {
        qDebug() << "waiting sort";
        return;
    }

    d->watingSort = true;
    QTimer::singleShot(500, this, [this](){
        Q_D(QrListVidewDelegate);
        qDebug() << "sort!";

        qSort(d->rawDataset.begin(), d->rawDataset.end(),
              [](const QrListViewData* left, const QrListViewData* right) {
            return left->compare(right);
        });

        if(d->rawDataset.size() != d->viewDataset.size()) {
            qSort(d->viewDataset.begin(), d->viewDataset.end(),
                  [](const QrListViewData* left, const QrListViewData* right) {
                return left->compare(right);
            });
        } else {
            d->viewDataset.clear();
            d->viewDataset = d->rawDataset;
        }

        emit dataChanged();

        d->watingSort = false;
        qDebug() << "sort finish!";
    });
}

void QrListVidewDelegate::filter(std::function<bool (QrListViewData*)> filterFunc)
{
    Q_D(QrListVidewDelegate);
    int limitSize = 10;
    if(d->filterFuncQueue.size() > limitSize) {
        for(int i=0; i<limitSize/2; i++) {
            d->filterFuncQueue.removeAt(i);
        }
    }
    d->filterFuncQueue.append(filterFunc);
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
            if(! d->filterFuncQueue.last()(data)) {
                continue;
            }
            d->viewDataset.push_back(data);
        }
        emit dataChanged();

        d->watingFilter = false;

        emit dataFiltered(d->viewDataset.empty());

        qDebug() << "filter finish!";
    });
}

int QrListVidewDelegate::itemsSize() const
{
    Q_D(const QrListVidewDelegate);
    return d->viewDataset.size();
}

void QrListVidewDelegate::setItemWidgetByIndex(int index, QWidget *item)
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

    setItemWidgetByData(data, item);

    if(hasMenu()) {
        item->setContextMenuPolicy(Qt::CustomContextMenu);
        item->disconnect();
        connect(item, &QWidget::customContextMenuRequested,
                [this, item, data](const QPoint &pos){
            Q_D(QrListVidewDelegate);
            d->menuData = data;
            listview()->menu()->exec(item->mapToGlobal(pos));
        });
    }
}

///////////////////////////////////////////////////////

QrListViewData::QrListViewData()
    : d_ptr(new QrListViewDataPrivate)
{

}

long QrListViewData::key() const
{
    return -1;
}

bool QrListViewData::compare(const QrListViewData *other) const
{
    Q_UNUSED(other);
    return true;
}
