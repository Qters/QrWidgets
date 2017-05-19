#ifndef CONTANCTLISTITEM_H
#define CONTANCTLISTITEM_H

#include <QStandardItem>

#include "qrglobal.h"

class ContactData;
class ContanctListItemPrivate;

class ContanctListItem : public QStandardItem
{
    QR_DECLARE_PRIVATE(ContanctListItem)

public:
    ContanctListItem();
    ContanctListItem(const QString &text);
    ContanctListItem(const ContanctListItem &other);
    ContanctListItem &operator=(const ContanctListItem &other);

public:
    void init(const ContactData& data);

    // QStandardItem interface
public:
    virtual bool operator <(const QStandardItem &other) const override;
};

#endif // CONTANCTLISTITEM_H
