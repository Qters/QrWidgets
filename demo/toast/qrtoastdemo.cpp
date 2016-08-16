#include "qrtoastdemo.h"
#include "ui_qrtoastdemo.h"

#include <QtWidgets/qdesktopwidget.h>

#include "qrtoast.h"

USING_NS_QRWIDGETS;


QrToastDemo::QrToastDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToastWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("demo - toast");

    connect(ui->pushButton, &QPushButton::clicked, [this](){
        QDesktopWidget *deskdop = QApplication::desktop();
        QrToast::instance()->show("Toast message",
                                  QPoint(deskdop->width()/2,
                                         deskdop->height()/2));
    });
}

QrToastDemo::~QrToastDemo()
{
    delete ui;
}
