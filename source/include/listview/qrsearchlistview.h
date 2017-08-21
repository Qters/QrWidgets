#ifndef QRSEARCHLISTVIEW_H
#define QRSEARCHLISTVIEW_H

#include <functional>

#include <QWidget>

#include "qrglobal.h"

#include "qrwidgets_global.h"

namespace Ui {
class QrSearchListview;
}

NS_QRWIDGETS_BEGIN

class QrListViewData;
class QrListVidewDelegate;

class QrSearchListviewPrivate;
class QRWIDGETSSHARED_EXPORT QrSearchListview : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrSearchListview)

public:
    explicit QrSearchListview(QWidget *parent = 0);
    ~QrSearchListview();

public:
    void setDelegate(QrListVidewDelegate *delegate,
                     std::function<bool (QString, QrListViewData*)> filterFunc);
    void setDataEmptyTips(const QString& tips);
    void setFilterEmptyTips(const QString& tips);

private:
    Ui::QrSearchListview *ui;
};

NS_QRWIDGETS_END

#endif // QRSEARCHLISTVIEW_H
