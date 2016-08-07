#include "qrwidgetdemo.h"
#include "ui_qrwidgetdemo.h"

#include "waitingspinner/waitingspinnerwidget.h"
#include "emailinputter/emailinputterwidget.h"

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
        waitingspinner.setAttribute( Qt::WA_DeleteOnClose );
        waitingspinner.show();

        widgetDoModelFunc(&waitingspinner);
    });
    connect(ui->emailinputter, &QPushButton::clicked, [this, widgetDoModelFunc](){
        EmailInputterWidget emailInputter;
        emailInputter.show();

        widgetDoModelFunc(&emailInputter);

    });
}

QrWidgetDemo::~QrWidgetDemo()
{
    delete ui;
}
