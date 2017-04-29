﻿#include "waitingspinnerwidget.h"
#include "ui_waitingspinnerwidget.h"

#include <QtCore/qtimer.h>
#include <QtCore/qeventloop.h>

#include "qrwaitingspinnerwidget.h"

USING_NS_QRWIDGETS;

WaitingSpinnerWidget::WaitingSpinnerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitingSpinnerWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("demo - waiting spinner");

    connect(ui->pushButton, &QPushButton::clicked, [this](){
        QrWaitingSpinnerWidget waiting(this);
        waiting.defaultSetting();
        waiting.start("loading...");

        QTimer loadingTime;
        loadingTime.start(4000);
        connect(&loadingTime, &QTimer::timeout, [this](){
            emit this->timeout();
        });

        QEventLoop eventloop;
        connect(this, &WaitingSpinnerWidget::timeout,
                         &eventloop, &QEventLoop::quit);
        eventloop.exec();

        waiting.stop();
    });
}

WaitingSpinnerWidget::~WaitingSpinnerWidget()
{
    delete ui;
}
