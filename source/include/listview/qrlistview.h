#ifndef QRLISTVIEW_H
#define QRLISTVIEW_H

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qabstractscrollarea.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

/*!
 * \brief Listview's cell data
 */
class QrListViewDataPrivate;
class QRWIDGETSSHARED_EXPORT QrListViewData {

    QR_DECLARE_PRIVATE(QrListViewData)

public:
    QrListViewData();

public:
    virtual bool filter(const QRegExp& regExp);
};

/*!
 * \brief Listview's Delegate
 *  1. auto manager data
 *  2. filter
 */
class QrListVidewDelegatePrivate;
class QRWIDGETSSHARED_EXPORT QrListVidewDelegate : public QObject
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrListVidewDelegate)

public:
    QrListVidewDelegate();

Q_SIGNALS:
    void dataChanged();

public:
    virtual QWidget* createItemWidget() = 0;
    virtual void setItemWidgetByData(QrListViewData* data, QWidget* itemWidget) = 0;

public:
    /*!
     * \brief should put data after YOU create(NEW), leave DELETE data to DELEGATE:
     * \param data
     */
    void managerData(QrListViewData* data);

    void filter(const QRegExp& regExp);
    int itemsSize() const;

    void setItemWidgetByIndex(int index, QWidget *itemWidget);
};

/*!
 * \brief Listview
 */
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
