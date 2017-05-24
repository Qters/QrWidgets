#include "listviewwidget.h"
#include "ui_listviewwidget.h"

#include "listdelegate.h"

ListviewWidget::ListviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListviewWidget)
{
    ui->setupUi(this);

    delegate = new ListDelegate();
    for(int i=0; i<100000; i++) {
        auto username = QString("User %1").arg(i);
        delegate->appendUser(username,
                             QString("Sign of %1.").arg(username));
    }

    ui->listview->setDelegate(delegate);
}

ListviewWidget::~ListviewWidget()
{
    delete ui;
}
