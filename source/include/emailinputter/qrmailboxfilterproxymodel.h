#ifndef QRMAILBOXFILTERPROXYMODEL_H
#define QRMAILBOXFILTERPROXYMODEL_H

#include <QtCore/qsortfilterproxymodel.h>

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QRWIDGETSSHARED_EXPORT QrMailboxFilterProxyModel : public QSortFilterProxyModel
{
public:
    QrMailboxFilterProxyModel(QObject *parent = 0);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const;
};

NS_QRWIDGETS_END

#endif // QRMAILBOXFILTERPROXYMODEL_H
