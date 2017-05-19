#ifndef CONTACTORDELEGATE_H
#define CONTACTORDELEGATE_H


#include <QStyledItemDelegate>

class ContactorDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ContactorDelegate(QObject *parent = 0);

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // CONTACTORDELEGATE_H
