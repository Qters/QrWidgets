#include "chatwidget.h"
#include "ui_chatwidget.h"

#include <QtWidgets/qstyle.h>

#include "qrutf8.h"

USING_NS_QRWIDGETS;

ChatWidget::ChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);

    QString text1 = "当一个程序启动时，就有一个进程被操作系统（OS）创建，与此同时一个线程也立刻运行，该线程通常叫做程序的主线程（Main Thread）";

    QString text2 = "当一个程序启动时，就有一个进程被操作系统（OS）创建，与此同时一个线程也立刻运行，该线程通常叫做程序的主线程（Main Thread），因为它是程序开始时就执行的，如果你需要再创建线程，那么创建的线程就是这个主线程的子线程。";

    QString text3 = "当一个程序启动时，就有一个进程被操作系统（OS）创建，与此同时一个线程也立刻运行，该线程通常叫做程序的主线程（Main Thread），因为它是程序开始时就执行的，如果你需要再创建线程，那么创建的线程就是这个主线程的子线程。每个进程至少都有一个主线程，在Winform中，应该就是创建GUI的线程。";

    QString text4 = "当一个程序启动时，就有一个进程被操作系统（OS）创建";

    ui->bubble1->setLogo(style()->standardPixmap(QStyle::SP_DirIcon));
    ui->bubble1->setName("Daniel");
    ui->bubble1->setMessage(text1);

    ui->bubble2->setLogo(style()->standardPixmap(QStyle::SP_FileIcon));
    ui->bubble2->setName("Emma");
    ui->bubble2->setMessage(text2);

    ui->bubble3->setLogo(style()->standardPixmap(QStyle::SP_VistaShield));
    ui->bubble3->setName("Suglas");
    ui->bubble3->setMessage(text3);

    ui->bubble4->setLogo(style()->standardPixmap(QStyle::SP_MediaPlay));
    ui->bubble4->setName("Alex");
    ui->bubble4->setMessage(text4);
}

ChatWidget::~ChatWidget()
{
    delete ui;
}
