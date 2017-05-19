#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QtWidgets/qlistview.h>

#include "qrglobal.h"

class ContactData;

class ContactListPrivate;
class ContactList : public QListView
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(ContactList)

public:
    explicit ContactList(QWidget *parent = Q_NULLPTR);
    ~ContactList();

public:
    void init(const QVector<ContactData> &contacters);

public:
    void filter(int groupId, const QString& name);
};

#endif // CONTACTLIST_H
