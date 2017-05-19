#ifndef QRCHATMESSAGE_H
#define QRCHATMESSAGE_H

#include <QWidget>

#include "qrwidgets_global.h"

namespace Ui {
class QrChatMessage;
}

NS_QRWIDGETS_BEGIN

class QRWIDGETSSHARED_EXPORT QrChatMessage : public QWidget
{
    Q_OBJECT

public:
    explicit QrChatMessage(QWidget *parent = 0);
    ~QrChatMessage();

public:
    void setName(const QString& text);
    void setLogo(const QPixmap &logo);
    void setMessage(const QString& text);

private:
    Ui::QrChatMessage *ui;
};

NS_QRWIDGETS_END

#endif // QRCHATMESSAGE_H
