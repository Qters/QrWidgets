﻿#ifndef QRTITLEBAR_H
#define QRTITLEBAR_H

#include <QtWidgets/QWidget>
#include <QtWidgets/qpushbutton.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

#include "qrmovable.h"

NS_QRWIDGETS_BEGIN

class QrTitleBarPrivate;
class QRWIDGETSSHARED_EXPORT QrTitleBar : public QrMovable
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrTitleBar)

public:
    enum BarFlags {
        Logo            = 0x00,
        Title           = 0x01,
        MinButton       = 0x02,
        MaxButton       = 0x04,
        CloseButton     = 0x08,
    };

public:
    QrTitleBar(QWidget *parent);
    ~QrTitleBar();

public:
    void setFlags(long flags);
    void setTitle(const QString& title);
    void setLogo(const QPixmap& logo);
    void defaultButtonStyle();

    // QWidget interface
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
};

NS_QRWIDGETS_END

#endif // QRTITLEBAR_H
