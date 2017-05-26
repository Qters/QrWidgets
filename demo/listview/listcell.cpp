#include "listcell.h"
#include "ui_listcell.h"

#include "listdelegate.h"

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

void ListCell::init(ListCellData *data)
{
    ui->name->setText(data->username);
    ui->selfSign->setText(data->selfSign);
    setToolTip(data->tooltip);
}
