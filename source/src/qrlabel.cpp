#include "qrlabel.h"

#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpalette.h>
#include <QtWidgets/qstyle.h>

NS_QRWIDGETS_BEGIN

class QrLabelPrivate {
    QR_DECLARE_PUBLIC(QrLabel)

public:
    bool press = false;
    int state = -1;

public:
    QrLabelPrivate(QrLabel *q)
        : q_ptr(q) {}

    void updateQss() {
        Q_Q(QrLabel);
        q->style()->unpolish(q);
        q->style()->polish(q);
        q->update();
    }
};

NS_QRWIDGETS_END

/////////////////////////////////////////////////////////////////

USING_NS_QRWIDGETS;

QrLabel::QrLabel(QWidget *parent)
    : QLabel(parent),
      d_ptr(new QrLabelPrivate(this))
{
}

int QrLabel::getState() const
{
    Q_D(const QrLabel);

    return d->state;
}

void QrLabel::setState(int value)
{
    Q_D(QrLabel);

    d->state = value;
    d->updateQss();

    emit stateChanged(value);
}

void QrLabel::mousePressEvent(QMouseEvent *event)
{
    Q_D(QrLabel);
    if (event->button() == Qt::LeftButton) {
        d->press = true;
        emit clicked();
    }
}

void QrLabel:: mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    Q_D(QrLabel);
    d->press = false;
    update();
}
