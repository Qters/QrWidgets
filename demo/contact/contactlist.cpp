#include "contactlist.h"

#include <QtGui/qstandarditemmodel.h>

#include "contactmodel.h"
#include "contactdata.h"
#include "contanctlistitem.h"
#include "contactorwidget.h"
#include "contactordelegate.h"
#include "contactorrole.h"

class ContactListPrivate{
    QR_DECLARE_PUBLIC(ContactList)

public:
    QStandardItemModel *sourceModel = nullptr;
    ContactProxyModel *proxyModel = nullptr;

    ContactorDelegate *contactorDelegate = nullptr;

public:
    ContactListPrivate(ContactList *q);

public:
    void init();
};

ContactListPrivate::ContactListPrivate(ContactList *q) : q_ptr(q) {}

void ContactListPrivate::init()
{
    Q_Q(ContactList);

    sourceModel = new QStandardItemModel(q);

    proxyModel = new ContactProxyModel(q);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setSourceModel(sourceModel);

    q->setModel(proxyModel);

    contactorDelegate = new ContactorDelegate(q);
    q->setItemDelegate(contactorDelegate);
}

//////////////////////////////////////////////////////////
ContactList::ContactList(QWidget *parent) :
    QListView(parent),
    d_ptr(new ContactListPrivate(this))
{
    Q_D(ContactList);
    d->init();
}

ContactList::~ContactList()
{

}

void ContactList::init(const QVector<ContactData> &contacters)
{
    Q_D(ContactList);
    Q_FOREACH(ContactData contacter, contacters) {
        ContanctListItem* item = new ContanctListItem();
        item->init(contacter);

        d->sourceModel->appendRow(item);

        auto index = d->proxyModel->mapFromSource(item->index());
        d->proxyModel->setData(index, contacter.level, ContactorRole::LevelId);
        d->proxyModel->setData(index, contacter.name, ContactorRole::UserName);
    }
}

void ContactList::filter(int groupId, const QString& name)
{
    Q_D(ContactList);
    //  name convert chinese to pinyin
    QRegExp groupIdRegExp(QString::number(groupId));
    groupIdRegExp.setCaseSensitivity(Qt::CaseInsensitive);
    d->proxyModel->setFilterRole(ContactorRole::GroupId, groupIdRegExp);

    QRegExp nameRegExp(QString(".*%1.*").arg(name));
    nameRegExp.setCaseSensitivity(Qt::CaseInsensitive);
    d->proxyModel->setFilterRole(ContactorRole::UserName, nameRegExp);

    d->proxyModel->setFilterRegExp("");

    for(int rowIdx=0; rowIdx<d->proxyModel->rowCount(); ++rowIdx) {
        openPersistentEditor(d->proxyModel->index(rowIdx, 0));
    }
}

