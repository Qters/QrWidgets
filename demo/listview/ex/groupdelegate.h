#ifndef GROUPDELEGATE_H
#define GROUPDELEGATE_H

#include "listgroupdelegate.h"

class GroupDelegatePrivate;
class GroupDelegate : public ListGroupDelegate
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(GroupDelegate)

public:
    GroupDelegate();

    // QrListVidewDelegate interface
public:
    virtual QWidget *createItemWidget() override;

    // ListviewExDelegate interface
public:
    virtual int normalItemHeight() const override;
    virtual int headItemHeight() const override;
};

//////////////////////////////////////////////

class GroupDelegateDataPrivate;
class GroupDelegateData : public ListGroupData {
    QR_DECLARE_PRIVATE(GroupDelegateData)

public:
    GroupDelegateData();

    // ListviewExData interface
public:
    virtual QString groupKey() override;
};

#endif // GROUPDELEGATE_H
