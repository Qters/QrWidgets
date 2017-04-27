#ifndef QRMOVABLE_H
#define QRMOVABLE_H

#include <QtWidgets/qpushbutton.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QrMovablePrivate;

class QRWIDGETSSHARED_EXPORT QrMovable : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrMovable)

public:
    QrMovable(QWidget *parent);
    ~QrMovable();

public:
    void donotMove(QPushButton *button);
    void setBoxColor(const QColor& boxColor);

public:
    void setBoxEffect(bool visible);

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

NS_QRWIDGETS_END

#endif // QRMOVABLE_H
