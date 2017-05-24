#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include "listview/qrlistview.h"

#include "qrglobal.h"

class ListCellData {
public:
    QString username;
    QString selfSign;
};

class ListDelegatePrivate;
class ListDelegate : public Qters::QrWidgets::QrListVidewDelegate
{
    QR_DECLARE_PRIVATE(ListDelegate)

public:
    ListDelegate();
    virtual ~ListDelegate();

public:
    void appendUser(const QString& username, const QString& selfSign);

    // QrListVidewDelegate interface
public:
    virtual int itemsSize() const override;
    virtual QWidget *createItemWidget() override;
    virtual void setItemWidgetByIndex(int index, QWidget *itemWidget) override;
};

#endif // LISTDELEGATE_H
