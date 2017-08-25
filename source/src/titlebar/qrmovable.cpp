#include "titlebar/qrmovable.h"

#include <QtCore/qdebug.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#include "titlebar/qrshaderdelegate.h"

NS_QRWIDGETS_BEGIN

class QrMovablePrivate{
public:
    QR_DECLARE_PUBLIC(QrMovable)

public:
    QrShaderDelegate *shaderDelegate = nullptr;
    bool isMovable = true;
    bool clickOnButton = false;

    bool isNeedBoxBeforeChanging = false;
    bool isChangingParentRect = false;
    bool moveByRigthBottomCorner = true;
    QPoint parentPointMoveBefore; //

public:
    QrMovablePrivate(QrMovable* q);
};

QrMovablePrivate::QrMovablePrivate(QrMovable *q)
    : q_ptr(q)
{
}


NS_QRWIDGETS_END

///////////////////////////////////////////////

USING_NS_QRWIDGETS;

QrMovable::QrMovable(QWidget *parent)
    : QWidget(parent),
      d_ptr(new QrMovablePrivate(this))
{
    Q_ASSERT(nullptr != parent);

    parent->installEventFilter(this);
    parent->setMouseTracking(true);

    init();

    Q_D(QrMovable);
    qDebug() << "moveable:" << d->shaderDelegate ;
}

QrMovable::~QrMovable()
{

}

void QrMovable::init()
{
    Q_D(QrMovable);

    d->shaderDelegate = new QrShaderDelegate(this);
    connect(d->shaderDelegate, &QrShaderDelegate::moveEndSend, [this](QPoint point){
        Q_D(const QrMovable);
        if (! d->isMovable) {
            return;
        }

        parentWidget()->move(point);
    });
}

void QrMovable::setBoxColor(const QColor &boxColor)
{
    Q_D(QrMovable);
    d->shaderDelegate->setBoxColor(boxColor);
}

void QrMovable::setBoxEffect(bool visible)
{
    Q_D(QrMovable);
    d->shaderDelegate->setNeedPaint(visible);
    qDebug() << "QrMovable::setBoxEffect " << visible;
}

void QrMovable::moveByRigthBottomCorner(bool move)
{
    Q_D(QrMovable);
    d->moveByRigthBottomCorner = move;
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
    auto isMouseInRightBottomOfParent = [this, event]() {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(nullptr == mouseEvent) {
            return false;
        }
        QPoint parentRightBottomPos(parentWidget()->rect().width(),
                                    parentWidget()->rect().height());

        int halfWidth = 15;
        QRect pointRect(parentRightBottomPos + QPoint(-halfWidth, -halfWidth),
                        parentRightBottomPos + QPoint(halfWidth, halfWidth));

        return pointRect.contains(mouseEvent->pos());
    };

    if(watched == parent()) {
        Q_D(QrMovable);
        switch(event->type()) {
        case QEvent::Paint:
            if(nullptr != d->shaderDelegate) {
                QRect region = parentWidget()->rect();

                QPainter painter(parentWidget());
                QPen pen;
                pen.setColor(d->shaderDelegate->boxColor());
                pen.setWidth(2);
                painter.setPen(pen);
                painter.drawRect(region.x(), region.y(), region.width(), region.height());
            }
            break;
        case QEvent::MouseMove:
            if (isMouseInRightBottomOfParent() && d->moveByRigthBottomCorner) {
                parentWidget()->setCursor(QCursor(Qt::SizeFDiagCursor));
                if(d->isChangingParentRect) {
                    QPoint offset = static_cast<QMouseEvent*>(event)->pos() - d->parentPointMoveBefore;
                    parentWidget()->setGeometry(parentWidget()->pos().x(), parentWidget()->pos().y(),
                                                parentWidget()->rect().width() + offset.x(),
                                                parentWidget()->rect().height() + offset.y());
                    parentWidget()->update();
                    d->parentPointMoveBefore = static_cast<QMouseEvent*>(event)->pos();
                } else {
                    parentWidget()->setCursor(QCursor(Qt::ArrowCursor));
                }
            }
            break;
        case QEvent::MouseButtonPress:
            if (isMouseInRightBottomOfParent() && d->moveByRigthBottomCorner) {
                parentWidget()->setCursor(QCursor(Qt::SizeFDiagCursor));

                d->isNeedBoxBeforeChanging = d->shaderDelegate->needPaint();
                setBoxEffect(false);

                d->isChangingParentRect = true;
                d->parentPointMoveBefore = static_cast<QMouseEvent*>(event)->pos();
            }
            break;
        case QEvent::MouseButtonRelease:
            if(d->isChangingParentRect && d->moveByRigthBottomCorner) {
                parentWidget()->setCursor(QCursor(Qt::ArrowCursor));
                d->isChangingParentRect = false;

                setBoxEffect(d->isNeedBoxBeforeChanging);
            }
            break;
        }
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

bool QrMovable::isChangingParentRect() const
{
    Q_D(const QrMovable);
    return d->isChangingParentRect;
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
