#ifndef GROUPDELEGATE_H
#define GROUPDELEGATE_H

#include "listviewexdelegate.h"

class GroupDelegatePrivate;
class GroupDelegate : public ListviewExDelegate
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(GroupDelegate)

public:
    GroupDelegate();

    // QrListVidewDelegate interface
public:
    virtual QWidget *createItemWidget() override;
    virtual void setItemWidgetByData(Qters::QrWidgets::QrListViewData *data, QWidget *itemWidget) override;

    // ListviewExDelegate interface
public:
    virtual int normalItemHeight() const override;
    virtual int headItemHeight() const override;
};

//////////////////////////////////////////////

class GroupDelegateDataPrivate;
class GroupDelegateData : public ListviewExData {
    QR_DECLARE_PRIVATE(GroupDelegateData)

public:
    GroupDelegateData();

    // ListviewExData interface
public:
    virtual QString groupKey() override;
};

#endif // GROUPDELEGATE_H
