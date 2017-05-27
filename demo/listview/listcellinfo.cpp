#include "listcellinfo.h"
#include "ui_listcellinfo.h"

ListCellInfo::ListCellInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListCellInfo)
{
    ui->setupUi(this);
}

ListCellInfo::~ListCellInfo()
{
    delete ui;
}

void ListCellInfo::init(const QString &username, const QString &selfsign)
{
    ui->username->setText(username);
    ui->selfsign->setText(selfsign);
}
