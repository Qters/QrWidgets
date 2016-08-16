#ifndef EZTOAST_H
#define EZTOAST_H

#include <QtCore/qobject.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

/*!
 *  qlabel show like toast of android
 *
 * QrToast::instance()->show("msg", QPoint(0, 0), 5000);
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

public:
    void show(const QString& msg,
              const QPoint& point,
              int msecs = 3000);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *);
};

NS_QRWIDGETS_END

#endif // EZTOAST_H
