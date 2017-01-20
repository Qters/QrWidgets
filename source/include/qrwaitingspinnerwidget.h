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

#include "qrglobal.h"
#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QrWaitingSpinnerWidgetPrivate;
class QRWIDGETSSHARED_EXPORT QrWaitingSpinnerWidget : public QWidget
{
    Q_OBJECT
    QR_DECLARE_PRIVATE(QrWaitingSpinnerWidget)

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
    void start(const QString& tips = "");
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

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *paintEvent) override;
    virtual void showEvent(QShowEvent *event) override;

private:
    QrWaitingSpinnerWidget(const QrWaitingSpinnerWidget&);
    QrWaitingSpinnerWidget& operator=(const QrWaitingSpinnerWidget&);
};

NS_QRWIDGETS_END
