#include "listview/qrlistdelegate.h"

#include <QtCore/qdebug.h>
#include <QtCore/qtimer.h>
#include <QtCore/qqueue.h>
#include <QtCore/qpoint.h>

#include "listview/qrlistview.h"

NS_QRWIDGETS_BEGIN

///////////////////////////////////////////////////////

class QrListViewDataPrivate {
public:
    bool isOnClick = false;
    bool isOnDoubleClick = false;
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
    QMap<QString, QrListViewData*> mapDataset;

public:
    QrListViewData* menuData = nullptr;
    QrListViewData* doubleClickData = nullptr;
    QrListViewData* clickData = nullptr;

public:
    QrListVidewDelegatePrivate(QrListVidewDelegate* q);
    virtual ~QrListVidewDelegatePrivate();

public:
    QrListViewData *getDataByIndex(int index);
};

QrListVidewDelegatePrivate::QrListVidewDelegatePrivate(QrListVidewDelegate *q)
    : q_ptr(q) {}

QrListVidewDelegatePrivate::~QrListVidewDelegatePrivate()
{
    Q_Q(QrListVidewDelegate);
    q->clear();
}

QrListViewData* QrListVidewDelegatePrivate::getDataByIndex(int index)
{
    if(index > viewDataset.size()) {
        qDebug() << "list size is smaller than " << index;
        return nullptr;
    }

    QrListViewData* data = viewDataset[index];
    if(nullptr == data) {
        qDebug() << "cell's data is nullptr";
        return nullptr;
    }

    return data;
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

void QrListVidewDelegate::clear()
{
    Q_D(QrListVidewDelegate);
    Q_FOREACH(QrListViewData* data, d->rawDataset) {
        delete data;
        data = nullptr;
    }
    d->rawDataset.clear();
    d->viewDataset.clear();
    d->mapDataset.clear();
    d->filterFuncQueue.clear();

    emit dataChanged();
    emit dataEmpty(true);
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

    if(1 == d->rawDataset.size()) {
        emit dataEmpty(false);
    }
}

QrListViewData *QrListVidewDelegate::getData(const QString& key)
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
    d->filterFuncQueue.clear();

    d->clickData = nullptr;
    d->doubleClickData = nullptr;

    emit dataChanged();

    if(d->rawDataset.isEmpty()) {
        emit dataEmpty(true);
    }
}

bool QrListVidewDelegate::isDataExist(const QString& key) const
{
    Q_D(const QrListVidewDelegate);
    return d->mapDataset.contains(key);
}

QrListViewData *QrListVidewDelegate::menuData()
{
    Q_D(QrListVidewDelegate);
    return d->menuData;
}

QrListViewData *QrListVidewDelegate::doubleClickData()
{
    Q_D(QrListVidewDelegate);
    return d->doubleClickData;
}

QrListViewData *QrListVidewDelegate::clickData()
{
    Q_D(QrListVidewDelegate);
    return d->clickData;
}

bool QrListVidewDelegate::hasMenu() const
{
    return false;
}

void QrListVidewDelegate::onDoubleClick(QWidget *item, QrListViewData* data, bool click)
{
    Q_UNUSED(item);
    Q_UNUSED(data);
    Q_UNUSED(click);
}

void QrListVidewDelegate::onClick(QWidget *item, QrListViewData* data, bool click)
{
    Q_UNUSED(item);
    Q_UNUSED(data);
    Q_UNUSED(click);
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

int QrListVidewDelegate::rawSize() const
{
    Q_D(const QrListVidewDelegate);
    return d->rawDataset.size();
}

QrListViewData* QrListVidewDelegate::setItemWidgetByIndex(int index, QWidget *item)
{
    Q_D(QrListVidewDelegate);
    QrListViewData* data = d->getDataByIndex(index);
    if(nullptr == data) {
        qDebug() << "cell's data is nullptr";
        return nullptr;
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

    return data;
}

void QrListVidewDelegate::refreshDoubleClickData(int index)
{
    Q_D(QrListVidewDelegate);
    if(nullptr != d->doubleClickData) {
        d->doubleClickData->onDoubleClick(false);
    }

    d->doubleClickData = d->getDataByIndex(index);

    if(nullptr != d->doubleClickData) {
        d->doubleClickData->onDoubleClick(true);
    }
}

void QrListVidewDelegate::refreshClickData(int index)
{
    Q_D(QrListVidewDelegate);
    if(nullptr != d->clickData) {
        d->clickData->onClick(false);
    }

    d->clickData = d->getDataByIndex(index);

    if(nullptr != d->clickData) {
        d->clickData->onClick(true);
    }
}

///////////////////////////////////////////////////////

QrListViewData::QrListViewData()
    : d_ptr(new QrListViewDataPrivate)
{

}

QString QrListViewData::key() const
{
    return "";
}

bool QrListViewData::compare(const QrListViewData *other) const
{
    Q_UNUSED(other);
    return true;
}

bool QrListViewData::isOnClick() const
{
    Q_D(const QrListViewData);
    return d->isOnClick;
}

void QrListViewData::onClick(bool click)
{
    Q_D(QrListViewData);
    d->isOnClick = click;
}

bool QrListViewData::isOnDoubleClick() const
{
    Q_D(const QrListViewData);
    return d->isOnDoubleClick;
}

void QrListViewData::onDoubleClick(bool click)
{
    Q_D(QrListViewData);
    d->isOnDoubleClick = click;
}

