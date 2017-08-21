#include "qrsearchlistview.h"
#include "ui_qrsearchlistview.h"

#include <QtCore/qdebug.h>
#include <QtWidgets/qmenu.h>

#include "listview/qrlistdelegate.h"

NS_QRWIDGETS_BEGIN

class QrSearchListviewPrivate {
    QR_DECLARE_PUBLIC(QrSearchListview)

public:
    QString dataEmptyTips;
    QString filterEmptyTips;
    std::function<bool (QString, QrListViewData*)> filterFunc;

public:
    QrSearchListviewPrivate(QrSearchListview* q);
};

QrSearchListviewPrivate::QrSearchListviewPrivate(QrSearchListview *q)
    : q_ptr(q)
{}


NS_QRWIDGETS_END

USING_NS_QRWIDGETS;

QrSearchListview::QrSearchListview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QrSearchListview),
    d_ptr(new QrSearchListviewPrivate(this))
{
    ui->setupUi(this);

    ui->emptyTips->hide();

    connect(ui->filter, &QLineEdit::textChanged, [this](const QString &text){
        if(nullptr == ui->listview->delegate()) {
            qDebug() << "filter is not execute, listview's delegate is nullptr.";
            return;
        }

        Q_D(QrSearchListview);
        std::function<bool (QString, QrListViewData*)> filterFunc = d->filterFunc;
        ui->listview->delegate()->filter([text, filterFunc](QrListViewData* _data){
            return filterFunc(text, _data);
        });
    });
}

QrSearchListview::~QrSearchListview()
{
    delete ui;
}

void QrSearchListview::setDelegate(QrListVidewDelegate *delegate,
                                   std::function<bool (QString, QrListViewData*)> filterFunc)
{
    ui->listview->setDelegate(delegate);
    connect(delegate, &QrListVidewDelegate::dataFiltered, [this](bool isEmpty){
        Q_D(const QrSearchListview);
        if(isEmpty) {
            ui->emptyTips->setText(d->filterEmptyTips);
            ui->emptyTips->show();
            ui->listview->hide();
        } else {
            ui->listview->show();
            ui->emptyTips->hide();
        }
    });
    connect(delegate, &QrListVidewDelegate::dataEmpty, [this](bool isEmpty){
        Q_D(const QrSearchListview);
        if(isEmpty) {
            ui->emptyTips->setText(d->dataEmptyTips);
            ui->emptyTips->show();
            ui->listview->hide();
        } else {
            ui->listview->show();
            ui->emptyTips->hide();
        }
    });

    Q_D(QrSearchListview);
    d->filterFunc = filterFunc;
}

void QrSearchListview::setDataEmptyTips(const QString &tips)
{
    Q_D(QrSearchListview);
    d->dataEmptyTips = tips;
}

void QrSearchListview::setFilterEmptyTips(const QString &tips)
{
    Q_D(QrSearchListview);
    d->filterEmptyTips = tips;
}
