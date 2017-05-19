#include "chat/qrmsgbubble.h"

#include <QPainter>
#include <QBrush>
#include <QMouseEvent>

NS_QRWIDGETS_BEGIN

class QrMsgBubblePrivate {
    QR_DECLARE_PUBLIC(QrMsgBubble)

public:
    QrMsgBubblePrivate(QrMsgBubble *q)
        : q_ptr(q) {}

public:
    bool mouseClicked = false;
    int clickPos = 0;

    int anchorCornorSpace = 4;
    int anchorShortWidth = 5;
    int anchorLongWidth = 12;
    int anchorRadius = 10;
    const QMargins contentMargin = QMargins(6, 10, 6, 10);
    QColor background = QColor("#89cff0");
    QrMsgBubble::AnchorDirection anchorDirection = QrMsgBubble::AnchorDirection::LeftUp;

public:
    QMargins anchorDirectionMargin(QrMsgBubble::AnchorDirection anchorDirection) const;
};

QMargins QrMsgBubblePrivate::anchorDirectionMargin(QrMsgBubble::AnchorDirection anchorDirection) const
{
    int contentMargin = 3;
    QMargins margin;
    switch(anchorDirection) {
    case Qters::QrWidgets::QrMsgBubble::Left:
        margin = QMargins(anchorShortWidth + contentMargin, contentMargin, contentMargin, contentMargin);
        break;
    case Qters::QrWidgets::QrMsgBubble::Right:
        margin = QMargins(contentMargin, contentMargin, anchorShortWidth + contentMargin, contentMargin);
        break;
    case Qters::QrWidgets::QrMsgBubble::LeftUp:
        margin = QMargins(anchorShortWidth + contentMargin, anchorShortWidth + contentMargin, contentMargin, contentMargin);
        break;
    case Qters::QrWidgets::QrMsgBubble::LeftDown:
        margin = QMargins(anchorShortWidth + contentMargin, contentMargin, contentMargin, anchorShortWidth + contentMargin);
        break;
    }
    return margin;
}

NS_QRWIDGETS_END

USING_NS_QRWIDGETS;

QrMsgBubble::QrMsgBubble(QWidget *parent)
    : QrLabel(parent),
      d_ptr(new QrMsgBubblePrivate(this))
{
    Q_D(const QrMsgBubble);
    setWordWrap(true);
    setMouseTracking(true);
    setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    setContentsMargins(d->anchorDirectionMargin(d->anchorDirection));
}

void QrMsgBubble::setAnchorDirection(QrMsgBubble::AnchorDirection direction)
{
    Q_D(QrMsgBubble);
    d->anchorDirection = direction;
    setContentsMargins(d->anchorDirectionMargin(d->anchorDirection));
}

QrMsgBubble::AnchorDirection QrMsgBubble::anchorDirection() const
{
    Q_D(const QrMsgBubble);
    return d->anchorDirection;
}

void QrMsgBubble::setBackground(const QColor &color)
{
    Q_D(QrMsgBubble);
    d->background = color;
}

QColor QrMsgBubble::background() const
{
    Q_D(const QrMsgBubble);
    return d->background;
}

void QrMsgBubble::paintEvent(QPaintEvent *event)
{
    Q_D(const QrMsgBubble);

    QPainter painter(this);
    QBrush brush(d->background);

    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    int boundingRectWidth = width();
    int boundingRectHeight = height();
    QRect bubbleRect = rect();
    switch (d->anchorDirection) {
    case Qters::QrWidgets::QrMsgBubble::Left:
        bubbleRect = QRect(d->anchorShortWidth, 0, boundingRectWidth - d->anchorShortWidth, boundingRectHeight);
        break;
    case Qters::QrWidgets::QrMsgBubble::Right:
        bubbleRect = QRect(0, 0, boundingRectWidth - d->anchorShortWidth, boundingRectHeight);
        break;
    case Qters::QrWidgets::QrMsgBubble::LeftUp:
        bubbleRect = QRect(0, d->anchorShortWidth, boundingRectWidth, boundingRectHeight - d->anchorShortWidth);
        break;
    case Qters::QrWidgets::QrMsgBubble::LeftDown:
        bubbleRect = QRect(0, 0, boundingRectWidth, boundingRectHeight - d->anchorShortWidth);
        break;
    }
    painter.drawRoundedRect(bubbleRect, d->anchorRadius, d->anchorRadius);

    QPointF points[3];
    switch (d->anchorDirection) {
    case AnchorDirection::Left:
        points[0] = QPointF(d->anchorShortWidth + d->anchorRadius / 2, d->anchorCornorSpace);
        points[1] = QPointF(0.0, d->anchorCornorSpace / 2);
        points[2] = QPointF(d->anchorShortWidth, d->anchorCornorSpace + d->anchorLongWidth);
        break;
    case AnchorDirection::Right:
        points[0] = QPointF(boundingRectWidth - d->anchorShortWidth - d->anchorRadius / 2, d->anchorCornorSpace);
        points[1] = QPointF(boundingRectWidth, d->anchorCornorSpace / 2);
        points[2] = QPointF(boundingRectWidth - d->anchorShortWidth, d->anchorCornorSpace + d->anchorLongWidth);
        break;
    case AnchorDirection::LeftUp:
        points[0] = QPointF(d->anchorCornorSpace, d->anchorShortWidth + d->anchorRadius / 2);
        points[1] = QPointF(d->anchorCornorSpace / 2, 0.0);
        points[2] = QPointF(d->anchorCornorSpace + d->anchorLongWidth, d->anchorShortWidth);
        break;
    case AnchorDirection::LeftDown:
        points[0] = QPointF(d->anchorCornorSpace, boundingRectHeight - d->anchorShortWidth - d->anchorRadius / 2);
        points[1] = QPointF(d->anchorCornorSpace / 2, boundingRectHeight);
        points[2] = QPointF(d->anchorCornorSpace + d->anchorLongWidth, boundingRectHeight - d->anchorShortWidth);
        break;
    default:
        break;
    }
    painter.drawConvexPolygon(points, 3);

    return QLabel::paintEvent(event);
}

void QrMsgBubble::mousePressEvent(QMouseEvent *event)
{
    Q_D(QrMsgBubble);
    if(rect().contains(event->pos())) {
        d->mouseClicked = true;
//        d->clickPos =
    }
    return QrLabel::mousePressEvent(event);
}

void Qters::QrWidgets::QrMsgBubble::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QrMsgBubble);
    if(d->mouseClicked) {
        d->mouseClicked = false;
    }
    return QrLabel::mouseReleaseEvent(event);
}

void Qters::QrWidgets::QrMsgBubble::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QrMsgBubble);
    if(d->mouseClicked) {
//        setSelection();
    }
    return QrLabel::mouseMoveEvent(event);
}
