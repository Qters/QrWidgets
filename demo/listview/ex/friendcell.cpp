#include "friendcell.h"
#include "ui_friendcell.h"

#include "frienddelegate.h"

FriendCell::FriendCell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendCell)
{
    ui->setupUi(this);
}

FriendCell::~FriendCell()
{
    delete ui;
}

bool FriendCell::init(FriendDelegateData *data)
{
    ui->groupName->setText(data->groupName);
    ui->name->setText(data->username);
    ui->sign->setText(data->sign);

    if(! data->isGroupHead()) {
        ui->groupName->hide();
        ui->groupLine->hide();
    } else {
        ui->groupName->show();
        ui->groupLine->show();
    }

    return true;
}

int FriendCell::normalItemHeight() const
{
    return size().height()
            - ui->groupName->height()
            - ui->groupLine->height();
}

int FriendCell::headItemHeight() const
{
    return size().height();
}
