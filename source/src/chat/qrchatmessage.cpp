#include "chat/qrchatmessage.h"
#include "ui_qrchatmessage.h"

USING_NS_QRWIDGETS;

QrChatMessage::QrChatMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QrChatMessage)
{
    ui->setupUi(this);

    ui->message->setAnchorDirection(QrMsgBubble::AnchorDirection::LeftUp);
    ui->image->setStyleSheet("width:30px; height:30px;");
    ui->username->setStyleSheet("color:gray;");
}

QrChatMessage::~QrChatMessage()
{
    delete ui;
}

void QrChatMessage::setName(const QString &text)
{
    ui->username->setText(text);
}

void QrChatMessage::setLogo(const QPixmap &logo)
{
    ui->image->setPixmap(logo);
}

void QrChatMessage::setMessage(const QString &text)
{
    ui->message->setText(text);
}


