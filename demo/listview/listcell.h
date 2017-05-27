#ifndef LISTCELL_H
#define LISTCELL_H

#include <QWidget>

#include "listview/qrlistview.h"
#include "listview/qrlistdelegate.h"

class ListCellData;

namespace Ui {
class ListCell;
}

class ListCell : public QWidget
{
    Q_OBJECT

public:
    explicit ListCell(QWidget *parent = 0);
    ~ListCell();

public:
    bool init(ListCellData *_data);

private:
    Ui::ListCell *ui;
};

#endif // LISTCELL_H
