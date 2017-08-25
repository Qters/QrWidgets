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
class QrListViewData;
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
    void updateOne(const QString &key);

public:
    virtual void onDoubleClick();
    virtual void onClick();

protected:
    virtual void scrollContentsBy(int dx, int dy) override;
    virtual bool event(QEvent *) override;
    virtual void resizeEvent(QResizeEvent *) override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    virtual void dateChangeUpdate();
    virtual QWidget *createWidget();

protected:
    bool isScrollUp() const;
    void clearDataWidget();
    QQueue<QWidget*> itemWidgets();

    void setWidgetItemIndex(QWidget *itemWidget, int itemIndex);
    int getWidgetItemIndex(QWidget *itemWidget) const;

    void setDataWidget(QrListViewData *delegateData, QWidget *itemWidget);

    QrListViewData* getDataOfWidget(QWidget *itemWidget) const;

private Q_SLOTS:
    virtual void dataChanged();
};

NS_QRWIDGETS_END

#endif // QRLISTVIEW_H
