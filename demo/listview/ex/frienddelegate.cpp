#include "frienddelegate.h"

#include "listview/qrlistdelegate.h"

#include "friendcell.h"

USING_NS_QRWIDGETS;

class FriendDelegatePrivate{
    QR_DECLARE_PUBLIC(FriendDelegate)

public:
    FriendDelegatePrivate(FriendDelegate *q);

public:
    int normalItemHeight = 0;
    int headItemHeigth = 0;
};


FriendDelegatePrivate::FriendDelegatePrivate(FriendDelegate *q)
    : q_ptr(q)
{
    FriendCell cell;
    normalItemHeight = cell.normalItemHeight()/* + 15*/;
    headItemHeigth = cell.headItemHeight();
}

///////////////////////

FriendDelegate::FriendDelegate()
    : ListGroupDelegate(),
      d_ptr(new FriendDelegatePrivate(this))
{

}

void FriendDelegate::appendFriend(int groupId, QString groupName,
                                  QString username, QString sign)
{
    FriendDelegateData* data = new FriendDelegateData;
    data->groupId = groupId;
    data->groupName = groupName;
    data->username = username;
    data->sign = sign;

    addData(data);
}

QWidget *FriendDelegate::createItemWidget()
{
    return new FriendCell();
}

int FriendDelegate::normalItemHeight() const
{
    Q_D(const FriendDelegate);
    return d->normalItemHeight;
}

int FriendDelegate::headItemHeight() const
{
    Q_D(const FriendDelegate);
    return d->headItemHeigth;
}

//////////////////////////////////////

class FriendDelegateDataPrivate{
    QR_DECLARE_PUBLIC(FriendDelegateData)

public:
    FriendDelegateDataPrivate(FriendDelegateData *q);
};


FriendDelegateDataPrivate::FriendDelegateDataPrivate(FriendDelegateData *q)
    : q_ptr(q)
{}

///////////////////////

FriendDelegateData::FriendDelegateData()
    : ListGroupData(),
      d_ptr(new FriendDelegateDataPrivate(this))
{

}

QString FriendDelegateData::groupKey()
{
    return QString::number(groupId) + groupName;
}

bool FriendDelegateData::compare(const QrListViewData *other) const
{
    const FriendDelegateData* otherData = static_cast<const FriendDelegateData*>(other);
    if(nullptr == otherData) {
        return false;
    }

    if(groupId == otherData->groupId) {
        return username < otherData->username;
    }
    return groupId < otherData->groupId;
}

bool FriendDelegateData::isGroupData() const
{
    return true;
}

QString FriendDelegateData::key() const
{
    return username;
}

QString FriendDelegateData::toString() const
{
    return QString::number(groupId) + " " + groupName + "," + username;
}
