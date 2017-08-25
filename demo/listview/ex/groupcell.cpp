#include "groupcell.h"
#include "ui_groupcell.h"

GroupCell::GroupCell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupCell)
{
    ui->setupUi(this);
}

GroupCell::~GroupCell()
{
    delete ui;
}
