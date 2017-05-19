#include "contactorwidget.h"
#include "ui_contactorwidget.h"

#include <QtCore/qdebug.h>

int ContactorWidget::count = 0;

ContactorWidget::ContactorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactorWidget)
{
    ++ count;
    qDebug() << "ContactorWidget:" << count;

    ui->setupUi(this);

    ui->level->setStyleSheet("color:blue;");

    init(0, "test");
}

ContactorWidget::~ContactorWidget()
{
    -- count;
    qDebug() << "~ContactorWidget:" << count;

    delete ui;
}

void ContactorWidget::init(int level, const QString &name)
{
    ui->level->setText(QString("Lv %1").arg(level));
    ui->name->setText(name);
}
