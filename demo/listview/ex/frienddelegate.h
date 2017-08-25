#ifndef FRIENDDELEGATE_H
#define FRIENDDELEGATE_H

#include "listviewexdelegate.h"

class FriendDelegatePrivate;
class FriendDelegate : public ListviewExDelegate
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
    virtual void setItemWidgetByData(Qters::QrWidgets::QrListViewData *data, QWidget *itemWidget) override;

    // ListviewExDelegate interface
public:
    virtual int normalItemHeight() const override;
    virtual int headItemHeight() const override;
};

//////////////////////////////////////////////

class FriendDelegateDataPrivate;
class FriendDelegateData : public ListviewExData {
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

    virtual bool isGroupData() const override;
};

#endif // FRIENDDELEGATE_H
