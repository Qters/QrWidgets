#ifndef QRCHATMESSAGE_H
#define QRCHATMESSAGE_H

#include <QWidget>

namespace Ui {
class QrChatMessage;
}

class QrChatMessage : public QWidget
{
    Q_OBJECT

public:
    explicit QrChatMessage(QWidget *parent = 0);
    ~QrChatMessage();

private:
    Ui::QrChatMessage *ui;
};

#endif // QRCHATMESSAGE_H
