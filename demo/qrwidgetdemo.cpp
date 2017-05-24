#include "qrwidgetdemo.h"
#include "ui_qrwidgetdemo.h"

#include "toast/qrtoastdemo.h"
#include "waitingspinner/waitingspinnerwidget.h"
#include "emailinputter/emailinputterwidget.h"
#include "layout/queuelayout/queuecontrolwidget.h"
#include "editablelabel/editablelabelwidget.h"
#include "titlebar/qrtitlebardemo.h"

#include "contact/contactdata.h"
#include "contact/contactpage.h"

#include "chat/chatwidget.h"
#include "listview/listviewwidget.h"

QrWidgetDemo::QrWidgetDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QrWidgetDemo)
{
    ui->setupUi(this);

    setWindowTitle("QrWidgets Demo");

    auto widgetDoModelFunc = [this](QWidget *widget){
        QEventLoop eventloop;
        connect(widget, &QWidget::destroyed,
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
    connect(ui->editableLabel, &QPushButton::clicked, [this, widgetDoModelFunc](){
        EditableLabelWidget editableLabel;
        editableLabel.show();

        widgetDoModelFunc(&editableLabel);
    });
    connect(ui->titlebar, &QPushButton::clicked, [this, widgetDoModelFunc](){
        QrTitlebarDemo titlebarDemo;
        titlebarDemo.show();

        widgetDoModelFunc(&titlebarDemo);
    });
    connect(ui->contact, &QPushButton::clicked, [this, widgetDoModelFunc](){
        ContactData member1;
        member1.groupid = 1;
        member1.groupName = "Group1";
        member1.name = "Alias";
        member1.level = 5;

        ContactData member2;
        member2.groupid = 1;
        member2.groupName = "Group1";
        member2.name = "Bez";
        member2.level = 3;

        ContactData member3;
        member3.groupid = 1;
        member3.groupName = "Group1";
        member3.name = "Duglas";
        member3.level = 1;

        ContactData member4;
        member4.groupid = 2;
        member4.groupName = "Group2";
        member4.name = "Dan";
        member4.level = 2;

        ContactData member5;
        member5.groupid = 2;
        member5.groupName = "Group2";
        member5.name = "Olio";
        member5.level = 3;

        ContactPage contactPage;
        contactPage.init(QVector<ContactData>()
                << member1
                 <<member2
                 <<member3
                 <<member4
                 <<member5);
        contactPage.show();

        widgetDoModelFunc(&contactPage);
    });
    connect(ui->chat, &QPushButton::clicked, [this, widgetDoModelFunc](){
        ChatWidget chatDemo;
        chatDemo.show();

        widgetDoModelFunc(&chatDemo);
    });
    connect(ui->listview, &QPushButton::clicked, [this, widgetDoModelFunc](){
        ListviewWidget listviewDemo;
        listviewDemo.show();

        widgetDoModelFunc(&listviewDemo);
    });
}

QrWidgetDemo::~QrWidgetDemo()
{
    delete ui;
}
