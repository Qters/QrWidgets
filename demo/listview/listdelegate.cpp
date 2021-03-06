#include "listdelegate.h"

#include <QtCore/qdebug.h>
#include <QtWidgets/qmenu.h>

#include "listcell.h"
#include "listcellinfo.h"


USING_NS_QRWIDGETS;

ListDelegate::ListDelegate()
    : QrListVidewDelegate()
{
}

void ListDelegate::initMenu(QMenu *menu)
{
    QAction* infoAction = menu->addAction("info");
    connect(infoAction, &QAction::triggered, [this](bool checked){
        Q_UNUSED(checked);

        ListCellData* data = static_cast<ListCellData*>(menuData());
        if(nullptr == data) {
            qDebug() << "info action's data is null";
            return;
        }
        emit showInfo(*data);
    });
}

bool ListDelegate::hasMenu() const
{
    return true;
}

void ListDelegate::appendUser(const QString &username, const QString &selfSign)
{
    ListCellData* data = new ListCellData;
    data->username = username;
    data->selfSign = selfSign;
    addData(data);
}

QWidget *ListDelegate::createItemWidget()
{
    return new ListCell();
}

void ListDelegate::setItemWidgetByData(QrListViewData *_data, QWidget *_itemWidget)
{
    ListCell *itemWidget = static_cast<ListCell *>(_itemWidget);
    if(nullptr == itemWidget) {
        return;
    }

    ListCellData *data = static_cast<ListCellData *>(_data);
    if(nullptr == data) {
        return;
    }

    itemWidget->init(data);
    itemWidget->changeStyle(data->isOnClick() ? ListCell::Style::Down : ListCell::Style::Right);
}

void ListDelegate::onDoubleClick(QWidget *item, QrListViewData *_data, bool click)
{
    Q_UNUSED(item);
    Q_UNUSED(click);

    ListCellData* data = static_cast<ListCellData*>(_data);
    if(nullptr == data) {
        qDebug() << "onDoubleClick's data is null";
        return;
    }
    emit showInfo(*data);
}

void ListDelegate::onClick(QWidget *item, QrListViewData *data, bool click)
{
    Q_UNUSED(data);

    ListCell *itemWidget = static_cast<ListCell *>(item);
    if(nullptr == itemWidget) {
        return;
    }

    itemWidget->changeStyle(click ? ListCell::Style::Down : ListCell::Style::Right);
}

///////////////////////////////////////////////////////

bool ListCellData::compare(const QrListViewData *_other) const
{
    const ListCellData* other = static_cast<const ListCellData*>(_other);
    if(nullptr == other) {
        return false;
    }
    return username.compare(other->username);
}
