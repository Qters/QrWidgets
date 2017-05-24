#ifndef LISTCELL_H
#define LISTCELL_H

#include <QWidget>

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
    void init(ListCellData* data);

private:
    Ui::ListCell *ui;
};

#endif // LISTCELL_H
