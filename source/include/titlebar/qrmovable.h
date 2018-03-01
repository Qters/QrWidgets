#ifndef QRMOVABLE_H
#define QRMOVABLE_H

#include <QtWidgets/qpushbutton.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QrMovablePrivate;
class QrMovableImplPrivate;

class QRWIDGETSSHARED_EXPORT QrMovableImpl : public QObject
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrMovableImpl)

public:
    QrMovableImpl(QWidget *moveWidget);
    ~QrMovableImpl();

};

class QRWIDGETSSHARED_EXPORT QrMovable : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrMovable)

public:
    QrMovable(QWidget *parent);
    ~QrMovable();

public:
    void init();

    void donotMove(QPushButton *button);
    void setBoxEffect(bool visible);
    void setMoveBoxColor(const QColor& boxColor);

public:
    void setMoveBoxEffect(bool visible);
    void moveByRigthBottomCorner(bool move);

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

protected:
    bool isChangingParentRect() const;
};

NS_QRWIDGETS_END

#endif // QRMOVABLE_H
