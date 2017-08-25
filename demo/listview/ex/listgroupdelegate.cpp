#include "listgroupdelegate.h"

#include <QtCore/qdebug.h>
#include <QtCore/qcryptographichash.h>
#include <QtWidgets/qscrollbar.h>

#include "listview/qrlistview.h"
#include "listgroupwidget.h"

USING_NS_QRWIDGETS;

class ListGroupDelegatePrivate{
    QR_DECLARE_PUBLIC(ListGroupDelegate)

public:
    ListGroupDelegatePrivate(ListGroupDelegate* q);

public:
    QMap<QString, QVector<ListGroupData*> > groupDatas;
};

ListGroupDelegatePrivate::ListGroupDelegatePrivate(ListGroupDelegate *q)
    : q_ptr(q)
{}

ListGroupDelegate::ListGroupDelegate()
    : QrListVidewDelegate(),
      d_ptr(new ListGroupDelegatePrivate(this))
{

}

void ListGroupDelegate::showGroupItems(const QString &groupHex, bool visible)
{
    Q_D(ListGroupDelegate);
    if(! d->groupDatas.contains(groupHex)) {
        qDebug() << "group delegate not contain this group";
        return;
    }

    Q_FOREACH(ListGroupData* data, d->groupDatas[groupHex]) {
        data->setVisible(visible);
    }
}

void ListGroupDelegate::sortImpl(bool update/* = true*/)
{
    QrListVidewDelegate::sortImpl(false);   //  delay update to end of function

    //  iterator, first is head of group
    QMap<QString, bool> existGroups;
    QVector<QrListViewData*> viewDatas = viewDataset();
    for(int dataIdx=0; dataIdx<viewDatas.size(); ++dataIdx) {
        ListGroupData* dataEx = static_cast<ListGroupData*>(viewDatas.at(dataIdx));
        if(nullptr == dataEx) {
            continue;
        }
        if(existGroups.contains(dataEx->groupHex())) {
            dataEx->setGroupHead(false);
        } else {
            dataEx->setGroupHead(true);
            existGroups[dataEx->groupHex()] = true;
        }
    }

    listview()->verticalScrollBar()->setRange(0, verScrollBarRangeMaxValue(0));

    if(update) {
        emit dataChanged(); //  delay update to here
    }
}

void ListGroupDelegate::addData(QrListViewData *data)
{
    ListGroupData* dataEx = static_cast<ListGroupData*>(data);
    if(nullptr == dataEx) {
        return;
    }
    dataEx->calcGroupInfo();

    Q_D(ListGroupDelegate);
    dataEx->setGroupHead(! d->groupDatas.contains(dataEx->groupHex()));

    if(d->groupDatas.contains(dataEx->groupHex())) {
        d->groupDatas[dataEx->groupHex()].push_back(dataEx);
    } else {
        d->groupDatas[dataEx->groupHex()] = (QVector<ListGroupData*>() << dataEx);
    }

    QrListVidewDelegate::addData(dataEx);
}

int ListGroupDelegate::itemCountToShow(int listviewHeight, int itemHeight)
{
    Q_UNUSED(itemHeight);
    return (listviewHeight/normalItemHeight()) + 2;
}

int ListGroupDelegate::verScrollBarRangeMaxValue(int itemHeight)
{
    Q_UNUSED(itemHeight);

    int widgetHeightOfAllDatas = 0;
    QVector<QrListViewData*> viewDatas = viewDataset();
    for(int dataIdx=0; dataIdx<viewDatas.size(); ++dataIdx) {
        ListGroupData* dataEx = static_cast<ListGroupData*>(viewDatas.at(dataIdx));
        if(nullptr == dataEx) {
            continue;
        }
        widgetHeightOfAllDatas += groupItemRenderHeight(dataEx);
    }
    return widgetHeightOfAllDatas - listview()->height();
}

void ListGroupDelegate::setItemWidgetByData(QrListViewData *_data, QWidget *_itemWidget)
{
    ListGroupWidget *itemWidget = static_cast<ListGroupWidget *>(_itemWidget);
    if(nullptr == itemWidget) {
        return;
    }

    ListGroupData *data = static_cast<ListGroupData *>(_data);
    if(nullptr == data) {
        return;
    }

    if(data->isGroupHead()) {
        if(data->isVisible()) {
            itemWidget->initHeadWidget(data);
        } else {
            itemWidget->initOnlyHeadWidget(data);
        }
    } else {
        itemWidget->initNormalWidget(data);
    }
}

int ListGroupDelegate::groupItemRenderHeight(QrListViewData *data) const
{
    ListGroupData* dataEx = static_cast<ListGroupData*>(data);
    if(nullptr == dataEx) {
        return 0;
    }
    if(dataEx->isGroupHead()) {
        return headItemHeight();
    }
    return normalItemHeight();
}


///////////////////////////////

class ListGroupDataPrivate{
    QR_DECLARE_PUBLIC(ListGroupData)

public:
    ListGroupDataPrivate(ListGroupData* q) : q_ptr(q) {}

public:
    bool isVisible = true;
    bool isGroupHead = false;
    QString groupHex;
};

ListGroupData::ListGroupData()
    : QrListViewData(),
      d_ptr(new ListGroupDataPrivate(this))
{

}

void ListGroupData::setVisible(bool visible)
{
    Q_D(ListGroupData);
    d->isVisible = visible;
}

bool ListGroupData::isVisible() const
{
    Q_D(const ListGroupData);
    return d->isVisible;
}

void ListGroupData::setGroupHead(bool isHead)
{
    Q_D(ListGroupData);
    d->isGroupHead = isHead;
}

bool ListGroupData::isGroupHead() const
{
    Q_D(const ListGroupData);
    return d->isGroupHead;
}

QString ListGroupData::groupHex() const
{
    Q_D(const ListGroupData);
    return d->groupHex;
}

void ListGroupData::calcGroupInfo()
{
    Q_D(ListGroupData);
    d->groupHex = QString(QCryptographicHash::hash(
                              groupKey().toUtf8(),
                              QCryptographicHash::Md5).toHex());
}
