#ifndef QRMSGBUBBLE_H
#define QRMSGBUBBLE_H

#include "qrglobal.h"

#include "qrwidgets_global.h"

#include "qrlabel.h"

NS_QRWIDGETS_BEGIN

class QrMsgBubblePrivate;

class QRWIDGETSSHARED_EXPORT QrMsgBubble : public QrLabel
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrMsgBubble)

public:
    explicit QrMsgBubble(QWidget *parent = Q_NULLPTR);

public:
    enum AnchorDirection {
        Left,
        Right,
        LeftUp,
        LeftDown,
    };

public:
    void setAnchorDirection(AnchorDirection direction);
    AnchorDirection anchorDirection() const;

public:
    void setBackground(const QColor& color);
    QColor background() const;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

NS_QRWIDGETS_END

#endif // QRMSGBUBBLE_H
