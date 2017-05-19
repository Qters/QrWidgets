#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QtCore/qsortfilterproxymodel.h>

#include "qrglobal.h"

class ContactProxyModelPrivate;
class ContactProxyModel : public QSortFilterProxyModel
{
    QR_DECLARE_PRIVATE(ContactProxyModel)

public:
    ContactProxyModel(QObject *parent = 0);

public:
    void resetFilterRole(const QVector<QPair<int, QRegExp> >& roleRegExps);
    void setFilterRole(int role, QRegExp roleRegExps);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const;
};

#endif // CONTACTMODEL_H
