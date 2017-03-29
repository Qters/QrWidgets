#include "titlebar/qrmovable.h"

#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#include "titlebar/qrshaderdelegate.h"

class QrMovablePrivate{
public:
    QR_DECLARE_PUBLIC(QrMovable)

public:
    QrShaderDelegate *shaderDelegate = nullptr;
    bool isMovable = true;
    bool clickOnButton = false;

public:
    QrMovablePrivate(QrMovable* q);

public:
    void init();
};

QrMovablePrivate::QrMovablePrivate(QrMovable *q)
    : q_ptr(q)
{
}

void QrMovablePrivate::init()
{
    Q_Q(QrMovable);

    shaderDelegate = new QrShaderDelegate(q);
    q->connect(shaderDelegate, &QrShaderDelegate::moveEndSend, [this](QPoint point){
        if (! isMovable) {
            return;
        }

        Q_Q(QrMovable);
        q->parentWidget()->move(point);
    });
}



///////////////////////////////////////////////

QrMovable::QrMovable(QWidget *parent)
    : QWidget(parent),
      d_ptr(new QrMovablePrivate(this))
{
    Q_ASSERT(nullptr != parent);

    parent->installEventFilter(this);

    Q_D(QrMovable);
    d->init();
}

QrMovable::~QrMovable()
{

}

void QrMovable::setBoxColor(const QColor &boxColor)
{
    Q_D(QrMovable);
    d->shaderDelegate->setBoxColor(boxColor);
}

void QrMovable::setBoxEffect(bool visible)
{
    Q_D(QrMovable);
    d->shaderDelegate->setVisible(visible);
}

void QrMovable::donotMove(QPushButton *button)
{
    connect(button, &QPushButton::pressed, [this](){
        Q_D(QrMovable);
        d->clickOnButton = true;
    });
}

bool QrMovable::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == parent() && event->type() == QEvent::Paint) {
        Q_D(QrMovable);
        QRect region = parentWidget()->rect();

        QPainter painter(parentWidget());
        QPen pen;
        pen.setColor(d->shaderDelegate->boxColor());
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRect(region.x(), region.y(), region.width(), region.height());
    }

    return QWidget::eventFilter(watched, event);
}

void QrMovable::mousePressEvent(QMouseEvent *e)
{
    Q_D(QrMovable);

    if(d->clickOnButton) {
        d->clickOnButton = false;
    }

    if (d->isMovable) {
        QRect rect = parentWidget()->geometry();
        d->shaderDelegate->mousePressed(e, rect);
    }

    QWidget::mousePressEvent(e);
}

void QrMovable::mouseMoveEvent(QMouseEvent *e)
{
    Q_D(QrMovable);
    if(d->clickOnButton) {
        return;
    }
    if (d->isMovable) {
        d->shaderDelegate->mouseMoving(e);
    }

    QWidget::mouseMoveEvent(e);
}

void QrMovable::mouseReleaseEvent(QMouseEvent *e)
{
    Q_D(QrMovable);
    if(d->clickOnButton) {
        return;
    }

    if (d->isMovable) {
        d->shaderDelegate->mouseReleased(e);
    }

    QWidget::mouseReleaseEvent(e);
}
