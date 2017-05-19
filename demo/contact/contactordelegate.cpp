#include "contactordelegate.h"

#include "contactorwidget.h"
#include "contactorrole.h"

ContactorDelegate::ContactorDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *ContactorDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem & option ,
    const QModelIndex & index ) const
{
    if(index.column() == 1) {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    ContactorWidget *editor = new ContactorWidget(parent);
    editor->init(index.model()->data(index, ContactorRole::LevelId).toInt(),
                 index.model()->data(index, ContactorRole::UserName).toString());
    return editor;
}
