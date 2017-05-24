#include "listdelegate.h"

#include <QtCore/qdebug.h>

#include "listcell.h"

class ListDelegatePrivate {
public:
    QVector<ListCellData*> dataset;
};

ListDelegate::ListDelegate()
    : d_ptr(new ListDelegatePrivate)
{

}

ListDelegate::~ListDelegate()
{
    Q_D(ListDelegate);
    Q_FOREACH(ListCellData* data, d->dataset) {
        delete data;
        data = nullptr;
    }
}

void ListDelegate::appendUser(const QString &username, const QString &selfSign)
{
    Q_D(ListDelegate);
    ListCellData* data = new ListCellData;
    data->username = username;
    data->selfSign = selfSign;
    d->dataset.push_back(data);
}

int ListDelegate::itemsSize() const
{
    Q_D(const ListDelegate);
    return d->dataset.size();
}

QWidget *ListDelegate::createItemWidget()
{
    return new ListCell();
}

void ListDelegate::setItemWidgetByIndex(int index, QWidget *itemWidget)
{
    ListCell* cell = static_cast<ListCell*>(itemWidget);
    if(nullptr == cell) {
        qDebug() << "list cell is nullptr of " << index;
        return;
    }

    Q_D(ListDelegate);
    if(index > d->dataset.size()) {
        qDebug() << "list size is smaller than " << index;
        return;
    }

    ListCellData* cellData = d->dataset[index];
    if(nullptr == cellData) {
        qDebug() << "cell's data is nullptr";
        return;
    }
    cell->init(cellData);
}
