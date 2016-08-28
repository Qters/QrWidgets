#include "qrwidgetdemo.h"
#include "ui_qrwidgetdemo.h"

#include "toast/qrtoastdemo.h"
#include "waitingspinner/waitingspinnerwidget.h"
#include "emailinputter/emailinputterwidget.h"
#include "layout/queuelayout/controlwidget.h"

QrWidgetDemo::QrWidgetDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QrWidgetDemo)
{
    ui->setupUi(this);

    auto widgetDoModelFunc = [this](QWidget *widget){
        QEventLoop eventloop;
        QObject::connect(widget, &QWidget::destroyed,
                         &eventloop, &QEventLoop::quit);
        eventloop.exec();
    };

    connect(ui->waitingspinner, &QPushButton::clicked, [this, widgetDoModelFunc](){
        WaitingSpinnerWidget waitingspinner;
//        waitingspinner.setAttribute( Qt::WA_DeleteOnClose );
        waitingspinner.show();

        widgetDoModelFunc(&waitingspinner);
    });
    connect(ui->emailinputter, &QPushButton::clicked, [this, widgetDoModelFunc](){
        EmailInputterWidget emailInputter;
        emailInputter.show();

        widgetDoModelFunc(&emailInputter);

    });
    connect(ui->toast, &QPushButton::clicked, [this, widgetDoModelFunc](){
        QrToastDemo toastDemo;
        toastDemo.show();

        widgetDoModelFunc(&toastDemo);
    });
    connect(ui->queuelayout, &QPushButton::clicked, [this, widgetDoModelFunc](){
        QueueControlWidget queueLayoutDemo;
        queueLayoutDemo.show();

        widgetDoModelFunc(&queueLayoutDemo);
    });
}

QrWidgetDemo::~QrWidgetDemo()
{
    delete ui;
}
