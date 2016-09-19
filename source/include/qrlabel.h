#ifndef QRLABEL_H
#define QRLABEL_H

#include <QLabel>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QrLabelPrivate;

class QRWIDGETSSHARED_EXPORT QrLabel : public QLabel
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrLabel)

public:
    QrLabel(QWidget *parent = Q_NULLPTR);

Q_SIGNALS:
    void stateChanged(int);
    void clicked();

public:
    int getState() const;
    void setState(int value);

    // QWidget interface
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
};

NS_QRWIDGETS_END

#endif  //  QRLABEL_H
