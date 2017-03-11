#ifndef QRTITLEBAR_H
#define QRTITLEBAR_H

#include <QtWidgets/QWidget>
#include <QtWidgets/qpushbutton.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

class QrTitleBarPrivate;
class QRWIDGETSSHARED_EXPORT QrTitleBar : public QWidget
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
        BoxEffect       = 0x10,
    };

public:
    QrTitleBar(QWidget *parent);
    ~QrTitleBar();

public:
    void setFlags(BarFlags flags);
    void setTitle(const QString& title);
    void setLogo(const QPixmap& logo);
    void setBoxColor(const QColor& boxColor);
    void donotMove(QPushButton *button);

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // QRTITLEBAR_H
