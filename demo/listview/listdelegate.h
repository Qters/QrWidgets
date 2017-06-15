#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include "listview/qrlistview.h"
#include "listview/qrlistdelegate.h"
#include "qrglobal.h"

class ListCellInfo;

class ListCellData : public Qters::QrWidgets::QrListViewData {
public:
    QString username;
    QString selfSign;

    // QrListViewData interface
public:
    virtual bool compare(const Qters::QrWidgets::QrListViewData *other) const override;
};

class ListDelegatePrivate;
class ListDelegate : public Qters::QrWidgets::QrListVidewDelegate
{
    Q_OBJECT

public:
    ListDelegate();

Q_SIGNALS:
    void showInfo(ListCellData);

public:
    void appendUser(const QString& username, const QString& selfSign);

    // QrListVidewDelegate interface
public:
    virtual void initMenu(QMenu *menu) override;
    virtual bool hasMenu() const override;

    virtual QWidget *createItemWidget() override;
    virtual void setItemWidgetByData(Qters::QrWidgets::QrListViewData *data, QWidget *itemWidget) override;
    virtual void onDoubleClick(QWidget *item, Qters::QrWidgets::QrListViewData *data, bool click) override;
    virtual void onClick(QWidget *item, Qters::QrWidgets::QrListViewData *data, bool click) override;
};

#endif // LISTDELEGATE_H
