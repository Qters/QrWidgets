#ifndef FRIENDDELEGATE_H
#define FRIENDDELEGATE_H

#include "listgroupdelegate.h"

class FriendDelegatePrivate;
class FriendDelegate : public ListGroupDelegate
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(FriendDelegate)

public:
    FriendDelegate();

public:
    void appendFriend(int groupId, QString groupName,
                      QString username, QString sign);

    // QrListVidewDelegate interface
public:
    virtual QWidget *createItemWidget() override;
};

//////////////////////////////////////////////

class FriendDelegateDataPrivate;
class FriendDelegateData : public ListGroupData {
    QR_DECLARE_PRIVATE(FriendDelegateData)

public:
    FriendDelegateData();

public:
    int groupId = 0;
    QString groupName;
    QString username;
    QString sign;

    // ListviewExData interface
public:
    virtual QString groupKey() override;

    // QrListViewData interface
public:
    virtual QString key() const override;
    virtual QString toString() const override;
    virtual bool compare(const Qters::QrWidgets::QrListViewData *other) const override;
};

#endif // FRIENDDELEGATE_H
