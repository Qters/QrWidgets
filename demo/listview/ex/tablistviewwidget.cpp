#include "tablistviewwidget.h"
#include "ui_tablistviewwidget.h"

#include "listview/qrlistview.h"

#include "frienddelegate.h"
#include "groupdelegate.h"

class TabListviewWidgetPrivate {
    QR_DECLARE_PUBLIC(TabListviewWidget)

public:
    TabListviewWidgetPrivate(TabListviewWidget *q);

public:
    FriendDelegate friendDelegate;
    GroupDelegate groupDelegate;
};

TabListviewWidgetPrivate::TabListviewWidgetPrivate(TabListviewWidget *q)
    : q_ptr(q)
{}


TabListviewWidget::TabListviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabListviewWidget),
    d_ptr(new TabListviewWidgetPrivate(this))
{
    ui->setupUi(this);

    ui->friends->setDataEmptyTips("No Friends");
    ui->groups->setDataEmptyTips("No Groups");

    Q_D(TabListviewWidget);
    ui->friends->listview()->setDelegate(&(d->friendDelegate));
    ui->groups->listview()->setDelegate(&(d->groupDelegate));

    QMap<QString, QString> groupNames;
    groupNames["0"] = "zero";
    groupNames["1"] = "first";
    groupNames["2"] = "second";
    groupNames["3"] = "three";
    groupNames["4"] = "four";
    groupNames["5"] = "five";
    groupNames["6"] = "six";
    groupNames["7"] = "seven";
    groupNames["8"] = "eight";
    groupNames["9"] = "nine";
    groupNames["10"] = "ten";
    for(int i=0; i<1000; i++) {
        QString groupId = QString::number(i).left(1);
        d->friendDelegate.appendFriend(groupId.toInt(), groupNames[groupId],
                QString("%1's name").arg(i),
                QString("hahaha, i'm %1").arg(i));
    }
    d->friendDelegate.sort();
}

TabListviewWidget::~TabListviewWidget()
{
    delete ui;
}

ListviewWithEmptyTips *TabListviewWidget::friends()
{
    return ui->friends;
}

