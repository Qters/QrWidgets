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
        QrToast::instance()->showInScreen("Screen Toast message");
        QrToast::instance()->showInWidget(this, "Widget Toast message");
    });
}

QrToastDemo::~QrToastDemo()
{
    delete ui;
}
