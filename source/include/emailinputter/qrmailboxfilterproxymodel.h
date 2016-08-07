#ifndef MAILBOXFILTERPROXYMODEL_H
#define MAILBOXFILTERPROXYMODEL_H

#include <QtCore/qsortfilterproxymodel.h>

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QrMailboxFilterProxyModel : public QSortFilterProxyModel
{
public:
    QrMailboxFilterProxyModel(QObject *parent = 0);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const;
};

NS_QRWIDGETS_END

#endif // MAILBOXFILTERPROXYMODEL_H
