#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include "listview/qrlistview.h"

#include "qrglobal.h"

class ListCellData : public Qters::QrWidgets::QrListViewData {
public:
    QString username;
    QString selfSign;

    // QrListViewData interface
public:
    virtual bool filter(const QRegExp &regExp) const override;
    virtual bool compare(const Qters::QrWidgets::QrListViewData *other) const override;
};

class ListDelegate : public Qters::QrWidgets::QrListVidewDelegate
{
public:
    void appendUser(const QString& username, const QString& selfSign);

    // QrListVidewDelegate interface
public:
    virtual QWidget *createItemWidget() override;
    virtual void setItemWidgetByData(Qters::QrWidgets::QrListViewData *data, QWidget *itemWidget) override;
};

#endif // LISTDELEGATE_H
