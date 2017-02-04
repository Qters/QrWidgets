#ifndef QRFILTERLIST_H
#define QRFILTERLIST_H

#include <QtWidgets/qwidget.h>


#include "qrglobal.h"

#include "qrwidgets_global.h"

QT_BEGIN_NAMESPACE
class QListWidget;
QT_END_NAMESPACE

NS_QRWIDGETS_BEGIN

class QrFilterListPrivate;
class QRWIDGETSSHARED_EXPORT QrFilterList : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrFilterList)

public:
    QrFilterList(QWidget* parent = Q_NULLPTR);

Q_SIGNALS:
    void listClick(QString text);
    void filterStart();
    void filterFinish();

public:
    QListWidget* listWidget();

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;
};

NS_QRWIDGETS_END

#endif // QRFILTERLIST_H
