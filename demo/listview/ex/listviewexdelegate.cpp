#include "listviewexdelegate.h"

#include <QtCore/qdebug.h>
#include <QtCore/qcryptographichash.h>
#include <QtWidgets/qscrollbar.h>

#include "listview/qrlistview.h"

USING_NS_QRWIDGETS;

class ListviewExDelegatePrivate{
    QR_DECLARE_PUBLIC(ListviewExDelegate)

public:
    ListviewExDelegatePrivate(ListviewExDelegate* q) : q_ptr(q) {}

public:
    QMap<QString, ListviewExData*> groupHeadDatas;
};

ListviewExDelegate::ListviewExDelegate()
    : QrListVidewDelegate(),
      d_ptr(new ListviewExDelegatePrivate(this))
{

}

void ListviewExDelegate::sortImpl(bool update/* = true*/)
{
    QrListVidewDelegate::sortImpl(false);   //  delay update to end of function

    //  iterator, first is head of group
    QMap<QString, bool> existGroups;
    QVector<QrListViewData*> viewDatas = viewDataset();
    for(int dataIdx=0; dataIdx<viewDatas.size(); ++dataIdx) {
        ListviewExData* dataEx = static_cast<ListviewExData*>(viewDatas.at(dataIdx));
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

//    for(int dataIdx=0; dataIdx<viewDatas.size(); ++dataIdx) {
//        ListviewExData* dataEx = static_cast<ListviewExData*>(viewDatas.at(dataIdx));
//        if(nullptr == dataEx) {
//            continue;
//        }
//        qDebug() << dataEx->toString();
//    }

    int r = verScrollBarRangeMaxValue(0);
    listview()->verticalScrollBar()->setRange(0, r);

    if(update) {
        emit dataChanged(); //  delay update to here
    }
}

void ListviewExDelegate::addData(QrListViewData *data)
{
    ListviewExData* dataEx = static_cast<ListviewExData*>(data);
    if(nullptr == dataEx) {
        return;
    }
    dataEx->calcGroupInfo();

    Q_D(ListviewExDelegate);
    dataEx->setGroupHead(! d->groupHeadDatas.contains(dataEx->groupHex()));

    QrListVidewDelegate::addData(dataEx);
}

int ListviewExDelegate::itemCountToShow(int listviewHeight, int itemHeight)
{
    Q_UNUSED(itemHeight);
    return (listviewHeight/normalItemHeight()) + 2;
}

int ListviewExDelegate::verScrollBarRangeMaxValue(int itemHeight)
{
//    return QrListVidewDelegate::verScrollBarRangeMaxValue(itemHeight);

//    return itemsSize() * itemHeight - 2 * listview()->size().height();
//    return 37670 - listview()->height();

    Q_UNUSED(itemHeight);

    Q_D(const ListviewExDelegate);
    int widgetHeightOfAllDatas = 0;
    QVector<QrListViewData*> viewDatas = viewDataset();
    for(int dataIdx=0; dataIdx<viewDatas.size(); ++dataIdx) {
        ListviewExData* dataEx = static_cast<ListviewExData*>(viewDatas.at(dataIdx));
        if(nullptr == dataEx) {
            continue;
        }
        widgetHeightOfAllDatas += groupItemRenderHeight(dataEx);
    }
    return widgetHeightOfAllDatas - listview()->height();
}

int ListviewExDelegate::groupItemRenderHeight(QrListViewData *data) const
{
    ListviewExData* dataEx = static_cast<ListviewExData*>(data);
    if(nullptr == dataEx) {
        return 0;
    }
    if(dataEx->isGroupHead()) {
        return headItemHeight();
    }
    return normalItemHeight();
}


///////////////////////////////

class ListviewExDataPrivate{
    QR_DECLARE_PUBLIC(ListviewExData)

public:
    ListviewExDataPrivate(ListviewExData* q) : q_ptr(q) {}

public:
    bool isGroupHead = false;   //  是否是组头
    QString groupHex;   //  组的哈希值
};

ListviewExData::ListviewExData()
    : QrListViewData(),
      d_ptr(new ListviewExDataPrivate(this))
{

}

void ListviewExData::setGroupHead(bool isHead)
{
    Q_D(ListviewExData);
    d->isGroupHead = isHead;
}

bool ListviewExData::isGroupHead() const
{
    Q_D(const ListviewExData);
    return d->isGroupHead;
}

QString ListviewExData::groupHex() const
{
    Q_D(const ListviewExData);
    return d->groupHex;
}

void ListviewExData::calcGroupInfo()
{
    Q_D(ListviewExData);
    d->groupHex = QString(QCryptographicHash::hash(
                              groupKey().toUtf8(),
                              QCryptographicHash::Md5).toHex());
}
