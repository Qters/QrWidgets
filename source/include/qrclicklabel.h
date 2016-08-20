#ifndef QRCLICKLABEL_H
#define QRCLICKLABEL_H

#include <QLabel>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QrClickLabelPrivate;

class QRWIDGETSSHARED_EXPORT QrClickLabel :public QLabel
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrClickLabel)

public:
    QrClickLabel(QWidget *parent = Q_NULLPTR);

signals:
    void clicked();

    // QWidget interface
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
};

NS_QRWIDGETS_END

#endif  //  QRCLICKLABEL_H
