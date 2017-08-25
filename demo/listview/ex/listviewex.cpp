#include "listviewex.h"

#include <QtCore/qdebug.h>
#include <QtWidgets/qscrollbar.h>
#include <QtCore/qqueue.h>

#include "listviewexdelegate.h"

class ListviewExPrivate {
    QR_DECLARE_PUBLIC(ListviewEx)

public:
    ListviewExPrivate(ListviewEx* q) : q_ptr(q) {}

public:
    int preRenderItemBeginIndex = 0;
};

USING_NS_QRWIDGETS;

ListviewEx::ListviewEx(QWidget *parent)
    : QrListView(parent),
      d_ptr(new ListviewExPrivate(this))
{

}

ListviewEx::~ListviewEx()
{

}

void ListviewEx::setDelegate(ListviewExDelegate *delegate)
{
    QrListView::setDelegate(static_cast<QrListVidewDelegate*>(delegate));
}

ListviewExDelegate *ListviewEx::delegate()
{
    return static_cast<ListviewExDelegate*>(QrListView::delegate());
}

void ListviewEx::dateChangeUpdate()
{
    Q_D(ListviewEx);

    d->preRenderItemBeginIndex = dataChangeUpdaetImpl();

    if(d->preRenderItemBeginIndex > delegate()->viewDataset().size()) {
        dataChangeUpdaetImpl();
    }
}

int ListviewEx::dataChangeUpdaetImpl()
{
    Q_D(ListviewEx);

    int vscorllbarValue = verticalScrollBar()->value();
    int curItemHeight = delegate()->groupItemRenderHeight(
                delegate()->getDataByIndex(
                    getWidgetItemIndex(itemWidgets().first())));
    int currentItemIndex = vscorllbarValue / curItemHeight;
    int itemWidgetOffset =(vscorllbarValue % curItemHeight) * -1;

    if(! isScrollUp() && d->preRenderItemBeginIndex > delegate()->itemsSize()) {
        //  fix data
        int listviewHeight = height();

        ListviewExData *firstData = nullptr;
        QVector<QrListViewData *> datas = delegate()->viewDataset();
        int dataIdx = datas.size()-1;
        for(; dataIdx>=0; --dataIdx) {
            if(listviewHeight <= 0) {
                break;
            }
            ListviewExData *data = static_cast<ListviewExData *>(datas.at(dataIdx));
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
    Q_FOREACH(QWidget* itemWidget, itemWidgets()) {
        itemWidget->move(0, itemWidgetOffset);
        setWidgetItemIndex(itemWidget, currentItemIndex);

        QrListViewData* delegateData = nullptr;
        if(currentItemIndex < delegate()->itemsSize()) {
            delegateData = delegate()->setItemWidgetByIndex(currentItemIndex, itemWidget);
            if(nullptr != delegateData) {
                setDataWidget(delegateData, itemWidget);
            }
            itemWidget->show();
        } else {
            itemWidget->hide();
        }

        if(nullptr != delegateData && delegateData->isGroupData()) {
            itemWidgetOffset += delegate()->groupItemRenderHeight(delegateData);
        }
        currentItemIndex += 1;
    }

    return currentItemIndex;
}
