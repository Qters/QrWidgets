#include "widget.h"
#include "ui_widget.h"

#include <QtCore/qtimer.h>
#include <QtCore/qeventloop.h>

#include "qrwaitingspinnerwidget.h"

USING_NS_QRWIDGETS;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("demo - waiting spinner");

    connect(ui->pushButton, &QPushButton::clicked, [this](){
        QrWaitingSpinnerWidget waiting(this);
        waiting.defaultSetting();
        waiting.start();

        QTimer loadingTime;
        loadingTime.start(4000);
        QObject::connect(&loadingTime, &QTimer::timeout, [this](){
            emit this->timeout();
        });

        QEventLoop eventloop;
        QObject::connect(this, &Widget::timeout,
                         &eventloop, &QEventLoop::quit);
        eventloop.exec();

        waiting.stop();
    });
}

Widget::~Widget()
{
    delete ui;
}
