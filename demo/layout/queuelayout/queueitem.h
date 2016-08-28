#ifndef QUEUEITEM_H
#define QUEUEITEM_H

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlabel.h>

class QueueItem : public QWidget
{
public:
    QueueItem(QWidget* parent = Q_NULLPTR);

public:
    virtual int heightForWidth(int width) const override { \
        return width;
    }

    virtual bool hasHeightForWidth() const override { \
        return true;
    }

private:
    static int count;
    QPushButton *closeButton;
    QLabel *content;
};

#endif // QUEUEITEM_H
