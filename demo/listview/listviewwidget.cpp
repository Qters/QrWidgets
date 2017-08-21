#include "listviewwidget.h"

#include <QtCore/qdebug.h>
#include <QtWidgets/qmenu.h>

#include "listdelegate.h"
#include "listcellinfo.h"

class ListviewWidgetPrivate {
    QR_DECLARE_PUBLIC(ListviewWidget)

public:
    ListDelegate* delegate = nullptr;
    ListCellInfo *cellInfo = nullptr;

public:
    ListviewWidgetPrivate(ListviewWidget* q);
};

ListviewWidgetPrivate::ListviewWidgetPrivate(ListviewWidget *q)
    : q_ptr(q)
{}


///////////////////////////////////////////////////////////////

USING_NS_QRWIDGETS;

ListviewWidget::ListviewWidget(QWidget *parent) :
    QrSearchListview(parent),
    d_ptr(new ListviewWidgetPrivate(this))
{
    Q_D(ListviewWidget);
    d->delegate = new ListDelegate();
    for(int i=0; i<100000; i++) {
        auto username = QString("User %1").arg(i);
        d->delegate->appendUser(username,
                             QString("Sign of %1.").arg(username));
    }
    setDelegate(d->delegate, [](QString text, QrListViewData* _data) {
        QRegExp filterReg(QString("*%1*").arg(text));
        filterReg.setCaseSensitivity(Qt::CaseInsensitive);
        filterReg.setPatternSyntax(QRegExp::Wildcard);

        ListCellData* data = static_cast<ListCellData*>(_data);
        if(nullptr == data) {
            return false;
        }
        return filterReg.exactMatch(data->username);
    });

    d->cellInfo = new ListCellInfo(this);
    d->cellInfo->hide();
    d->cellInfo->setWindowFlags(Qt::Window);
    connect(d->delegate, &ListDelegate::showInfo, [this](ListCellData data){
        Q_D(ListviewWidget);
        d->cellInfo->init(data.username, data.selfSign);
        d->cellInfo->show();
    });
}

ListviewWidget::~ListviewWidget()
{
}
