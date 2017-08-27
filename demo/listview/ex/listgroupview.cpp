#include "listgroupview.h"

#include <QtCore/qdebug.h>
#include <QtWidgets/qscrollbar.h>
#include <QtCore/qqueue.h>

#include "listgroupdelegate.h"
#include "listgroupwidget.h"

class ListGroupViewPrivate {
    QR_DECLARE_PUBLIC(ListGroupView)

public:
    ListGroupViewPrivate(ListGroupView* q) : q_ptr(q) {}

public:
    int preRenderItemBeginIndex = 0;
};

USING_NS_QRWIDGETS;

ListGroupView::ListGroupView(QWidget *parent)
    : QrListView(parent),
      d_ptr(new ListGroupViewPrivate(this))
{

}

ListGroupView::~ListGroupView()
{

}

void ListGroupView::setDelegate(ListGroupDelegate *delegate)
{
    QrListView::setDelegate(static_cast<QrListVidewDelegate*>(delegate));
}

ListGroupDelegate *ListGroupView::delegate()
{
    return static_cast<ListGroupDelegate*>(QrListView::delegate());
}

void ListGroupView::dateChangeUpdate()
{
    Q_D(ListGroupView);

    d->preRenderItemBeginIndex = dataChangeUpdaetImpl();

    if(d->preRenderItemBeginIndex > delegate()->viewDataset().size()) {
        dataChangeUpdaetImpl();
    }
}

QWidget *ListGroupView::createWidget()
{
    ListGroupWidget *widget = static_cast<ListGroupWidget*>(QrListView::createWidget());
    if(nullptr == widget) {
        qWarning("create nullptr widget");
        return nullptr;
    }
    connect(widget, &ListGroupWidget::onGroupHeadClick, [this, widget](){
        return; //  function is not finish.

        ListGroupData *data = static_cast<ListGroupData *>(getDataOfWidget(widget));
        if(nullptr == data) {
            qDebug() << "group head's data is nullptr.";
            return;
        }

        delegate()->showGroupItems(data->groupHex(), ! data->isVisible());

        verticalScrollBar()->setRange(0, delegate()->verScrollBarRangeMaxValue(0));

        emit delegate()->dataChanged();
    });
    return widget;
}

int ListGroupView::dataChangeUpdaetImpl()
{
    Q_D(ListGroupView);

    int vscorllbarValue = verticalScrollBar()->value();
    int curItemHeight = delegate()->groupItemRenderHeight(
                delegate()->getDataByIndex(
                    getWidgetItemIndex(itemWidgets().first())));
    int currentItemIndex = vscorllbarValue / curItemHeight;
    int itemWidgetOffset =(vscorllbarValue % curItemHeight) * -1;

    if(! isScrollUp() && d->preRenderItemBeginIndex > delegate()->itemsSize()) {
        //  fix data
        int listviewHeight = height();

        ListGroupData *firstData = nullptr;
        QVector<QrListViewData *> datas = delegate()->viewDataset();
        int dataIdx = datas.size()-1;
        for(; dataIdx>=0; --dataIdx) {
            if(listviewHeight <= 0) {
                break;
            }
            ListGroupData *data = static_cast<ListGroupData *>(datas.at(dataIdx));
            if(nullptr == data) {
                continue;
            }
            firstData = data;
            listviewHeight -= delegate()->groupItemRenderHeight(data);
        }
        if(nullptr != firstData) {
            curItemHeight = delegate()->groupItemRenderHeight(firstData);
            currentItemIndex = dataIdx + 2;
        }
    }

    clearDataWidget();
    Q_FOREACH(QWidget* _itemWidget, itemWidgets()) {
        ListGroupWidget *itemWidget = static_cast<ListGroupWidget*>(_itemWidget);
        if(nullptr == itemWidget) {
            continue;
        }

        itemWidget->move(0, itemWidgetOffset);

        ListGroupData *delegateData = nullptr;
        for(; currentItemIndex<delegate()->itemsSize(); ++currentItemIndex) {
            delegateData = static_cast<ListGroupData*>(delegate()->getDataByIndex(currentItemIndex));

            if(nullptr == delegateData) {
                continue;
            }

            if(! delegateData->isVisible()
                    && ! delegateData->isGroupHead()) { //  head's part show, content's part hide
                continue;
            }

            if(0 != currentItemIndex && delegateData->isGroupHead()) {
                itemWidgetOffset += delegate()->groupMargin();
                itemWidget->move(0, itemWidgetOffset);
            }

            delegate()->setItemWidgetByIndex(currentItemIndex, itemWidget);
            setDataWidget(delegateData, itemWidget);
            itemWidget->show();
            break;
        }
        if(nullptr == delegateData) {
            itemWidget->hide();
        }
        setWidgetItemIndex(itemWidget, currentItemIndex);

        if(nullptr != delegateData) {
            itemWidgetOffset += delegate()->groupItemRenderHeight(delegateData);
        }
        currentItemIndex += 1;
    }

    return currentItemIndex;
}
