#include "contanctlistitem.h"

#include "contactdata.h"
#include "contactorrole.h"

class ContanctListItemPrivate{
    QR_DECLARE_PUBLIC(ContanctListItem)

public:

public:
    ContanctListItemPrivate(ContanctListItem *q);

public:

};

ContanctListItemPrivate::ContanctListItemPrivate(ContanctListItem *q)
    : q_ptr(q) {}

ContanctListItem::ContanctListItem()
    : d_ptr(new ContanctListItemPrivate(this))
{

}

ContanctListItem::ContanctListItem(const QString &text)
    : QStandardItem(text),
      d_ptr(new ContanctListItemPrivate(this))
{
}

ContanctListItem::ContanctListItem(const ContanctListItem &other)
    : QStandardItem(other),
      d_ptr(new ContanctListItemPrivate(this))
{
}

ContanctListItem &ContanctListItem::operator=(const ContanctListItem &other)
{
    QStandardItem::operator=(other);
    return *this;
}

void ContanctListItem::init(const ContactData &data)
{
    setData(data.groupid, ContactorRole::GroupId);
    setData(data.name, ContactorRole::UserName);

//    setText(data.name);
}

//  Returns true if this item is less than /a other; otherwise returns false.
bool ContanctListItem::operator <(const QStandardItem &other) const
{
    return data(ContactorRole::UserName).toString() < other.data(ContactorRole::UserName).toString();
}
