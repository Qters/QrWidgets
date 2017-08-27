#include "groupcell.h"
#include "ui_groupcell.h"

GroupCell::GroupCell(QWidget *parent) :
    ListGroupWidget(parent),
    ui(new Ui::GroupCell)
{
    ui->setupUi(this);
}

GroupCell::~GroupCell()
{
    delete ui;
}

bool GroupCell::initHeadPartWidget(ListGroupData *data)
{
    Q_UNUSED(data);
    return true;
}

bool GroupCell::initContentPartWidget(ListGroupData *data)
{
    Q_UNUSED(data);
    return true;
}

void GroupCell::showHeadWidget(bool show)
{
    Q_UNUSED(show);
}

void GroupCell::showContentWidget(bool show)
{
    Q_UNUSED(show);
}

int GroupCell::contentHeight() const
{
    return size().height();
}

int GroupCell::entireHeight() const
{
    return size().height();
}
