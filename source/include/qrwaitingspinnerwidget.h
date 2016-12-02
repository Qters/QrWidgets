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

#pragma once

// Qt includes
#include <QWidget>
#include <QTimer>
#include <QColor>

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QRWIDGETSSHARED_EXPORT QrWaitingSpinnerWidget : public QWidget {
    Q_OBJECT

public:
    /*! Constructor for "standard" widget behaviour - use this
   * constructor if you wish to, e.g. embed your widget in another. */
    QrWaitingSpinnerWidget(QWidget *parent = 0,
                         bool centerOnParent = true,
                         bool disableParentWhenSpinning = true,
                         bool semiTransparent = true);

    /*! Constructor - use this constructor to automatically create a modal
   * ("blocking") spinner on top of the calling widget/window.  If a valid
   * parent widget is provided, "centreOnParent" will ensure that
   * QtWaitingSpinner automatically centres itself on it, if not,
   * "centreOnParent" is ignored. */
    QrWaitingSpinnerWidget(Qt::WindowModality modality,
                         QWidget *parent = 0,
                         bool centerOnParent = true,
                         bool disableParentWhenSpinning = true,
                         bool semiTransparent = true);

public slots:
    void start();
    void stop();

public:
    void defaultSetting();
    void setColor(QColor color);
    void setRoundness(qreal roundness);
    void setMinimumTrailOpacity(qreal minimumTrailOpacity);
    void setTrailFadePercentage(qreal trail);
    void setRevolutionsPerSecond(qreal revolutionsPerSecond);
    void setNumberOfLines(int lines);
    void setLineLength(int length);
    void setLineWidth(int width);
    void setInnerRadius(int radius);
    void setText(QString text);

    QColor color();
    qreal roundness();
    qreal minimumTrailOpacity();
    qreal trailFadePercentage();
    qreal revolutionsPersSecond();
    int numberOfLines();
    int lineLength();
    int lineWidth();
    int innerRadius();

    bool isSpinning() const;

private slots:
    void rotate();

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void paintEvent(QPaintEvent *paintEvent);

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
    QrWaitingSpinnerWidget(const QrWaitingSpinnerWidget&);
    QrWaitingSpinnerWidget& operator=(const QrWaitingSpinnerWidget&);

    QTimer *_timer;
    bool    _semiTransparent;
    bool    _centerOnParent;
    bool    _disableParentWhenSpinning;
    int     _currentCounter;
    bool    _isSpinning;
};

NS_QRWIDGETS_END
