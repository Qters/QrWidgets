#include "friendcell.h"
#include "ui_friendcell.h"

#include <QtCore/qdebug.h>

#include "frienddelegate.h"

USING_NS_QRWIDGETS;

FriendCell::FriendCell(QWidget *parent) :
    ListGroupWidget(parent),
    ui(new Ui::FriendCell)
{
    ui->setupUi(this);

    connect(ui->groupName, &QrLabel::clicked, this, &FriendCell::onGroupHeadClick);
}

FriendCell::~FriendCell()
{
    delete ui;
}

bool FriendCell::initHeadWidget(ListGroupData *_data)
{
    FriendDelegateData* data = static_cast<FriendDelegateData*>(_data);
    if(nullptr == data) {
        qDebug() << "friend cell's data is nullptr.";
        return false;
    }
    ui->groupName->setText(data->groupName);
    ui->name->setText(data->username);
    ui->sign->setText(data->sign);

    ui->groupName->show();
    ui->groupLine->show();

    return true;
}

bool FriendCell::initOnlyHeadWidget(ListGroupData *data)
{
    if(! initHeadWidget(data)) {
        return false;
    }

    for(int idx=0; idx<ui->contents->count(); ++idx) {
        QWidget* contentWidget = ui->contents->itemAt(idx)->widget();
        if(nullptr != contentWidget) {
            contentWidget->hide();
        }
    }

    return true;
}

bool FriendCell::initNormalWidget(ListGroupData *_data)
{
    FriendDelegateData* data = static_cast<FriendDelegateData*>(_data);
    if(nullptr == data) {
        qDebug() << "friend cell's data is nullptr.";
        return false;
    }
    ui->groupName->setText(data->groupName);
    ui->name->setText(data->username);
    ui->sign->setText(data->sign);

    ui->groupName->hide();
    ui->groupLine->hide();

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
