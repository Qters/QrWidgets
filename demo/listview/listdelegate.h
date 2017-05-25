#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include "listview/qrlistview.h"

#include "qrglobal.h"

class ListCellData : public Qters::QrWidgets::QrListViewData {
    // QrListViewData interface
public:
    virtual bool filter(const QRegExp &regExp) override;

public:
    QString username;
    QString selfSign;
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
