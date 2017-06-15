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
    enum Style{
        Right,
        Down
    };

public:
    bool init(ListCellData *_data);
    void changeStyle(Style style);

private:
    Ui::ListCell *ui;
};

#endif // LISTCELL_H
