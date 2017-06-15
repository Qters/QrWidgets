#ifndef QRLISTDELEGATE_H
#define QRLISTDELEGATE_H

#include <functional>

#include <QtWidgets/qmenu.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QrListView;

/*!
 * \brief Listview's cell data
 */
class QrListViewDataPrivate;
class QRWIDGETSSHARED_EXPORT QrListViewData {

    QR_DECLARE_PRIVATE(QrListViewData)

public:
    QrListViewData();

public:
    virtual QString key() const;
    virtual bool compare(const QrListViewData *other) const;

public:
    bool isOnClick() const;
    void onClick(bool click);
    bool isOnDoubleClick() const;
    void onDoubleClick(bool click);
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
    //  notify to change listview's view
    void dataChanged();
    void dataEmpty(bool isEmpty);
    void dataFiltered(bool isEmpty);

public:
    virtual QWidget* createItemWidget() = 0;
    virtual void setItemWidgetByData(QrListViewData* data, QWidget* itemWidget) = 0;

    virtual void initMenu(QMenu* menu);
    virtual bool hasMenu() const;

    virtual void onDoubleClick(QWidget *item, QrListViewData* data, bool click);
    virtual void onClick(QWidget *item, QrListViewData* data, bool click);

public:
    void setListview(QrListView* listview);
    QrListView* listview();

    /*!
     * \brief should put data after YOU create(NEW), leave DELETE data to DELEGATE:
     * \param data
     */
    void addData(QrListViewData* data);
    void deleteData(QrListViewData* data);
    QrListViewData* getData(const QString& key);
    bool isDataExist(const QString& key) const;

    QrListViewData* menuData();
    QrListViewData* doubleClickData();
    QrListViewData* clickData();

    void sort();
    void filter(std::function<bool (QrListViewData*)> filterFunc);

    void clear();
    int itemsSize() const;
    int rawSize() const;
    QrListViewData* setItemWidgetByIndex(int index, QWidget *itemWidget);

    void refreshClickData(int index);
    void refreshDoubleClickData(int index);
};

NS_QRWIDGETS_END

#endif // QRLISTDELEGATE_H
