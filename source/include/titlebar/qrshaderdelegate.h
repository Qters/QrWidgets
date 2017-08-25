#ifndef QRSHADERDELEGATE_H
#define QRSHADERDELEGATE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QRect>
#include <QPoint>

#include "qrglobal.h"

#include "qrwidgets_global.h"

class QrShaderDelegatePrivate;
class QRWIDGETSSHARED_EXPORT QrShaderDelegate : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrShaderDelegate)

public:
    explicit QrShaderDelegate(QWidget *parent = 0);

public:
    void mousePressed(QMouseEvent* e, QRect rect);
    void mouseMoving(QMouseEvent* e);
    void mouseReleased(QMouseEvent* e);

    void setBoxColor(const QColor& color);
    QColor boxColor() const;
    void setNeedPaint(bool needpaint);
    bool needPaint() const;

protected:
    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseMoveEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;
    virtual void paintEvent(QPaintEvent* ) override;

signals:
    void moveEndSend(QPoint endPoint);
};

#endif // QRSHADERDELEGATE_H
