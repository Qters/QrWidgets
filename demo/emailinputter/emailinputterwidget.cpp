#include "emailinputterwidget.h"
#include "ui_emailinputterwidget.h"

#include <QtCore/qdebug.h>

#include <QtGui/qstandarditemmodel.h>

EmailInputterWidget::EmailInputterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmailInputterWidget)
{
    ui->setupUi(this);

    QStandardItemModel *listData = new QStandardItemModel();

    QStandardItem *rootItem = listData->invisibleRootItem();
    QStandardItem *item1 = new QStandardItem("yuyj@fsmeeting.com");
    rootItem->appendRow(item1);
    QStandardItem *item2 = new QStandardItem("chensy@fsmeeting.com");
    rootItem->appendRow(item2);
    QStandardItem *item3 = new QStandardItem("chenhk@fsmeeting.com");
    rootItem->appendRow(item3);
    QStandardItem *item4 = new QStandardItem("adam@fsmeeting.com");
    rootItem->appendRow(item4);

    ui->textEdit->showDropDownList(listData);
}

EmailInputterWidget::~EmailInputterWidget()
{
    delete ui;
}
