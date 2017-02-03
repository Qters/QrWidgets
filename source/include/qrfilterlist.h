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

public:
    QListWidget* listWidget();
};

NS_QRWIDGETS_END

#endif // QRFILTERLIST_H
