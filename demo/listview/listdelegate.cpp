#include "listdelegate.h"

#include <QtCore/qdebug.h>

#include "listcell.h"

USING_NS_QRWIDGETS;

void ListDelegate::appendUser(const QString &username, const QString &selfSign)
{
    ListCellData* data = new ListCellData;
    data->username = username;
    data->selfSign = selfSign;
    data->tooltip = QString("I'm %1").arg(username);
    addData(data);
}

QWidget *ListDelegate::createItemWidget()
{
    return new ListCell();
}

void ListDelegate::setItemWidgetByData(QrListViewData *data, QWidget *itemWidget)
{
    ListCell* cell = static_cast<ListCell*>(itemWidget);
    if(nullptr == cell) {
        qDebug() << "list cell is nullptr";
        return;
    }

    ListCellData* cellData = static_cast<ListCellData*>(data);
    if(nullptr == cellData) {
        qDebug() << "cell's data is nullptr";
        return;
    }

    cell->init(cellData);
}

bool ListCellData::compare(const QrListViewData *_other) const
{
    const ListCellData* other = static_cast<const ListCellData*>(_other);
    if(nullptr == other) {
        return false;
    }
    return username.compare(other->username);
}
