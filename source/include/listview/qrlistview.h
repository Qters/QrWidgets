#ifndef QRLISTVIEW_H
#define QRLISTVIEW_H

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qabstractscrollarea.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QRWIDGETSSHARED_EXPORT QrListVidewDelegate : public QObject
{
    Q_OBJECT

Q_SIGNALS:
    void dataChanged();

public:
    virtual int itemsSize() const = 0;
    virtual QWidget* createItemWidget() = 0;
    virtual void setItemWidgetByIndex(int index, QWidget* itemWidget) = 0;
};

class QrListViewPrivate;
class QRWIDGETSSHARED_EXPORT QrListView : public QAbstractScrollArea
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrListView)

public:
    QrListView(QWidget *parent = nullptr);
    ~QrListView();

public:
    void setDelegate(QrListVidewDelegate *delegate);
    QrListVidewDelegate* delegate();

protected:
    virtual void scrollContentsBy(int dx, int dy) override;
    virtual bool event(QEvent *) override;
    virtual void resizeEvent(QResizeEvent *) override;

private Q_SLOTS:
    void dataChanged();
};

NS_QRWIDGETS_END

#endif // QRLISTVIEW_H
