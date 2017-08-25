#ifndef GROUPCELL_H
#define GROUPCELL_H

#include <QWidget>

namespace Ui {
class GroupCell;
}

class GroupCell : public QWidget
{
    Q_OBJECT

public:
    explicit GroupCell(QWidget *parent = 0);
    ~GroupCell();

private:
    Ui::GroupCell *ui;
};

#endif // GROUPCELL_H
