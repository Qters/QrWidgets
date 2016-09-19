#include "emailinputterwidget.h"
#include "ui_emailinputterwidget.h"

#include <QtCore/qdebug.h>

#include <QtGui/qstandarditemmodel.h>

EmailInputterWidget::EmailInputterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmailInputterWidget)
{
    ui->setupUi(this);

    setWindowTitle("demo - email inputter");

    QStandardItemModel *listData = new QStandardItemModel();

    QStandardItem *rootItem = listData->invisibleRootItem();
    QStandardItem *item1 = new QStandardItem("iamyoer@gmail.com");
    rootItem->appendRow(item1);
    QStandardItem *item2 = new QStandardItem("iamyoer2@gmail.com");
    rootItem->appendRow(item2);
    QStandardItem *item3 = new QStandardItem("qters@gmail.com");
    rootItem->appendRow(item3);
    QStandardItem *item4 = new QStandardItem("iamyoer@hotmail.com");
    rootItem->appendRow(item4);

    ui->widget->setDDListData(listData);

    ui->widget->setMoveBy(this);
}

EmailInputterWidget::~EmailInputterWidget()
{
    delete ui;
}
