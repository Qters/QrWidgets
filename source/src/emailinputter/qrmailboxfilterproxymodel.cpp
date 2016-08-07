#include "emailinputter/qrmailboxfilterproxymodel.h"

USING_NS_QRWIDGETS;

QrMailboxFilterProxyModel::QrMailboxFilterProxyModel(QObject *parent /*= 0*/)
    : QSortFilterProxyModel (parent)
{

}

bool QrMailboxFilterProxyModel::filterAcceptsRow(int source_row,
                                               const QModelIndex &source_parent) const
{
    if (filterRegExp().isEmpty()) {
        return true;
    }

    auto index = sourceModel()->index(source_row, 0, source_parent);
    if (sourceModel()->rowCount(index) > 0
            && sourceModel()->columnCount(index) > 0) {
        return true;    //  not a leaf node
    }

    return sourceModel()->data(index).toString().contains(filterRegExp());
}
