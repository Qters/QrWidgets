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

bool GroupCell::initHeadWidget(ListGroupData *data)
{
    Q_UNUSED(data);
    return true;
}

bool GroupCell::initNormalWidget(ListGroupData *data)
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

int GroupCell::normalItemHeight() const
{
    return size().height();
}

int GroupCell::headItemHeight() const
{
    return size().height();
}
