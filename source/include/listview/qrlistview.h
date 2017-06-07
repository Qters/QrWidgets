#ifndef QRLISTVIEW_H
#define QRLISTVIEW_H

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qabstractscrollarea.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QrListVidewDelegate;

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

    QMenu* menu();

protected:
    virtual void scrollContentsBy(int dx, int dy) override;
    virtual bool event(QEvent *) override;
    virtual void resizeEvent(QResizeEvent *) override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private Q_SLOTS:
    void dataChanged();
};

NS_QRWIDGETS_END

#endif // QRLISTVIEW_H
