#include "qrtoastdemo.h"
#include "ui_qrtoastdemo.h"

#include "qrtoast.h"

USING_NS_QRWIDGETS;


QrToastDemo::QrToastDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToastWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("demo - toast");

    connect(ui->pushButton, &QPushButton::clicked, [this](){
        QrToast::instance()->show("Toast message",
                                  ui->pushButton->mapToGlobal(ui->pushButton->pos()));
    });
}

QrToastDemo::~QrToastDemo()
{
    delete ui;
}
