#include "groupdelegate.h"

#include "groupcell.h"

USING_NS_QRWIDGETS;

class GroupDelegatePrivate{
    QR_DECLARE_PUBLIC(GroupDelegate)

public:
    GroupDelegatePrivate(GroupDelegate *q);
};


GroupDelegatePrivate::GroupDelegatePrivate(GroupDelegate *q)
    : q_ptr(q)
{}

///////////////////////

GroupDelegate::GroupDelegate()
    : ListGroupDelegate(),
      d_ptr(new GroupDelegatePrivate(this))
{
    initItemHeights();
}

QWidget *GroupDelegate::createItemWidget()
{
    return new GroupCell();
}

//////////////////////////////////////

class GroupDelegateDataPrivate{
    QR_DECLARE_PUBLIC(GroupDelegateData)

public:
    GroupDelegateDataPrivate(GroupDelegateData *q);
};


GroupDelegateDataPrivate::GroupDelegateDataPrivate(GroupDelegateData *q)
    : q_ptr(q)
{}

///////////////////////

GroupDelegateData::GroupDelegateData()
    : ListGroupData(),
      d_ptr(new GroupDelegateDataPrivate(this))
{

}

QString GroupDelegateData::groupKey()
{
    return "";
}

