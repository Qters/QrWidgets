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

bool FriendCell::initHeadPartWidget(ListGroupData *_data)
{
    if(! ListGroupWidget::initHeadPartWidget(_data)) {
        return false;
    }

    FriendDelegateData* data = static_cast<FriendDelegateData*>(_data);
    if(nullptr == data) {
        qDebug() << "friend cell's data is nullptr.";
        return false;
    }
    ui->groupName->setText(data->groupName);

    return true;
}

bool FriendCell::initContentPartWidget(ListGroupData *_data)
{
    if(! ListGroupWidget::initContentPartWidget(_data)) {
        return false;
    }

    FriendDelegateData* data = static_cast<FriendDelegateData*>(_data);
    if(nullptr == data) {
        qDebug() << "friend cell's data is nullptr.";
        return false;
    }
    ui->name->setText(data->username);
    ui->sign->setText(data->sign);

    return true;
}

void FriendCell::showHeadWidget(bool show)
{
    if(show) {
        ui->groupName->show();
        ui->groupLine->show();
    } else {
        ui->groupName->hide();
        ui->groupLine->hide();
    }
}

void FriendCell::showContentWidget(bool show)
{
    for(int idx=0; idx<ui->contents->count(); ++idx) {
        QWidget* contentWidget = ui->contents->itemAt(idx)->widget();
        if(nullptr != contentWidget) {
            if(show) {
                contentWidget->show();
            } else {
                contentWidget->hide();
            }
        }
    }
}

int FriendCell::contentHeight() const
{
    return size().height()
            - ui->groupName->height()
            - ui->groupLine->height();
}

int FriendCell::entireHeight() const
{
    return size().height();
}
