#include "qrchatmessage.h"
#include "ui_qrchatmessage.h"

QrChatMessage::QrChatMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QrChatMessage)
{
    ui->setupUi(this);
}

QrChatMessage::~QrChatMessage()
{
    delete ui;
}
