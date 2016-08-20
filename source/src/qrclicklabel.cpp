#include "qrclicklabel.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>

NS_QRWIDGETS_BEGIN

class QrClickLabelPrivate {
public:
    bool press = false;
};

NS_QRWIDGETS_END

/////////////////////////////////////////////////////////////////

USING_NS_QRWIDGETS;

QrClickLabel::QrClickLabel(QWidget *parent)
    : QLabel(parent),
      d_ptr(new QrClickLabelPrivate)
{
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    setCursor(Qt::PointingHandCursor);
}

void QrClickLabel::mousePressEvent(QMouseEvent *event)
{
    Q_D(QrClickLabel);
    if (event->button() == Qt::LeftButton) {
        d->press = true;
        emit clicked();
    }
}

void QrClickLabel:: mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    Q_D(QrClickLabel);
    d->press = false;
    update();
}
