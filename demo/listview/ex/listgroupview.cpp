#include "listgroupview.h"

#include <QtCore/qdebug.h>
#include <QtWidgets/qscrollbar.h>
#include <QtCore/qqueue.h>

#include "listgroupdelegate.h"

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
