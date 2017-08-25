#include "listviewwithemptytips.h"
#include "ui_listviewwithemptytips.h"

#include "listgroupdelegate.h"

USING_NS_QRWIDGETS;

class ListviewWithEmptyTipsPrivate {
    QR_DECLARE_PUBLIC(ListviewWithEmptyTips)

public:
    ListviewWithEmptyTipsPrivate(ListviewWithEmptyTips * q);

public:
    QString dataEmptyTips;
};

ListviewWithEmptyTipsPrivate::ListviewWithEmptyTipsPrivate(ListviewWithEmptyTips *q)
    : q_ptr(q) {}

////////////////////////////////////////////////////

ListviewWithEmptyTips::ListviewWithEmptyTips(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListviewWithEmptyTips),
    d_ptr(new ListviewWithEmptyTipsPrivate(this))
{
    ui->setupUi(this);

    ui->emptyTips->hide();
//    ui->listview->hide();

    connect(ui->listview->delegate(), &ListGroupDelegate::dataEmpty, [this](bool isEmpty){
        Q_D(const ListviewWithEmptyTips);
        if(isEmpty) {
            ui->emptyTips->setText(d->dataEmptyTips);
            ui->emptyTips->show();
            ui->listview->hide();
        } else {
            ui->listview->show();
            ui->emptyTips->hide();
        }
    });
}

ListviewWithEmptyTips::~ListviewWithEmptyTips()
{
    delete ui;
}

void ListviewWithEmptyTips::setDataEmptyTips(const QString &tips)
{
    Q_D(ListviewWithEmptyTips);
    d->dataEmptyTips = tips;
    ui->emptyTips->setText(tips);
}

Qters::QrWidgets::QrListView *ListviewWithEmptyTips::listview()
{
    return ui->listview;
}
