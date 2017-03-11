#include "titlebar/qrshaderdelegate.h"

#include <QStyleOption>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QDebug>

class QrShaderDelegatePrivate{
public:
    QPoint startPoint;
    QColor boxColor = Qt::black;
};

QrShaderDelegate::QrShaderDelegate(QWidget *parent)
    : QWidget(parent),
      d_ptr(new QrShaderDelegatePrivate)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void QrShaderDelegate::mousePressed(QMouseEvent *e, QRect rect)
{
    Q_D(QrShaderDelegate);
    setGeometry(rect);
    d->startPoint = e->globalPos();

    mousePressEvent(e);
}

void QrShaderDelegate::mouseMoving(QMouseEvent* e)
{
    show();

    mouseMoveEvent(e);
}

void QrShaderDelegate::mouseReleased(QMouseEvent* e)
{
    mouseReleaseEvent(e);
}

void QrShaderDelegate::setBoxColor(const QColor &color)
{
    Q_D(QrShaderDelegate);
    d->boxColor = color;
}

QColor QrShaderDelegate::boxColor() const
{
    Q_D(const QrShaderDelegate);
    return d->boxColor;
}

void QrShaderDelegate::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}

void QrShaderDelegate::mouseMoveEvent(QMouseEvent *e)
{
    Q_D(QrShaderDelegate);
    move(pos() + e->globalPos() - d->startPoint);
    d->startPoint = e->globalPos();

    QWidget::mouseMoveEvent(e);
}

void QrShaderDelegate::mouseReleaseEvent(QMouseEvent *e)
{
    Q_D(QrShaderDelegate);
    QPoint desPos = pos() + e->globalPos() - d->startPoint;
    emit moveEndSend(desPos);

    hide();

    QWidget::mouseReleaseEvent(e);
}

void QrShaderDelegate::paintEvent(QPaintEvent *)
{
    Q_D(QrShaderDelegate);

    QRect region = rect();

    QPainter painter(this);
    QPen pen;
    pen.setColor(d->boxColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(region.x(), region.y(), region.width(), region.height());
}
