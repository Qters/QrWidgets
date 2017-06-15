#include "listcell.h"
#include "ui_listcell.h"

#include "listdelegate.h"

USING_NS_QRWIDGETS;

ListCell::ListCell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListCell)
{
    ui->setupUi(this);

    changeStyle(Style::Right);
}

ListCell::~ListCell()
{
    delete ui;
}

bool ListCell::init(ListCellData *data)
{
    ui->name->setText(data->username);
    ui->selfSign_right->setText(data->selfSign);
    ui->selfSign_down->setText(data->selfSign);

    setToolTip(QString("I'm %1").arg(data->username));

    return true;
}

void ListCell::changeStyle(ListCell::Style style)
{
    switch(style) {
    case ListCell::Right:
        ui->selfSign_down->hide();
        ui->selfSign_right->show();
        break;
    case ListCell::Down:
        ui->selfSign_down->show();
        ui->selfSign_right->hide();
        break;
    }
}
