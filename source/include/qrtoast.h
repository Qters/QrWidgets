#ifndef QRTOAST_H
#define QRTOAST_H

#include <QtCore/qobject.h>
#include <QtWidgets/qlabel.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QRWIDGETSSHARED_EXPORT QrToastInfo {
public:
    int timeout = 1500;
    QString message;
    QWidget* widget = nullptr;  //  default value, nullptr mean show in screen
    QLabel* toast = nullptr;

public:
    void calcPos(QSize toastSize, QPoint *toastPos, QRect *widgetRect) const;
};

/*!
 *  qlabel show like toast of android
 *
 * QrToast::instance()->showInScreen("msg");
 */

class QrToastPrivate;
class QRWIDGETSSHARED_EXPORT QrToast : public QObject
{
    Q_OBJECT
    QR_DECLARE_PRIVATE(QrToast)

public:
    static QrToast* instance();

private:
    QrToast();
    virtual ~QrToast();

public:
    void showInScreen(const QString& msg, int msecs = 1500);
    void showInWidget(QWidget* widget, const QString& msg, int msecs = 1500);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *);
};

NS_QRWIDGETS_END

#endif // QRTOAST_H
