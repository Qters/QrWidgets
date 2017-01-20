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
#include <QtWidgets/qlabel.h>

NS_QRWIDGETS_BEGIN

class QrWaitingSpinnerWidgetPrivate {
    QR_DECLARE_PUBLIC(QrWaitingSpinnerWidget)

public:
    QrWaitingSpinnerWidgetPrivate(QrWaitingSpinnerWidget *q,
                                  bool centerOnParent,
                                  bool disableParentWhenSpinning,
                                  bool semiTransparent);

private:
    QColor  color;
    qreal   roundness; // 0..100
    qreal   minimumTrailOpacity;
    qreal   trailFadePercentage;
    qreal   revolutionsPerSecond;
    int     numberOfLines;
    int     lineLength;
    int     lineWidth;
    int     innerRadius;

private:
    QTimer *timer;
    bool    semiTransparent;
    bool    centerOnParent;
    bool    disableParentWhenSpinning;
    int     currentCounter;
    bool    isSpinning;

private:
    QLabel* tipsLabel = nullptr;

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
    centerOnParent(centerOnParent),
    disableParentWhenSpinning(disableParentWhenSpinning),
    semiTransparent(semiTransparent)
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
    color = Qt::black;
    roundness = 100.0;
    minimumTrailOpacity = 3.14159265358979323846;
    trailFadePercentage = 80.0;
    revolutionsPerSecond = 1.57079632679489661923;
    numberOfLines = 20;
    lineLength = 10;
    lineWidth = 2;
    innerRadius = 10;
    currentCounter = 0;
    isSpinning = false;

    Q_Q(QrWaitingSpinnerWidget);
    timer = new QTimer(q);
    q->connect(timer, SIGNAL(timeout()), q, SLOT(rotate()));

    tipsLabel = new QLabel(q);
    tipsLabel->setStyleSheet("color:white;");
    tipsLabel->setAlignment(Qt::AlignHCenter);
    tipsLabel->hide();

    updateSize();
    updateTimer();
    q->hide();
}

void QrWaitingSpinnerWidgetPrivate::updateSize()
{
    Q_Q(QrWaitingSpinnerWidget);
    int size = (innerRadius + lineLength) * 2;
        if (! semiTransparent) {
        q->setFixedSize(size, size);
    }
}

void QrWaitingSpinnerWidgetPrivate::updateTimer()
{
    timer->setInterval(1000 / (numberOfLines * revolutionsPerSecond));
}

void QrWaitingSpinnerWidgetPrivate::updatePosition()
{
    Q_Q(QrWaitingSpinnerWidget);
    if (q->parentWidget() && centerOnParent) {
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
    if (nullptr != parent && d->semiTransparent) {
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
    if (nullptr != parent && d->semiTransparent) {
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

    if (d->currentCounter >= d->numberOfLines) {
        d->currentCounter = 0;
    }

    painter.setPen(Qt::NoPen);
    if(parentWidget() && d->semiTransparent) {
        painter.fillRect(parentWidget()->rect(), QBrush(QColor(0, 0, 0, 200)));
    }
    QSize size = this->size();
    for (int i = 0; i < d->numberOfLines; ++i) {
        painter.save();
        if (d->semiTransparent) {
            painter.translate(size.width()*0.5, size.height()*0.5 );
        } else {
            painter.translate(d->innerRadius + d->lineLength,
                              d->innerRadius + d->lineLength);
        }
        qreal rotateAngle =
                static_cast<qreal>(360 * i) / static_cast<qreal>(d->numberOfLines);
        painter.rotate(rotateAngle);
        painter.translate(d->innerRadius, 0);
        int distance =
                d->lineCountDistanceFromPrimary(i, d->currentCounter, d->numberOfLines);
        QColor color =
                d->currentLineColor(distance, d->numberOfLines, d->trailFadePercentage,
                                 d->minimumTrailOpacity, d->color);
        painter.setBrush(color);
        // TODO improve the way rounded rect is painted
        painter.drawRoundedRect(
                    QRect(0, -d->lineWidth / 2, d->lineLength, d->lineWidth), d->roundness,
                    d->roundness, Qt::RelativeSize);
        painter.restore();
    }
}

void QrWaitingSpinnerWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    Q_D(QrWaitingSpinnerWidget);
    if(d->tipsLabel->isVisible()) {
        QPoint promptLabelPos = QPoint(rect().center().x() - d->tipsLabel->width()/2 ,
                                       rect().center().y() + 2 * d->lineLength + d->tipsLabel->height());
        d->tipsLabel->move(promptLabelPos);
    }
}

void QrWaitingSpinnerWidget::start(const QString& tips /*= ""*/)
{
    Q_D(QrWaitingSpinnerWidget);
    d->updatePosition();
    d->isSpinning = true;

    d->tipsLabel->setText(tips);
    d->tipsLabel->setVisible(! tips.isEmpty());

    show();

    if(parentWidget() && d->disableParentWhenSpinning) {
        parentWidget()->setEnabled(false);
    }

    if (! d->timer->isActive()) {
        d->timer->start();
        d->currentCounter = 0;
    }
}

void QrWaitingSpinnerWidget::stop()
{
    Q_D(QrWaitingSpinnerWidget);
    d->isSpinning = false;
    hide();

    if(parentWidget() && d->disableParentWhenSpinning) {
        parentWidget()->setEnabled(true);
    }

    if (d->timer->isActive()) {
        d->timer->stop();
        d->currentCounter = 0;
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
    d->numberOfLines = lines;
    d->currentCounter = 0;
    d->updateTimer();
}

void QrWaitingSpinnerWidget::setLineLength(int length)
{
    Q_D(QrWaitingSpinnerWidget);
    d->lineLength = length;
    d->updateSize();
}

void QrWaitingSpinnerWidget::setLineWidth(int width)
{
    Q_D(QrWaitingSpinnerWidget);
    d->lineWidth = width;
    d->updateSize();
}

void QrWaitingSpinnerWidget::setInnerRadius(int radius)
{
    Q_D(QrWaitingSpinnerWidget);
    d->innerRadius = radius;
    d->updateSize();
}

QColor QrWaitingSpinnerWidget::color()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->color;
}

qreal QrWaitingSpinnerWidget::roundness()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->roundness;
}

qreal QrWaitingSpinnerWidget::minimumTrailOpacity()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->minimumTrailOpacity;
}

qreal QrWaitingSpinnerWidget::trailFadePercentage()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->trailFadePercentage;
}

qreal QrWaitingSpinnerWidget::revolutionsPersSecond()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->revolutionsPerSecond;
}

int QrWaitingSpinnerWidget::numberOfLines()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->numberOfLines;
}

int QrWaitingSpinnerWidget::lineLength()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->lineLength;
}

int QrWaitingSpinnerWidget::lineWidth()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->lineWidth;
}

int QrWaitingSpinnerWidget::innerRadius()
{
    Q_D(QrWaitingSpinnerWidget);
    return d->innerRadius;
}

bool QrWaitingSpinnerWidget::isSpinning() const
{
    Q_D(const QrWaitingSpinnerWidget);
    return d->isSpinning;
}

void QrWaitingSpinnerWidget::setRoundness(qreal roundness)
{
    Q_D(QrWaitingSpinnerWidget);
    d->roundness = std::max(0.0, std::min(100.0, roundness));
}

void QrWaitingSpinnerWidget::setColor(QColor color)
{
    Q_D(QrWaitingSpinnerWidget);
    d->color = color;
}

void QrWaitingSpinnerWidget::setRevolutionsPerSecond(qreal revolutionsPerSecond)
{
    Q_D(QrWaitingSpinnerWidget);
    d->revolutionsPerSecond = revolutionsPerSecond;
    d->updateTimer();
}

void QrWaitingSpinnerWidget::setTrailFadePercentage(qreal trail)
{
    Q_D(QrWaitingSpinnerWidget);
    d->trailFadePercentage = trail;
}

void QrWaitingSpinnerWidget::setMinimumTrailOpacity(qreal minimumTrailOpacity)
{
    Q_D(QrWaitingSpinnerWidget);
    d->minimumTrailOpacity = minimumTrailOpacity;
}

void QrWaitingSpinnerWidget::rotate()
{
    Q_D(QrWaitingSpinnerWidget);
    ++ d->currentCounter;
    if (d->currentCounter >= d->numberOfLines) {
        d->currentCounter = 0;
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
