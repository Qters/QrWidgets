#include "listcell.h"
#include "ui_listcell.h"

#include "listdelegate.h"

USING_NS_QRWIDGETS;

ListCell::ListCell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListCell)
{
    ui->setupUi(this);
}

ListCell::~ListCell()
{
    delete ui;
}

bool ListCell::init(ListCellData *data)
{
    ui->name->setText(data->username);
    ui->selfSign->setText(data->selfSign);

    setToolTip(QString("I'm %1").arg(data->username));

    return true;
}
