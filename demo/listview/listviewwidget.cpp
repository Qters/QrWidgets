#include "listviewwidget.h"
#include "ui_listviewwidget.h"

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
    QWidget(parent),
    ui(new Ui::ListviewWidget),
    d_ptr(new ListviewWidgetPrivate(this))
{
    ui->setupUi(this);

    ui->emptyTips->hide();

    Q_D(ListviewWidget);
    d->delegate = new ListDelegate();
    for(int i=0; i<100000; i++) {
        auto username = QString("User %1").arg(i);
        d->delegate->appendUser(username,
                             QString("Sign of %1.").arg(username));
    }
    ui->listview->setDelegate(d->delegate);

    d->cellInfo = new ListCellInfo(this);
    d->cellInfo->hide();
    d->cellInfo->setWindowFlags(Qt::Window);
    connect(d->delegate, &ListDelegate::showInfo, [this](ListCellData data){
        Q_D(ListviewWidget);
        d->cellInfo->init(data.username, data.selfSign);
        d->cellInfo->show();
    });
    connect(d->delegate, &ListDelegate::dataFiltered, [this](bool isEmpty){
        if(isEmpty) {
            ui->emptyTips->setText("no one.");
            ui->emptyTips->show();
            ui->listview->hide();
        } else {
            ui->listview->show();
            ui->emptyTips->hide();
        }
    });
    connect(d->delegate, &ListDelegate::dataEmpty, [this](bool isEmpty){
        if(isEmpty) {
            ui->emptyTips->setText("empty");
            ui->emptyTips->show();
            ui->listview->hide();
        } else {
            ui->listview->show();
            ui->emptyTips->hide();
        }
    });

    connect(ui->filter, &QLineEdit::textChanged, [this](const QString &text){
        QRegExp filterReg(QString("*%1*").arg(text));
        filterReg.setCaseSensitivity(Qt::CaseInsensitive);
        filterReg.setPatternSyntax(QRegExp::Wildcard);

        Q_D(ListviewWidget);
        d->delegate->filter([filterReg](QrListViewData* _data) -> bool{
            ListCellData* data = static_cast<ListCellData*>(_data);
            if(nullptr == data) {
                return false;
            }
            return filterReg.exactMatch(data->username);
        });
    });
}

ListviewWidget::~ListviewWidget()
{
    delete ui;
}
