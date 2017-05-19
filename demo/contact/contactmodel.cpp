#include "contactmodel.h"

#include <QtCore/qdebug.h>

#include "contactlist.h"
#include "contactorwidget.h"

class ContactProxyModelPrivate{
public:
     QMap<int, QRegExp> roleRegExps;
};

ContactProxyModel::ContactProxyModel(QObject *parent)
    : QSortFilterProxyModel (parent),
      d_ptr(new ContactProxyModelPrivate)
{

}

void ContactProxyModel::resetFilterRole(const QVector<QPair<int, QRegExp> >& roleRegExps)
{
    Q_D(ContactProxyModel);
    Q_FOREACH(auto roleRegExp, roleRegExps) {
        d->roleRegExps[roleRegExp.first] = roleRegExp.second;
    }
}

void ContactProxyModel::setFilterRole(int role, QRegExp roleRegExps)
{
    Q_D(ContactProxyModel);
    d->roleRegExps[role] = roleRegExps;
}

bool ContactProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    auto showFunc = [this, source_row, source_parent](){
//        ContactList* parentWidget = qobject_cast<ContactList*>(parent());
//        if(nullptr == parentWidget) {
//            qDebug() << "parent widget is nullptr";
//        }
//        ContactorWidget *contactorWidget = new ContactorWidget(parentWidget);
//        contactorWidget->init(0, "sss");
//        parentWidget->setIndexWidget(mapFromSource(sourceModel()->index(source_row, filterKeyColumn())),
//                                     contactorWidget);
    };

    Q_D(const ContactProxyModel);
    if (d->roleRegExps.isEmpty()) {
        showFunc();
        return true;
    }

    auto filterFunc = [this, source_row, source_parent](int role, QRegExp regExp){
        auto index = sourceModel()->index(source_row, filterKeyColumn(), source_parent);
        return regExp.exactMatch(sourceModel()->data(index, role).toString());
    };

    if (d->roleRegExps.isEmpty()) {
        if(filterFunc(Qt::DisplayRole, filterRegExp())) {
            showFunc();
        }
    }

    for(auto roleRegExp : d->roleRegExps.toStdMap()) {
        if(! filterFunc(roleRegExp.first, roleRegExp.second)) {
            return false;
        }
    }

    showFunc();

    return true;
}
