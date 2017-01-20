/* Original Work Copyright (c) 2012-2014 Alexander Turkin
   Modified 2014 by William Hallatt
   Modified 2015 by Jacob Dawid

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// Own includes
#include "qrwaitingspinnerwidget.h"

// Standard includes
#include <cmath>
#include <algorithm>

// Qt includes
#include <QPainter>
#include <QTimer>
#include <QEvent>

NS_QRWIDGETS_BEGIN

class QrWaitingSpinnerWidgetPrivate {
    QR_DECLARE_PUBLIC(QrWaitingSpinnerWidget)

public:
    QrWaitingSpinnerWidgetPrivate(QrWaitingSpinnerWidget *q,
                                  bool centerOnParent,
                                  bool disableParentWhenSpinning,
                                  bool semiTransparent);

private:
    QColor  _color;
    qreal   _roundness; // 0..100
    qreal   _minimumTrailOpacity;
    qreal   _trailFadePercentage;
    qreal   _revolutionsPerSecond;
    int     _numberOfLines;
    int     _lineLength;
    int     _lineWidth;
    int     _innerRadius;

private:
    QTimer *_timer;
    bool    _semiTransparent;
    bool    _centerOnParent;
    bool    _disableParentWhenSpinning;
    int     _currentCounter;
    bool    _isSpinning;

private:
    static int lineCountDistanceFromPrimary(int current, int primary,
                                            int totalNrOfLines);
    static QColor currentLineColor(int distance, int totalNrOfLines,
                                   qreal trailFadePerc, qreal minOpacity,
                                   QColor color);

    void initialize();
    void updateSize();
    void updateTimer();
    void updatePosition();
};

QrWaitingSpinnerWidgetPrivate::QrWaitingSpinnerWidgetPrivate(QrWaitingSpinnerWidget *q,
                                                             bool centerOnParent,
                                                             bool disableParentWhenSpinning,
                                                             bool semiTransparent)
    : q_ptr(q),
    _centerOnParent(centerOnParent),
    _disableParentWhenSpinning(disableParentWhenSpinning),
    _semiTransparent(semiTransparent)
{}

int QrWaitingSpinnerWidgetPrivate::lineCountDistanceFromPrimary(
        int current, int primary, int totalNrOfLines)
{
    int distance = primary - current;
    if (distance < 0) {
        distance += totalNrOfLines;
    }
    return distance;
}

QColor QrWaitingSpinnerWidgetPrivate::currentLineColor(int countDistance, int totalNrOfLines,
                                                       qreal trailFadePerc, qreal minOpacity,
                                                       QColor color)
{
    if (countDistance == 0) {
        return color;
    }
    const qreal minAlphaF = minOpacity / 100.0;
    int distanceThreshold =
            static_cast<int>(ceil((totalNrOfLines - 1) * trailFadePerc / 100.0));
    if (countDistance > distanceThreshold) {
        color.setAlphaF(minAlphaF);
    } else {
        qreal alphaDiff = color.alphaF() - minAlphaF;
        qreal gradient = alphaDiff / static_cast<qreal>(distanceThreshold + 1);
        qreal resultAlpha = color.alphaF() - gradient * countDistance;

        // If alpha is out of bounds, clip it.
        resultAlpha = std::min(1.0, std::max(0.0, resultAlpha));
        color.setAlphaF(resultAlpha);
    }
    return color;
}

void QrWaitingSpinnerWidgetPrivate::initialize()
{
    _color = Qt::black;
    _roundness = 100.0;
    _minimumTrailOpacity = 3.14159265358979323846;
    _trailFadePercentage = 80.0;
    _revolutionsPerSecond = 1.57079632679489661923;
    _numberOfLines = 20;
    _lineLength = 10;
    _lineWidth = 2;
    _innerRadius = 10;
    _currentCounter = 0;
    _isSpinning = false;

    Q_Q(QrWaitingSpinnerWidget);
    _timer = new QTimer(q);
    q->connect(_timer, SIGNAL(timeout()), q, SLOT(rotate()));
    updateSize();
    updateTimer();
    q->hide();
}

void QrWaitingSpinnerWidgetPrivate::updateSize()
{
    Q_Q(QrWaitingSpinnerWidget);
    int size = (_innerRadius + _lineLength) * 2;
        if (! _semiTransparent) {
        q->setFixedSize(size, size);
    }
}

void QrWaitingSpinnerWidgetPrivate::updateTimer()
{
    _timer->setInterval(1000 / (_numberOfLines * _revolutionsPerSecond));
}

void QrWaitingSpinnerWidgetPrivate::updatePosition()
{
    Q_Q(QrWaitingSpinnerWidget);
    if (q->parentWidget() && _centerOnParent) {
        q->move(q->parentWidget()->width() / 2 - q->width() / 2,
             q->parentWidget()->height() / 2 - q->height() / 2);
    }
}

NS_QRWIDGETS_END

USING_NS_QRWIDGETS;

QrWaitingSpinnerWidget::QrWaitingSpinnerWidget(
        QWidget *parent,
        bool centerOnParent,
        bool disableParentWhenSpinning,
        bool semiTransparent)
    : QWidget(parent),
      d_ptr(new QrWaitingSpinnerWidgetPrivate(
                this,
                centerOnParent,
                disableParentWhenSpinning,
                semiTransparent))
{
    Q_D(QrWaitingSpinnerWidget);
    d->initialize();

    if (nullptr != parent) {
        parent->installEventFilter(this);
    }
    if (nullptr != parent && d->_semiTransparent) {
        resize(parentWidget()->size());
    }
}

QrWaitingSpinnerWidget::QrWaitingSpinnerWidget(
        Qt::WindowModality modality,
        QWidget *parent,
        bool centerOnParent,
        bool disableParentWhenSpinning,
        bool semiTransparent)
    : QWidget(parent, Qt::Dialog | Qt::FramelessWindowHint),
      d_ptr(new QrWaitingSpinnerWidgetPrivate(
                this,
                centerOnParent,
                disableParentWhenSpinning,
                semiTransparent))
{
    Q_D(QrWaitingSpinnerWidget);
    d->initialize();

    // We need to set the window modality AFTER we've hidden the
    // widget for the first time since changing this property while
    // the widget is visible has no effect.
    setWindowModality(modality);
    setAttribute(Qt::WA_TranslucentBackground);

    if (nullptr != parent) {
        parent->installEventFilter(this);
    }
    if (nullptr != parent && d->_semiTransparent) {
        resize(parentWidget()->size());
    }
}

void QrWaitingSpinnerWidget::paintEvent(QPaintEvent *)
{
    Q_D(QrWaitingSpinnerWidget);

    d->updatePosition();
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::transparent);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (d->_currentCounter >= d->_numberOfLines) {
        d->_currentCounter = 0;
    }

    painter.setPen(Qt::NoPen);
    if(parentWidget() && d->_semiTransparent) {
        painter.fillRect(parentWidget()->rect(), QBrush(QColor(0, 0, 0, 200)));
    }
    QSize size = this->size();
    for (int i = 0; i < d->_numberOfLines; ++i) {
        painter.save();
        if (d->_semiTransparent) {
            painter.translate(size.width()*0.5, size.height()*0.5 );
        } else {
            painter.translate(d->_innerRadius + d->_lineLength,
                              d->_innerRadius + d->_lineLength);
        }
        qreal rotateAngle =
                static_cast<qreal>(360 * i) / static_cast<qreal>(d->_numberOfLines);
        painter.rotate(rotateAngle);
        painter.translate(d->_innerRadius, 0);
        int distance =
                d->lineCountDistanceFromPrimary(i, d->_currentCounter, d->_numberOfLines);
        QColor color =
                d->currentLineColor(distance, d->_numberOfLines, d->_trailFadePercentage,
                                 d->_minimumTrailOpacity, d->_color);
        painter.setBrush(color);
        // TODO improve the way rounded rect is painted
        painter.drawRoundedRect(
                    QRect(0, -d->_lineWidth / 2, d->_lineLength, d->_lineWidth), d->_roundness,
                    d->_roundness, Qt::RelativeSize);
        painter.restore();
    }
}

void QrWaitingSpinnerWidget::start()
{
    Q_D(QrWaitingSpinnerWidget);
    d->updatePosition();
    d->_isSpinning = true;
    show();

    if(parentWidget() && d->_disableParentWhenSpinning) {
        parentWidget()->setEnabled(false);
    }

    if (! d->_timer->isActive()) {
        d->_timer->start();
        d->_currentCounter = 0;
    }
}

void QrWaitingSpinnerWidget::stop()
{
    Q_D(QrWaitingSpinnerWidget);
    d->_isSpinning = false;
    hide();

    if(parentWidget() && d->_disableParentWhenSpinning) {
        parentWidget()->setEnabled(true);
    }

    if (d->_timer->isActive()) {
        d->_timer->stop();
        d->_currentCounter = 0;
    }
}

void QrWaitingSpinnerWidget::defaultSetting()
{
    setRoundness(70.0);
    setMinimumTrailOpacity(15.0);
    setTrailFadePercentage(70.0);
    setNumberOfLines(20);
    setLineLength(20);
    setLineWidth(2);
    setInnerRadius(18);
    setRevolutionsPerSecond(1);
    setColor(Qt::white);
}

void QrWaitingSpinnerWidget::setNumberOfLines(int lines)
{
    Q_D(QrWaitingSpinnerWidget);
    d->_numberOfLines = lines;
    d->_currentCounter = 0;
    d->updateTimer();
}

void QrWaitingSpinnerWidget::setLineLength(int length)
{
    Q_D(QrWaitingSpinnerWidget);
    d->_lineLength = length;
    d->updateSize();
}

void QrWaitingSpinnerWidget::setLineWidth(int width)
{
    Q_D(QrWaitingSpinnerWidget);
    d->_lineWidth = width;
    d->updateSize();
}

void QrWaitingSpinnerWidget::setInnerRadius(int radius)
{
    Q_D(QrWaitingSpinnerWidget);
    d->_innerRadius = radius;
    d->updateSize();
}

QColor QrWaitingSpinnerWidget::color()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->_color;
}

qreal QrWaitingSpinnerWidget::roundness()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->_roundness;
}

qreal QrWaitingSpinnerWidget::minimumTrailOpacity()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->_minimumTrailOpacity;
}

qreal QrWaitingSpinnerWidget::trailFadePercentage()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->_trailFadePercentage;
}

qreal QrWaitingSpinnerWidget::revolutionsPersSecond()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->_revolutionsPerSecond;
}

int QrWaitingSpinnerWidget::numberOfLines()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->_numberOfLines;
}

int QrWaitingSpinnerWidget::lineLength()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->_lineLength;
}

int QrWaitingSpinnerWidget::lineWidth()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->_lineWidth;
}

int QrWaitingSpinnerWidget::innerRadius()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->_innerRadius;
}

bool QrWaitingSpinnerWidget::isSpinning() const
{
    Q_D(const QrWaitingSpinnerWidget);
    return d->_isSpinning;
}

void QrWaitingSpinnerWidget::setRoundness(qreal roundness)
{
    Q_D(QrWaitingSpinnerWidget);
    d->_roundness = std::max(0.0, std::min(100.0, roundness));
}

void QrWaitingSpinnerWidget::setColor(QColor color)
{
    Q_D(QrWaitingSpinnerWidget);
    d->_color = color;
}

void QrWaitingSpinnerWidget::setRevolutionsPerSecond(qreal revolutionsPerSecond)
{
    Q_D(QrWaitingSpinnerWidget);
    d->_revolutionsPerSecond = revolutionsPerSecond;
    d->updateTimer();
}

void QrWaitingSpinnerWidget::setTrailFadePercentage(qreal trail)
{
    Q_D(QrWaitingSpinnerWidget);
    d->_trailFadePercentage = trail;
}

void QrWaitingSpinnerWidget::setMinimumTrailOpacity(qreal minimumTrailOpacity)
{
    Q_D(QrWaitingSpinnerWidget);
    d->_minimumTrailOpacity = minimumTrailOpacity;
}

void QrWaitingSpinnerWidget::rotate()
{
    Q_D(QrWaitingSpinnerWidget);
    ++ d->_currentCounter;
    if (d->_currentCounter >= d->_numberOfLines) {
        d->_currentCounter = 0;
    }
    update();
}

bool QrWaitingSpinnerWidget::eventFilter(QObject *watched, QEvent *event)
{
    auto parentWidget = qobject_cast<QWidget*>(parent());
    if (nullptr != parentWidget
            && parentWidget == watched
            && QEvent::Resize == event->type()) {
        resize(parentWidget->size());
    }
    return QWidget::eventFilter(watched, event);
}
