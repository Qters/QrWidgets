#include "qrtitlebardemo.h"
#include "ui_qrtitlebardemo.h"

#include <QtWidgets/qstyle.h>

QrTitlebarDemo::QrTitlebarDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QrTitlebarDemo)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    ui->titlebar->defaultButtonStyle();
    ui->titlebar->setLogo(style()->standardPixmap(QStyle::SP_DirIcon));
    ui->titlebar->setTitle("Title");
}

QrTitlebarDemo::~QrTitlebarDemo()
{
    delete ui;
}
